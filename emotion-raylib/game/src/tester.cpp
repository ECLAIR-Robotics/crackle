#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <fcntl.h>  // Added for fcntl()
#include <errno.h>  // Added for errno

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char* server_ip = "127.0.0.1";
    const int port = 8080;
    
    std::cout << "Emotion Tester Client\n";
    std::cout << "Connecting to " << server_ip << ":" << port << "\n";
    std::cout << "Available commands: happy, sad, angry, exit\n\n";

    while (true) {
        // Create socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
            std::cerr << "Socket creation error: " << strerror(errno) << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
            std::cerr << "Invalid address: " << strerror(errno) << std::endl;
            close(sock);
            return -1;
        }

        // Connect to server with timeout
        std::cout << "Attempting to connect..." << std::endl;
        
        // Set socket to non-blocking for timeout
        int flags = fcntl(sock, F_GETFL, 0);
        fcntl(sock, F_SETFL, flags | O_NONBLOCK);
        
        connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        
        fd_set set;
        FD_ZERO(&set);
        FD_SET(sock, &set);
        
        struct timeval timeout;
        timeout.tv_sec = 2;  // 2 second timeout
        timeout.tv_usec = 0;
        
        int connect_result = select(sock + 1, NULL, &set, NULL, &timeout);
        
        if (connect_result <= 0) {
            std::cerr << "Connection failed: " << (connect_result == 0 ? "Timeout" : strerror(errno)) << std::endl;
            close(sock);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        // Set socket back to blocking
        fcntl(sock, F_SETFL, flags);

        // Get user input
        std::string command;
        std::cout << "Enter emotion (happy/sad/angry) or 'exit': ";
        std::getline(std::cin, command);

        if (command == "exit") {
            close(sock);
            break;
        }

        if (command != "happy" && command != "sad" && command != "angry") {
            std::cout << "Invalid command. Try again.\n";
            close(sock);
            continue;
        }

        // Send command to server
        int bytes_sent = send(sock, command.c_str(), command.length(), 0);
        if (bytes_sent < 0) {
            std::cerr << "Send failed: " << strerror(errno) << std::endl;
        } else {
            std::cout << "Sent: " << command << std::endl;
        }
        
        close(sock);
    }

    std::cout << "Exiting tester program.\n";
    return 0;
}