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

pthread_mutex_t mutex;
pthread_cond_t condition;
int count;

void* task1(void* arg){
    while(1){
        pthread_mutex_lock(&mutex);
        while(count < 10){
            pthread_cond_wait(&condition,&mutex);
        }
        cout<<"task2:  解除阻塞"<<endl;
        count-=10;
        pthread_mutex_unlock(&mutex);
    }
 
    return 0;
}  

void* task2(void* arg){
    
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 100000; j++);
        pthread_mutex_lock(&mutex);
        count++;
        cout<<"task2:  count:"<<count<<endl;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&condition);

    }
    
    return 0;
}  



int main(){
    pthread_t tid1;
    pthread_t tid2;
    
    
    
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&condition,NULL);

    pthread_create(&tid1,NULL,task1,NULL);
    pthread_create(&tid2,NULL,task2,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condition);
    return 0;
}