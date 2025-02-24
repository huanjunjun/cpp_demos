#include<iostream>
#include<thread>
#include<mutex>
using namespace std ;

class hungrySingleton{
    private:
        hungrySingleton(){
            cout<<"hungrySingleton::hungrySingleton()"<<endl;
        }
        hungrySingleton(const hungrySingleton&)=delete;
        hungrySingleton& operator=(const hungrySingleton&)=delete;
        hungrySingleton(hungrySingleton&&)=delete;
        ~hungrySingleton(){
            cout<<"hungrySingleton::~hungrySingleton()"<<endl;
        }
        static hungrySingleton instance;
        
    public:


        // static hungrySingleton get();
        // 这个定义依赖拷贝构造函数
        // 当函数返回一个对象时，实际上会创建一个该对象的副本并返回。在 hungrySingleton::get() 函数中，return instance; 这行代码会触发拷贝构造函数，因为它需要创建一个 instance 的副本并返回给调用者。

        static hungrySingleton& get();

        
};


// ！！！！！！！！！！！！！！！！！！！！！！！！！！！！此处注意！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
// hungrySingleton hungrySingleton::get(){
//     return instance;
// }
// 这个定义依赖拷贝构造函数
// 当函数返回一个对象时，实际上会创建一个该对象的副本并返回。在 hungrySingleton::get() 函数中，return instance; 这行代码会触发拷贝构造函数，因为它需要创建一个 instance 的副本并返回给调用者。

hungrySingleton& hungrySingleton::get(){
    return instance;
}
hungrySingleton hungrySingleton::instance;//初始化


int main(){
    thread t1[10];
    for(int i=0;i<10;i++){
        t1[i] = thread([](){
            hungrySingleton& s =  hungrySingleton::get();
            cout<<&s<<endl;
        });
    }

    for(int i=0;i<10;i++){
        t1[i].join();
    }
    return 0;
}