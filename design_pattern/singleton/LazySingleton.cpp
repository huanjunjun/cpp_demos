#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

// 懒汉模式: 构造函数私有化，禁用拷贝构造，禁用赋值函数，禁用移动构造，单例变量初始化为指针，使用double check locking
class LazySingleton{
    private:
        LazySingleton(){
            cout<<"LazySingleton::LazySingleton()"<<endl;
        }
        LazySingleton(const LazySingleton&)=delete;
        LazySingleton(LazySingleton&&)=delete;
        LazySingleton& operator=(const LazySingleton&)=delete;
        ~LazySingleton(){
            cout<<"LazySingleton::~LazySingleton()"<<endl;
        }
        static LazySingleton* instance;
        static mutex mtx;
    public:
        static LazySingleton* get();
};

LazySingleton* LazySingleton::instance = nullptr;
mutex LazySingleton::mtx;
LazySingleton* LazySingleton::get(){
    if(instance==nullptr){
        lock_guard<mutex> lock(mtx);
        if(instance==nullptr){
            // instance = new LazySingleton();
            // new三部走：分配空间、初始化、赋值，在编译器的优化下可能会变成：分配空间、赋值、初始化，有可能导致get函数返回空
            auto tmp = new LazySingleton();
            atomic_thread_fence(std::memory_order_acquire);
            instance = tmp;
        }

    }
    return instance;
}


int main(){
    thread t[10];
    for(int i=0;i<10;i++){
        t[i] = thread([](){
            LazySingleton* s = LazySingleton::get();
            cout<<s<<endl;
        });
    }
    for(int i=0;i<10;i++){
        t[i].join();
    }
    return 0;
}