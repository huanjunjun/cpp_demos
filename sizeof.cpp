#include<stdio.h>

 struct null_struct
{
    /* data */
};

 struct struct1
{
    /* data */
    char c;
    double d;
    int i;

};




int main(){
    printf("the bit length of int is %d\n",(int)sizeof(int));

    // 指针的大小是系统位宽，处理器位数
    printf("the bit length of the computer's address is %d\n",(int)sizeof(int*));
    printf("null_struct's size is %d\n",(int)sizeof(null_struct));
    printf("struct1's size is %d\n",(int)sizeof(struct1));
    return 0;
}