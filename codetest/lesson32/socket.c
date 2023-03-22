/*
    #include <sys/types.h>          
    #include <sys/socket.h>

    int socket(int domain, int type, int protocol);
        功能：创建一个套接字
        参数：
            -domain：
                AF_UNIX
                AF_LOCAL
                AF_INET
                AF_INET6
            -type：通信过程中使用的协议类型
                SOCK_STREAM：流式协议
                SOCK_DGRAM：报式协议
            -protocol：具体的一个协议，一般写0
                -SOCK_STREAM：一般默认TCP
                -SOCK_DGRAM：一般默认UDP
            -返回值：
                成功：返回文件描述符，操作的就是内核缓冲区
                失败：-1
    int bind(int sockfd, const struct sockaddr *addr,
        socklen_t addrlen); //socket命名
        功能：绑定，将fd和对应的ip+端口进行绑定
        参数：
            sockfd：操作的文件描述符
            addr：需要绑定的socket地址，其中封装了ip和端口号信息
            addrlen：第二个参数结构体占的内存大小
    int listen(int sockfd, int backlog);
        功能：监听这个socket上的连接
        参数：
            sockfd：文件描述符
            backlog：连接的最大值（包括已连接和未连接的）
    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
        功能：接收客户端连接，默认阻塞，阻塞等待客户端连接
        参数：
            sockfd：用于监听的文件描述符
            addr：传出参数：记录了连接成功之后客户端的地址信息（ip，port）
            addrlen：指定第二个参数的对应的内存大小
        返回值：
            成功：用于通信的文件描述符
            -1：失败
    int connect(int sockfd, const struct sockaddr *addr,
            socklen_t addrlen);
        功能：客户端连接服务器
        参数：
            sockfd：用于通信的文件描述符
            addr：客户端要链接的服务器的地址信息
            adrlen：第二个参数的内存大小
        返回值：成功0，失败-1
*/

