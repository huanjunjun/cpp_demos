#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "创建socket失败" << std::endl;
        return 1;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345); // 服务端端口
    
    // 获取目标服务器IP
    char server_ip[16];
    std::cout << "输入服务端IP: ";
    std::cin >> server_ip;
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    char buffer[1024];
    socklen_t addr_len = sizeof(server_addr);

    while (true) {
        std::cout << "输入要发送的消息 (输入exit退出): ";
        std::cin.getline(buffer, sizeof(buffer));
        
        if (strcmp(buffer, "exit") == 0) break;
        
        // 发送数据
        int sent_len = sendto(sockfd, buffer, strlen(buffer), 0,
                            (sockaddr*)&server_addr, addr_len);
        if (sent_len < 0) {
            std::cerr << "发送失败" << std::endl;
            continue;
        }

        // 接收服务端响应
        char response[1024];
        int recv_len = recvfrom(sockfd, response, sizeof(response), 0,
                              (sockaddr*)&server_addr, &addr_len);
        if (recv_len > 0) {
            response[recv_len] = '\0';
            std::cout << "服务端响应: " << response << std::endl;
        }
    }

    close(sockfd);
    return 0;
}
