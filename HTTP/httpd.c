#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

static void usage(const char* proc)
{
    printf("Usage:%s port\n",proc);//告诉客户端应该如何使用
}
static int startup(int port)
{
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)//创建失败
    {
        perror("socket");
        exit(2);
    }
    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //第一个参数要设置的套接字
    //第二个参数要设置的层数
    //第三个是要干什么，现在的情况是需要端口可以复用
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
static void* handler_request(void* arg)
{
    int sock = (int)arg;
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
    //监听套接字有了就进行事件处理
    for(;;)
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int new_sock = accept(listen_sock,(struct sockaddr*)&client,&len);//获取连接
        if(new_sock < 0)
        {
            perror("accept");
            continue;
        }
        pthread_t id;
        pthread_create(&id,NULL,handler_request,(void *)new_sock);
        pthread_detach(id);
    }
}
