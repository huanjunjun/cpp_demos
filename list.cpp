#include<iostream>
#include<list>

int main(){
    std::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    l.push_back(4);
    l.push_back(5);
    l.push_back(6);
    l.push_back(7);
    l.push_back(8);

    for(const auto& i : l){
        std::cout << i << " ";
    }
    std::cout<<std::endl;
    
    l.insert(l.begin(), 0);
    l.insert(l.end(), 9);

    for(const auto& i : l){
        std::cout << i <<" ";
    }
    std::cout<<std::endl;
    return 0;
}