#include<stdio.h>
#include<pthread.h>
#include<string.h>
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





void* task1(void* arg){
    
    string* ret12 = new string("hello");
    //此处的返回值必须是一个指针，所以需要使用new来分配内存，否则会出现段错误
    // 如果 string ret12 = "hello"; return &ret12; 会出现段错误
    return (void*)ret12;
}  

void* task2(void* arg){
    string* ret22 = new string("world");
    // pthread_exit的作用与return相同，都是用来返回线程的退出状态。
    // 如果 string ret22 = "world"; pthread_exit(&ret22); 会出现段错误
    pthread_exit((void*)ret22);
    return NULL;
}  


int main(){
    pthread_t tid1,tid2;
    
    pthread_create(&tid1,NULL,task1,NULL);
    pthread_create(&tid2,NULL,task2,NULL);

    void* ret1;
    void* ret2;
    pthread_join(tid1,&ret1);
    pthread_join(tid2,&ret2);
    
    string* str1 = (string*)ret1;
    string* str2 = (string*)ret2;
    cout<<*str1<<endl;
    cout<<*str2<<endl;

    
    return 0;
}