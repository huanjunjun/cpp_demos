#include<iostream>
#include<stdio.h>
#include<pthread.h>
#include <unistd.h>
#include<chrono>
#include<thread>
using namespace std;

// 使用条件变量完成

class mydata{
    private:
        int index;
        int capicity;
        int* data;
    public:
        pthread_mutex_t mutex1;
        pthread_cond_t cond1;
        pthread_cond_t cond2;

        mydata(int n):index(0),capicity(n){
            data = new int[n];
            pthread_mutex_init(&mutex1,NULL);
            pthread_cond_init(&cond1,NULL);
            pthread_cond_init(&cond2,NULL);
        }
        ~mydata(){
            delete[] data;
            pthread_cond_destroy(&cond1);
            pthread_mutex_destroy(&mutex1);
        }
        
        void consume(void* args){
            while(1){

                std::this_thread::sleep_for(
                    std::chrono::milliseconds(100)
                );
                pthread_mutex_lock(&mutex1);
                while(index<=0){
                    cout<<"消费者等待"<<endl;
                    pthread_cond_wait(&cond1,&mutex1);
                }

                cout<<"current index is:"<<index<<endl;
                index--;
                cout<<"consume one then index is:"<<index<<endl;

                pthread_cond_signal(&cond2);
                pthread_mutex_unlock(&mutex1);

            }
            return ;
        }

        void produce(void* args){
            while(1){
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(10)
                );
                
                pthread_mutex_lock(&mutex1);

                while(index>=capicity){
                    cout<<"生产者等待"<<endl;
                    pthread_cond_wait(&cond2,&mutex1);
                }
                cout<<"current index is:"<<index<<endl;
                index++;
                cout<<"produce one then index is:"<<index<<endl;

                pthread_cond_broadcast(&cond1);
                pthread_mutex_unlock(&mutex1);
                // pthread_cond_signal(&cond1);

            }
            return ;
        }

};

void* produce(void* args){
    mydata* data = (mydata*)args;
    data->produce(NULL);
    return NULL;
}

void* consume(void* args){
    mydata* data = (mydata*)args;
    data->consume(NULL);
    return NULL;
}


int main(){
    pthread_t tid1,tid2;

    mydata data(10);
    pthread_create(&tid1,NULL,produce,&data);
    
    pthread_create(&tid2,NULL,consume,&data);
    pthread_create(&tid2,NULL,consume,&data);
    pthread_create(&tid2,NULL,consume,&data);
    pthread_create(&tid2,NULL,consume,&data);
    pthread_create(&tid2,NULL,consume,&data);
    pthread_create(&tid2,NULL,consume,&data);
    pthread_create(&tid2,NULL,consume,&data);
    pthread_create(&tid2,NULL,consume,&data);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 1;


}