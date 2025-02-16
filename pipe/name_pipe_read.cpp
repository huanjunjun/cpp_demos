#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/signal.h>//这个头文件中定义了信号相关的函数
#include<unistd.h>//这个头文件中定义的函数有fork(),pipe(),read(),write(),close()等
#include<fcntl.h>//这个头文件中定义了O_NONBLOCK
#include<iostream>
using namespace std;


int main(){
    int name_pipe_fd = open("hjfifo", O_RDONLY);
    if(name_pipe_fd == -1){
        perror("open hjfifo");
        exit(1);
    }
    char buffer[1024];
    while(1){
        ssize_t bytes_read = read(name_pipe_fd, buffer, sizeof(buffer));
        if(bytes_read == -1){
            perror("read");
            exit(1);
        }
        printf("Received: %s\n", buffer);
    }
    exit(0);
}