/*
    大端字节序：高位存在低地址，低位存在高地址
    小端字节序：高位存在高地址，低位存在低地址

*/

#include <stdio.h>

int main(){
    union
    {
        /* data */
        int value;
        char bit[4];
    }test;
    test.value=0x01020304;
    if(test.bit[0]==1&&test.bit[1]==2&&test.bit[2]==3&&test.bit[3]==4)
    {
        printf("大端字节序\n");
    }
    else if(test.bit[0]==4&&test.bit[1]==3&&test.bit[2]==2&&test.bit[3]==1){
        printf("小端字节序\n");
    }else{
        printf("未知\n");
    }
    return 0;
}