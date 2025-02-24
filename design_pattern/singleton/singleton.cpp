#include<iostream>
#include<vector>
#include<algorithm>
#include<thread>
#include<mutex>
#include<condition_variable>
using namespace std;

/*
单例模式：
饿汉模式的 instance是一个静态变量，在程序启动时就已经创建好了。
懒汉模式的 instance是一个指针变量，在第一次调用 getInstance方法时才会创建对象。

饿汉模式的 instance是线程安全的，因为它在程序启动时就已经创建好了，所以不会出现多个线程同时创建对象的情况。
懒汉模式的 instance不是线程安全的，因为它在第一次调用 getInstance方法时才会创建对象，
如果有多个线程同时调用 getInstance方法，就可能会创建多个对象。
*/

class Singleton{
    private:
    // 私有化构造函数，不允许外部创建对象
        Singleton(){
            printf("Singleton::Singleton()\n");
        };
    // 私有化析构函数，不允许外部销毁对象
        ~Singleton(){
            printf("Singleton::~Singleton()\n");
        };

    // 删除拷贝构造函数，不允许外部拷贝对象
        Singleton(const Singleton&)= delete;
    // 删除赋值运算符，不允许外部赋值对象
        Singleton& operator=(const Singleton&) = delete;
    // 删除移动构造函数，不允许外部移动对象
        Singleton(Singleton&&) = delete;

    // 私有化静态指针变量，指向唯一的对象
        static Singleton instance;
        static mutex mtx;

    public:
            // 公有化静态方法，返回唯一的对象
        static Singleton& getInstance(){
            return instance;
        }

};


// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!本次书写忘了将静态变量在类外初始化，导致编译错误!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 规定：在 C++ 中，静态成员变量需要在类外进行定义和初始化。
// 定义并初始化静态成员变量
Singleton Singleton::instance;//此处定义会默认调用instance的构造函数，在main函数之前执行，所以是线程安全的
mutex Singleton::mtx;

// 对于懒汉模式而言 需要将instance 声明为 Singleton* 指针类型，并在类外进行初始化为nullptr
// Singleton* Singleton::instance = nullptr;

void* create_singleton(void * args){

    Singleton &s=Singleton::getInstance();
    // Singleton s1=s;
    cout<<&s<<endl;
    return nullptr;
}

int main(){
    printf("main start\n");
    pthread_t tid[10];
    for(int i=0;i<10;i++){
        pthread_create(&tid[i],nullptr,create_singleton,nullptr);
    }
    for(int i=0;i<10;i++){
        pthread_join(tid[i],nullptr);
    }
    return 0;
}