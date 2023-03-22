/*
    网络通信时，需要将数据转换为网络字节序（大端），
    接收端接收后根据自身情况将网络字节序转换为主机字节序
    #include <arpa/inet.h>

    uint16_t ntohs(uint16_t netshort);//转换端口（2字节）
    uint16_t htons(uint16_t hostshort);

    uint32_t ntohl(uint32_t netlong);//转换ip（4字节）
    uint32_t htonl(uint32_t hostlong);

    h --host主机字节序
    n --net网络字节序

*/

#include <stdio.h>
#include <arpa/inet.h>

int main(){
    //htons
    unsigned short a=0x0102;
    printf("a:%x\n",a);
    unsigned short b=htons(a);
    printf("b:%x\n",b);
    printf("============================>\n");
    
    //htonl 转换IP
    char buf[4]={192,168,1,100};
    int num=*(int *)buf;
    int sum = htonl(num);
    unsigned char *p=(char *)&sum;
    printf("%d %d %d %d\n",*p,*(p+1),*(p+2),*(p+3));
    return 0;
}