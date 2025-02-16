#include <iostream>
#include <unistd.h>      // for fork(), close()
#include <sys/socket.h>  // for socket(), bind(), listen(), accept()
#include <netinet/in.h>  // for sockaddr_in
#include <arpa/inet.h>   // for inet_ntoa()
#include <cstring>       // for memset()
#include <csignal>       // for signal()
#include <sys/wait.h>    // for waitpid()

const int PORT = 8080;   // 监听的端口号
const int BUFFER_SIZE = 1024;

// 处理客户端请求的函数
void handleClient(int clientSocket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // 读取客户端请求
    bytesRead = read(clientSocket, buffer, BUFFER_SIZE - 1);
    if (bytesRead < 0) {
        std::cerr << "Error reading from socket" << std::endl;
        close(clientSocket);
        return;
    }
    buffer[bytesRead] = '\0';

    // 打印客户端请求
    std::cout << "Received request:\n" << buffer << std::endl;

    // 发送简单的 HTTP 响应
    const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 13\r\n"
        "\r\n"
        "Hello, World!";
    write(clientSocket, response, strlen(response));

    // 关闭客户端连接
    close(clientSocket);
    std::cout << "Client connection closed." << std::endl;
}

// 回收僵尸进程
void reapZombies(int signal) {
    while (waitpid(-1, nullptr, WNOHANG) > 0);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // 创建服务器 socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // 绑定服务器地址和端口
    memset(&serverAddr, 0, sizeof(serverAddr));//memset() is a function that fills a block of memory with a particular value. It takes three arguments: a pointer to the block of memory, the value that you want to fill the memory with, and the number of bytes to be filled.
    serverAddr.sin_family = AF_INET;//AF_INET is an address family that is used to designate the type of addresses that your socket can communicate with (in this case, Internet Protocol v4 addresses). When you create a socket, you have to specify its address family, and then you can only use addresses of that type with the socket.
    serverAddr.sin_addr.s_addr = INADDR_ANY;//INADDR_ANY is a constant that represents the IP address and port number of the machine on which the server is running. It is used in the bind() function to associate the server with the IP address and port number of the machine.
    serverAddr.sin_port = htons(PORT); //htons() is a function that converts the unsigned short integer hostshort from host byte order to network byte order. The host byte order is the byte order of the machine that the code is running on, while the network byte order is the byte order used on the network.

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {//The bind() function is used on the server side to associate a socket with a specific IP address and port number. It takes three arguments: the file descriptor of the socket to bind, a pointer to a sockaddr structure that contains the IP address and port number to bind to, and the size of the sockaddr structure.
        std::cerr << "Error binding socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    // 开始监听
    if (listen(serverSocket, 10) < 0) {//The listen() function is used on the server side to set up the socket to accept incoming client connections. It takes two arguments: the file descriptor of the socket to listen on, and the maximum number of client connections that can be queued up before the server starts to refuse new connections.
        std::cerr << "Error listening on socket" << std::endl;
        close(serverSocket);
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    // 设置信号处理函数，回收僵尸进程
    signal(SIGCHLD, reapZombies);

    while (true) {
        // 接受客户端连接
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }
 
        std::cout << "New client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

        // 创建子进程处理客户端请求
        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "Error forking process" << std::endl;
            close(clientSocket);
        } else if (pid == 0) {
            // 子进程
            close(serverSocket); // 子进程不需要监听 socket
            handleClient(clientSocket);
            exit(0); // 处理完成后退出子进程
        } else {
            // 父进程
            close(clientSocket); // 父进程不需要客户端 socket
        }
    }

    // 关闭服务器 socket（通常不会执行到这里）
    close(serverSocket);
    return 0;
}