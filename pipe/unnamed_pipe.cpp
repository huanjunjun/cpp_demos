#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/signal.h>//这个头文件中定义了信号相关的函数
#include<unistd.h>//这个头文件中定义的函数有fork(),pipe(),read(),write(),close()等
#include<fcntl.h>//这个头文件中定义了O_NONBLOCK
#include<iostream>
using namespace std;


int main(){
    int fd[2];
    char buffer[1024];
    pipe(fd);
    if(fork() == 0){
        // 从标准输入读取数据到字符串str
        // string str;
        // cin>>str;
        cin>>buffer;
        //将str写入管道
        close(fd[0]);
        // write(fd[1], str.c_str() , str.size());//写入数据
        write(fd[1], buffer, sizeof(buffer));//写入数据
        exit(0);
    }else{
        close(fd[1]);
        // fcntl(fd[0], F_SETFL, O_NONBLOCK);
        read(fd[0], buffer, sizeof(buffer));//读取数据
        printf("Received: %s\n", buffer);
        // printf("Received: %d\n", buffer);
        exit(0);
    }
}