#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/signal.h>//这个头文件中定义了信号相关的函数
#include<unistd.h>//这个头文件中定义的函数有fork(),pipe(),read(),write(),close()等
#include<fcntl.h>//这个头文件中定义了O_NONBLOCK
#include <sys/types.h>
#include <sys/stat.h>
#include<iostream>
using namespace std;


int main(){
    if (access("hjfifo", F_OK) == -1) {
        if (mkfifo("hjfifo", 0666) == -1) {
            perror("mkfifo");
            exit(1);
        }
    }
    int name_pipe_fd = open("hjfifo", O_WRONLY);
    if(name_pipe_fd == -1){
        perror("open");
        exit(1);
    }
    char buffer[1024];
    
    while(1){
        // cin>>buffer;
        // cout<<"buffer size:"<<strlen(buffer)<<endl;
        // write(name_pipe_fd, buffer, strlen(buffer)+1);
        write(name_pipe_fd,"hello huanjun",14);
        
    }
    close(name_pipe_fd);
    exit(0);
}