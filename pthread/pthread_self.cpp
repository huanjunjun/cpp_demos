#include<pthread.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include<stdio.h>


int main(){
    
    long int  tid = syscall(SYS_gettid);
    printf("Thread ID is %ld\n", tid);
    // syscall(SYS_gettid) 返回的是操作系统对线程的编号ID

    pthread_t ptid = pthread_self();
    printf("posix thread ID is %ld\n", ptid);
    // pthread_self() 返回的是POSIX线程库对线程的编号ID

    pause();
    // 在这里暂停，打开终端输入 ps -aux ，可以看到当前进程的id与 syscall(SYS_gettid) 返回的ID是一样的
    return 0;
}