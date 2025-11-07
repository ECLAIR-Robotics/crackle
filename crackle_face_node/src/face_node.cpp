#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <algorithm>
#include <signal.h>

using std::placeholders::_1;

class FaceNode : public rclcpp::Node
{
public:
  FaceNode()
  : Node("face_node")
  {
    // Prevent SIGPIPE from terminating the process on send()
    signal(SIGPIPE, SIG_IGN);

    subscription_ = this->create_subscription<std_msgs::msg::String>(
        "face/emotion", 10, std::bind(&FaceNode::topic_callback, this, _1));

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
      RCLCPP_ERROR(this->get_logger(), "Invalid address: %s", strerror(errno));
    }
  }

  ~FaceNode() override {
    if (sock >= 0) close(sock);
  }

private:
  // Create socket if needed
  bool ensure_socket() {
    if (sock >= 0) return true;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      RCLCPP_ERROR(this->get_logger(), "Socket creation error: %s", strerror(errno));
      return false;
    }
    // Optional: quick reuse
    int yes = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    return true;
  }

  // Connect with non-blocking + select timeout
  bool connect_with_timeout(int timeout_sec = 2) {
    if (!ensure_socket()) return false;

    // Set non-blocking for connect
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    int rc = ::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (rc == 0) {
      // Already connected
      fcntl(sock, F_SETFL, flags);
      return true;
    }
    if (rc < 0 && errno != EINPROGRESS) {
      RCLCPP_DEBUG(this->get_logger(), "Immediate connect() error: %s", strerror(errno));
      fcntl(sock, F_SETFL, flags);
      ::close(sock); sock = -1;
      return false;
    }

    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(sock, &wfds);
    struct timeval tv { .tv_sec = timeout_sec, .tv_usec = 0 };
    rc = select(sock + 1, nullptr, &wfds, nullptr, &tv);
    if (rc <= 0) {
      RCLCPP_DEBUG(this->get_logger(), "Connect select timeout/failure: %s", (rc==0?"timeout":strerror(errno)));
      fcntl(sock, F_SETFL, flags);
      ::close(sock); sock = -1;
      return false;
    }

    // Check for connect error via getsockopt
    int so_error = 0; socklen_t len = sizeof(so_error);
    getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);
    fcntl(sock, F_SETFL, flags);
    if (so_error != 0) {
      RCLCPP_DEBUG(this->get_logger(), "Connect SO_ERROR: %s", strerror(so_error));
      ::close(sock); sock = -1;
      return false;
    }

    return true;
  }

  // Try to ensure we are connected; reconnect if needed
  bool ensure_connected() {
    if (sock < 0) {
      if (!connect_with_timeout()) return false;
      RCLCPP_INFO(this->get_logger(), "Connected to server");
      return true;
    }

    // Cheap check: getpeername to see if still connected
    struct sockaddr peer; socklen_t len = sizeof(peer);
    if (getpeername(sock, &peer, &len) == -1) {
      // Not connected
      ::close(sock); sock = -1;
      if (!connect_with_timeout()) return false;
      RCLCPP_INFO(this->get_logger(), "Reconnected to server");
    }
    return true;
  }

  // Send with one reconnect attempt on EPIPE/ECONNRESET
  bool send_command_with_reconnect(const std::string &cmd) {
    if (!ensure_connected()) return false;

    ssize_t sent = ::send(sock, cmd.c_str(), cmd.size(), MSG_NOSIGNAL);
    if (sent >= 0) return true;

    if (errno == EPIPE || errno == ECONNRESET) {
      RCLCPP_WARN(this->get_logger(), "Send failed (%s). Reconnecting…", strerror(errno));
      ::close(sock); sock = -1;
      if (!connect_with_timeout()) {
        RCLCPP_ERROR(this->get_logger(), "Reconnect failed: %s", strerror(errno));
        return false;
      }
      sent = ::send(sock, cmd.c_str(), cmd.size(), MSG_NOSIGNAL);
      if (sent >= 0) return true;
      RCLCPP_ERROR(this->get_logger(), "Send after reconnect failed: %s", strerror(errno));
      return false;
    }

    RCLCPP_ERROR(this->get_logger(), "Send failed: %s", strerror(errno));
    return false;
  }

  // NOTE: removed 'const' so we can recreate the socket fd if needed
  void topic_callback(const std_msgs::msg::String & msg)
  {
    std::string command = msg.data;
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    if (std::find(valid_emotions.begin(), valid_emotions.end(), command) == valid_emotions.end()) {
      RCLCPP_ERROR(this->get_logger(), "Invalid command: %s", command.c_str());
      return;
    }

    if (!send_command_with_reconnect(command)) {
      RCLCPP_ERROR(this->get_logger(), "Failed to send command (even after reconnect).");
      return;
    }

    RCLCPP_INFO(this->get_logger(), "Sent command: %s", command.c_str());
  }

  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  int sock = -1;
  struct sockaddr_in serv_addr{};
  const char* server_ip = "127.0.0.1";
  const int port = 8080;
  const std::vector<std::string> valid_emotions = {
      "happy", "sad", "angry", "bored", "not_impressed",
      "evil_idea", "flirty", "aww", "wtf"
  };
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<FaceNode>());
  rclcpp::shutdown();
  return 0;
}
