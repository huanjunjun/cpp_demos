#include<iostream>

using namespace std;



class car{
    private:
    public:
        car(){}
        ~car(){}
        virtual void drive()=0;
        virtual void stop()=0;
};

// 此处要明确指定为public继承，否则会报错，没有指定继承方式，默认是私有继承
// 这会导致基类的成员在派生类中变为私有成员，从派生类对象无法隐式转换为基类对象，从而产生编译错误。
class  benz: public car{
    private:
    public:
        benz(){}
        ~benz(){}
        void drive(){
            cout<<"benz drive"<<endl;
        }
        void stop(){
            cout<<"benz stop"<<endl;
        }
};


class bmw: public car{
    private:
    public:
        bmw(){}
        ~bmw(){}
        void drive(){
            cout<<"bmw drive"<<endl;
        }
        void stop(){
            cout<<"bmw stop"<<endl;
        }
};


class carFactory{
    private:
    public:
        carFactory(){}
        ~carFactory(){}

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!这个地方要返回car* 指针类型，因为 founction returning an abstract class or an interface is not allowed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // 也可以返回智能指针
        car* produce(string type){
            if(type=="benz"){
                return new benz();
            }
            if (type=="bmw")
            {
                /* code */
                return new bmw();
            }
            return new benz();
            
        }

};

int main(){
    carFactory factory;
    car* c = factory.produce("benz");
    c->drive();

    car* c2 = factory.produce("bmw");
    c2->drive();
}