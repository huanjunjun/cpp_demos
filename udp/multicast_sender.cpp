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

    // 设置组播TTL（生存时间）
    unsigned char ttl = 1;  // 限制在局域网内传输
    setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));

    sockaddr_in multicast_addr;
    memset(&multicast_addr, 0, sizeof(multicast_addr));
    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_addr.s_addr = inet_addr("239.255.255.250");  // 标准组播地址
    multicast_addr.sin_port = htons(1900);  // SSDP常用端口

    std::cout << "组播发送端已启动 (239.255.255.250:1900)..." << std::endl;

    char buffer[1024];
    while (true) {
        std::cout << "输入广播消息: ";
        std::cin.getline(buffer, sizeof(buffer));
        
        int sent_len = sendto(sockfd, buffer, strlen(buffer), 0,
                            (sockaddr*)&multicast_addr, sizeof(multicast_addr));
        if (sent_len < 0) {
            std::cerr << "发送失败" << std::endl;
        }
    }

    close(sockfd);
    return 0;
}
