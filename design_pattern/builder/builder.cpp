#include<iostream>
#include<thread>
#include<mutex>
using namespace std ;

class Product{
    private:
        string name;
        int price;
    public:
        Product(){}
        ~Product(){}
        Product(string name,int price):name(name),price(price){}
        Product(const Product& p){
            cout<<"invoke Product copy construction "<<endl;
            this->name = p.name;
            this->price = p.price;
        }
        void show(){
            cout<<"product name:"<<name<<endl;
            cout<<"product price:"<<price<<endl;
        }
        void setName(string name){
            cout<<"Product::setName()"<<endl;
            this->name = name;
        };
        void setPrice(int price){
            cout<<"Product::setPrice()"<<endl;
            this->price = price;
        }
};

class ProductBuilder{
    private:
        Product product;
    public:
        // 构造函数
        ProductBuilder(){
            cout<<"ProductBuilder::ProductBuilder()"<<endl;
            product =Product();
            cout<<"ProductBuilder::ProductBuilder() end"<<endl;
        }
        ProductBuilder(const ProductBuilder& pb){
            // cout<<"invoke ProductBuilder copy construction "<<endl;
            this->product = pb.product;
        }
        //！！！！！！！！！！！！！！！！！！！！！！！！！！返回自身的函数，定义为引用类型，避免拷贝构造函数的调用 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        ProductBuilder& setName(string name){
            cout<<"ProductBuilder::setName()"<<endl;
            product.setName(name);
            return *this;
        }
        ProductBuilder& setPrice(int price){
            cout<<"ProductBuilder::setPrice()"<<endl;
            product.setPrice(price);
            return *this;
        }
        Product build(){
            cout<<"ProductBuilder::build()"<<endl;
            return product;
        }
};
int main(){
    ProductBuilder pb = ProductBuilder();
    Product p = pb.setName("huanjun").setPrice(199).build();
    Product p2 = pb.setName("haha").setPrice(299).build();
    return 0;
}