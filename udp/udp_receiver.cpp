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
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(12345); // 监听端口，可修改

    if (bind(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "绑定端口失败" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "UDP服务端已启动，监听端口 12345..." << std::endl;

    char buffer[1024];
    sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                               (sockaddr*)&client_addr, &addr_len);
        
        if (recv_len < 0) {
            std::cerr << "接收数据错误" << std::endl;
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "收到来自 " << client_ip << ":" << ntohs(client_addr.sin_port)
                  << " 的数据: " << buffer << std::endl;
    }

    close(sockfd);
    return 0;
}
