#include<iostream>
#include<vector>
using namespace std;


class client{
    public:
        int a;
        int b;
        int c;
        int* data;

        client(int a,int b,int c,int data_value):a(a),b(b),c(c){
            cout<<"invoke constructor"<<endl;
            data = new int[10000];
            for(int i = 0; i < 10000; i++){
                data[i] = data_value;
            }
        }

        /**
         * @brief 拷贝构造函数，用于创建一个新对象，该对象是另一个对象的副本。
         * 
         * 该函数接受一个常量左值引用作为参数，将传入对象的成员变量复制到新对象中。
         * 
         * @param other 常量左值引用，表示要复制的对象。
         */
        client(const client& other){
            // 输出日志，表明拷贝构造函数被调用
            cout<<"invoke copy constructor"<<endl;
            // 复制传入对象的成员变量 a
            a = other.a;
            // 复制传入对象的成员变量 b
            b = other.b;
            // 复制传入对象的成员变量 c
            c = other.c;
            // 为新对象分配内存，复制 data 数组
            data = new int[10000];
            for(int i = 0; i < 10000; i++){
                data[i] = other.data[i];
            }
        }
        /**
         * @brief 移动构造函数，用于将一个临时对象的资源转移到新对象中。
         * 
         * 该函数接受一个右值引用作为参数，将传入对象的资源所有权转移到新对象，
         * 并将传入对象的指针置为nullptr，避免资源的重复释放。
         * 
         * @param other 右值引用，表示要转移资源的临时对象。
         */
        client(client&& other){
            // 输出日志，表明移动构造函数被调用
            cout<<"invoke move constructor"<<endl;
            // 复制传入对象的成员变量 a
            a = other.a;
            // 复制传入对象的成员变量 b
            b = other.b;
            // 复制传入对象的成员变量 c
            c = other.c;
            // 将传入对象的 data 指针所指向的资源转移到新对象
            data = other.data;
            // 将传入对象的 data 指针置为 nullptr，避免资源重复释放
            other.data = nullptr;
        }
    private:

};

int main(){
    client c1(1,2,3,1);
    client&& c2 = move(c1);
    cout<<"c1 address:"<<&c1<<endl;
    cout<<c1.data[0]<<endl;

    cout<<"c2 address:"<<&c2<<endl;
    cout<<c2.data[0]<<endl;

    return 0;

}