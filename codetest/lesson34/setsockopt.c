/*
    根据TCP状态转换，当服务器进程断开后，会进入TIME_WAIT状态，该过程会等待2MSL时间
    （1分钟），此时如果重新启动服务器端程序，bind会失败，因为端口信息仍被保存，此时
    则需要端口复用
    #include <sys/types.h>          
    #include <sys/socket.h>
    //设置套接字属性，不仅仅是端口复用
    int setsockopt(int sockfd, int level, int optname,
                const void *optval, socklen_t optlen);
        参数：
            -sockfd：要操作的文件描述符
            -level：级别：SOL_SOCKET（端口复用的级别）
            -optname：选项的名称
                -SO_REUSEADDR
                -SO_REUSEPORT
            -optval:端口复用的值（整型）
                -1：可以复用
                -0：不可以复用
            -optlen：optval参数的大小
    端口复用设置的时机是在服务器绑定端口前


查看网络相关信息命令
netstat：
    -a 所有的socket
    -p 显示正在使用的socket
    -n 直接使用IP地址
*/