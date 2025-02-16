#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/syscall.h>
#include<iostream>
using namespace std;

/**
 * 
 * 在POSIX线程库（pthreads）中，线程执行函数的参数和返回值类型都是void*，这是为了提供最大的灵活性和通用性。
 * 参数类型为void*
 * 线程执行函数的参数类型为void*，
 * 这意味着可以传递任何类型的数据指针给线程函数。
 * 通过这种方式，线程函数可以接收不同类型的参数，
 * 而不需要在函数定义时指定具体的类型。调用线程函数时，
 * 可以将所需的数据类型转换为void*，
 * 然后在线程函数内部再将其转换回原来的类型。例如：
 */



void* task(void* arg){
    for(int i='a';i<='z';i++){
        cout<<"thread id:"<<syscall(SYS_gettid);
        cout<<" value:"<<(char)i<<endl;
    }
}  


 
int main(){
    pthread_t tid1,tid2;
    
    pthread_create(&tid1,NULL,task,NULL);
    pthread_create(&tid2,NULL,task,NULL);


    /**
     * 函数的作用是将一个线程的状态设置为分离状态（detached state）。
     * 当一个线程被设置为分离状态时，它的资源会在线程终止时自动释放，
     * 而不需要其他线程调用pthread_join来回收它的资源。
     */
    pthread_detach(tid1);
    pthread_detach(tid2);
    
    pause();
    
    return 0;
}