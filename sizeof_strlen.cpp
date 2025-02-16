#include<iostream>
#include<cstring>

int main(){
    /**
     * sizeof是一个关键字
     * 而strlen是一个 cstring库 中的函数
     */
    char* c_str = "hello huanjun!";
    std::cout<<strlen(c_str)<<std::endl;
    std::cout<<sizeof c_str <<std::endl;

    char c_str2[] = "hello huanjun!";
    std::cout<<strlen(c_str2)<<std::endl;
    std::cout<<sizeof c_str2 <<std::endl;
    return 0;
}