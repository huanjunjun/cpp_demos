#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
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
    int* p = (int*)arg;
    for(int i = 0; i < 10; i++){
        cout<<"task1: "<<p[i]<<endl;
        sleep(1);
    }
    return 0;
}

int main(){
    pthread_t tid;
    int ret ;
    int a[10]={1,2,3,4,5,6,7,8,9,10};

    ret = pthread_create(&tid, NULL, task1, a);
    if(ret){
        printf("Error in creating thread\n");
        return -1;
    }
    cout<<"创建成功 ret="<<ret<<endl;
    pause();
    return 0;

}