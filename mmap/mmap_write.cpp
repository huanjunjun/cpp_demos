#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
using namespace std;

int main(){

    int fd = open("hjmap", O_RDWR | O_CREAT, 0666);//创建一个文件
    if(fd == -1){
        perror("open");
        exit(1);
    }
    ftruncate(fd, 4096);//设置文件大小
    // mmap的参数：映射的起始地址，映射的大小，映射的权限，映射的标志，文件描述符，在文件中的偏移
    char *p = (char *)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(p == MAP_FAILED){
        perror("mmap");
        exit(1);
    }
    close(fd);//映射建立后，文件描述符就可以关闭了
    while(1){
        cin>>p;
        cout<<"buffer size:"<<strlen(p)<<endl;
    }
    munmap(p, 4096);//解除映射
    exit(0);
}