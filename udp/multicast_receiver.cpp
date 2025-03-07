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

    // 允许端口复用
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = INADDR_ANY;
    local_addr.sin_port = htons(1900);

    if (bind(sockfd, (sockaddr*)&local_addr, sizeof(local_addr)) < 0) {
        std::cerr << "绑定端口失败" << std::endl;
        close(sockfd);
        return 1;
    }

    // 加入组播组
    ip_mreq multicast_group;
    multicast_group.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    multicast_group.imr_interface.s_addr = INADDR_ANY;
    setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
              &multicast_group, sizeof(multicast_group));

    std::cout << "组播接收端已启动，等待数据..." << std::endl;

    char buffer[1024];
    sockaddr_in src_addr;
    socklen_t addr_len = sizeof(src_addr);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int recv_len = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                              (sockaddr*)&src_addr, &addr_len);
        
        if (recv_len > 0) {
            char src_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &src_addr.sin_addr, src_ip, INET_ADDRSTRLEN);
            std::cout << "来自 " << src_ip << " 的组播数据: " << buffer << std::endl;
        }
    }

    close(sockfd);
    return 0;
}
