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


using std::placeholders::_1;

class FaceNode : public rclcpp::Node
{
  public:
    FaceNode()
    : Node("face_node")
    {
      subscription_ = this->create_subscription<std_msgs::msg::String>(
      "face/emotion", 10, std::bind(&FaceNode::topic_callback, this, _1));
      
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
          RCLCPP_ERROR(this->get_logger(), "Socket creation error: %s", strerror(errno));
          return;
      }

      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(port);
      if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
          RCLCPP_ERROR(this->get_logger(), "Invalid address: %s", strerror(errno));
          close(sock);
          return;
      }




      
    }

  private:
    void topic_callback(const std_msgs::msg::String & msg) const
    {
      RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
      RCLCPP_INFO(this->get_logger(), "Attempting to connect...");
      int flags = fcntl(sock, F_GETFL, 0);
      fcntl(sock, F_SETFL, flags | O_NONBLOCK);

      connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));


      fd_set set;
      FD_ZERO(&set);
      FD_SET(sock, &set);

      struct timeval timeout;
      timeout.tv_sec = 2;
      timeout.tv_usec = 0;
      
      int connection_result = select(sock + 1, NULL, &set, NULL, &timeout);

      if (connection_result <= 0) {
          RCLCPP_ERROR(this->get_logger(), "Connection failed: %s", strerror(errno));
          close(sock);
          return;
      } else {
          RCLCPP_INFO(this->get_logger(), "Connected to server");
      }
      fcntl(sock, F_SETFL, flags);


      std::string command = msg.data;

      std::transform(command.begin(), command.end(), command.begin(), ::tolower);
      if (std::find(valid_emotions.begin(), valid_emotions.end(), command) == valid_emotions.end()) {
          RCLCPP_ERROR(this->get_logger(), "Invalid command: %s", command.c_str());
          return;
      }

      int bytes_sent = send(sock, command.c_str(), command.length(), 0);
      if (bytes_sent < 0) {
          RCLCPP_ERROR(this->get_logger(), "Failed to send command: %s", strerror(errno));
          close(sock);
          return;
      }


    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    int sock = 0;
    struct sockaddr_in serv_addr;
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