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

struct array_process_unit{
    int* array;
    int begin;
    int end;
};

/**
 * 是的，使用结构体是为pthread_create传递多个参数的最佳方式之一。
 * 通过结构体，你可以将多个参数打包成一个单一的指针传递给线程函数，
 * 这样可以避免参数传递的限制，并且使代码更加清晰和易于维护。
 */

void* process(void* arg){

    array_process_unit* a = (array_process_unit*)arg;
    int begin = a->begin;
    int end = a->end;
    int *array = a->array;
    for(int i = begin; i < end; i++){
        array[i] = array[i] * 10;
    }

}


int main(){
    int a[1000];
    for(int i = 0; i < 1000; i++){
        a[i] = i;
    }
    pthread_t tid[10];
    int ret[10];    
    for(int i=0;i<10;i++){

        ret[i] = pthread_create(&tid[i], NULL,  process, new array_process_unit{a, i*100, (i+1)*100});
        if(ret[i] != 0){ 
            cout << "pthread_create error: error_code=" << ret[i] << endl;
        }
    }

    for(int i=0;i<10;i++){
        pthread_join(tid[i], NULL);
    }

    for(int i=0 ;i<1000;i++){
        cout<<a[i]<<" ";
    }

    
    return 0;
}