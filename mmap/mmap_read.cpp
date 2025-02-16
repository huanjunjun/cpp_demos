#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
using namespace std;


int main(){
    // 打开文件 "hjmap"，如果文件不存在则创建，权限为读写，文件权限为0666
    int fd = open("hjmap", O_RDWR | O_CREAT, 0666);
    if(fd == -1){
        // 如果打开文件失败，输出错误信息并退出程序
        perror("open");
        exit(1);
    }
    // 设置文件大小为4096字节
    ftruncate(fd, 4096);
    // 使用mmap函数将文件映射到内存中，映射的起始地址为NULL，映射的大小为4096字节，映射的权限为读写，映射的标志为共享，文件描述符为fd，在文件中的偏移为0
    char *p = (char *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        // 如果映射失败，输出错误信息并退出程序
        perror("mmap");
        exit(1);
    }
    // 关闭文件描述符，因为映射建立后，文件描述符就可以关闭了
    close(fd);
    while(1){
        // 程序休眠1秒
        sleep(1);
        // 输出接收到的内容
        cout<<"Received: "<<p<<endl;
    }
    // 解除内存映射
    munmap(p, 4096);
    // 程序正常退出
    exit(0);
}