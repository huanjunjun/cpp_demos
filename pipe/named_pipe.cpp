#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/signal.h>//这个头文件中定义了信号相关的函数
#include<unistd.h>//这个头文件中定义的函数有fork(),pipe(),read(),write(),close()等
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<iostream>
using namespace std;

int main(){
    char buffer[1024];
    //创建一个命名管道
    int ret = mkfifo("fifo", 0666);
    if(ret == -1){
        perror("mkfifo");
        exit(1);
    }
    //打开管道

    
    int pid = fork();
    if(pid == 0){
        // pid == 0 代表子进程
        int fd;
        fd = open("fifo", O_RDONLY);
        if(fd == -1){
            perror("open");
            exit(1);
        }
        read(fd, buffer, sizeof(buffer));
        printf("Received: %s\n", buffer);

        exit(0);
    }

    int fd = open("fifo", O_RDWR);
    if(fd == -1){
        perror("open");
        exit(1);
    }
    cin>>buffer;
    write(fd, buffer, sizeof(buffer));
    exit(0);




}