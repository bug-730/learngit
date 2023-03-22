/*
    #include <arpa/inet.h>

    int inet_pton(int af, const char *src, void *dst);
        参数：
            af:AF_INET表示ipv4
                AF_INET6表示ipv6
            src：要转换的点分十进制字符串
            dst：传出参数，转换后的结果保存在这里
    const char *inet_ntop(int af, const void *src,
                    char *dst, socklen_t size);
        参数：
            af:AF_INET表示ipv4
                AF_INET6表示ipv6
            src：要转换的ip整数的地址
            dst：传出参数，转换成IP地址字符串保存的地方
            size：第三个参数的大小（数组容量）
        返回值：和dst一样

*/
#include <stdio.h>
#include <arpa/inet.h>
int main(){

    //点分十进制IP地址
    char *str="192.168.1.100";
    
    //将点分十进制IP转换为网络字节序的整数
    
    unsigned int num = 0;
    inet_pton(AF_INET,str,&num);

    unsigned char *p=(char *)&num;
    printf("%d %d %d %d\n",*p,*(p+1),*(p+2),*(p+3));

    //将网络字节序的整数转换为点分十进制IP
    char buf[16];
    const char *ip = inet_ntop(AF_INET,&num,buf,sizeof(buf));
    printf("str :%s\n",str);
    printf("ip :%s\n",ip);
    return 0;
}