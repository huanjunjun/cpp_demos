#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <csignal>
#include "2process_pool.cpp"

// 服务器配置常量
const int PORT = 8080;       // 服务器监听端口
const int BUFFER_SIZE = 1024; // 接收缓冲区大小

/**
 * 处理客户端请求
 * @param clientSocket 客户端socket文件描述符
 */
void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // 读取客户端请求数据
    bytesRead = read(clientSocket, buffer, BUFFER_SIZE - 1);
    if (bytesRead < 0) {
        std::cerr << "读取socket数据错误" << std::endl;
        close(clientSocket);
        return;
    }
    buffer[bytesRead] = '\0'; // 添加字符串结束符

    // 打印接收到的请求
    std::cout << "收到请求:\n" << buffer << std::endl;

    // 发送HTTP响应
    const char* response =
        "HTTP/1.1 200 OK\r\n"          // HTTP协议版本和状态码
        "Content-Type: text/plain\r\n" // 内容类型
        "Content-Length: 13\r\n"       // 内容长度
        "\r\n"                         // 空行分隔头部和主体
        "Hello, World!";               // 响应主体
    write(clientSocket, response, strlen(response));

    // 关闭客户端连接
    close(clientSocket);
    std::cout << "客户端连接已关闭" << std::endl;
}

/**
 * 主函数 - HTTP服务器入口
 * @return 程序退出状态码
 */
int main() {
    int serverSocket;
    struct sockaddr_in serverAddr;

    // 创建服务器socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "创建socket失败" << std::endl;
        return 1;
    }

    // 配置服务器地址结构
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;         // IPv4地址族
    serverAddr.sin_addr.s_addr = INADDR_ANY; // 监听所有网络接口
    serverAddr.sin_port = htons(PORT);       // 设置监听端口

    // 绑定socket到指定地址和端口
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "绑定socket失败" << std::endl;
        close(serverSocket);
        return 1;
    }

    // 开始监听客户端连接
    if (listen(serverSocket, 10) < 0) {
        std::cerr << "监听socket失败" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "服务器正在监听端口 " << PORT << "..." << std::endl;

    // 创建进程池，4个工作进程
    ProcessPool pool(4);

    // 主循环，持续接受客户端连接
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        
        // 接受新的客户端连接
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            std::cerr << "接受连接失败" << std::endl;
            continue;
        }

        // 打印客户端连接信息
        std::cout << "新客户端连接: " 
                  << inet_ntoa(clientAddr.sin_addr) << ":" 
                  << ntohs(clientAddr.sin_port) << std::endl;

        // 将客户端处理任务提交到进程池
        pool.submit([clientSocket]() {
            handleClient(clientSocket);
        });
    }

    // 关闭服务器socket（通常不会执行到这里）
    close(serverSocket);
    return 0;
}
