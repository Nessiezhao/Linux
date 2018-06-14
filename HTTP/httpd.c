#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

static void usage(const char* proc)
{
    printf("Usage:%s port\n",proc);//告诉客户端应该如何使用
}
int startup(int port)
{
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)//创建失败
    {
        perror("socket");
        exit(2);
    }
    struct sockaddr_in local;//填充本地消息
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(port);
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0 )
    {
        perror("bind");
        exit(3);
    }
    if(listen(sock,5) < 0)
    {
        perror("listen");
        exit(4);
    }
    return sock;
}
int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        usage(argv[0]);
        return 1;
    }
    //现在的Http服务器底层是基于Tcp的，所以第一件事一定要有一个listen_sock
    int listen_sock = startup(atoi(argv[1]));//调用start来获得监听套接字
}
