#include<iostream>
#include<stdio.h>

using namespace std;

void myfun(){
    static int count=0;
    for(int i=0;i<10;i++){
        count++;
        printf("count: %d\n",count);
    }
}


int main(){
     int i=1;
     while(i<10){
        i++;
        myfun();
     }
}
