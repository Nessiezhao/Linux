#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<strings.h>
#include<pthread.h>
#define MAX 1024
static void usage(const char* proc)
{
    printf("Usage:%s port\n",proc);//告诉客户端应该如何使用
}
static int startup(int port)
{
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock < 0)//创建失败
    {
        //服务器是以后台进行运行的，正常情况下这里不应该perror
        //而应该把错误信息打印到日志当中
        perror("socket");
        //创建套接字失败的话就不再继续向下运行
        exit(2);
    }
    //保证服务器断开连接的时候，不能让服务器因为time_wait而不能立即重启
    //所以要调用setsockopt
    int opt = 1;
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    //第一个参数要设置的套接字
    //第二个参数要设置的层数
    //第三个是要干什么，现在的情况是需要端口可以复用
    struct sockaddr_in local;//填充本地消息
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);//绑定本地任意ip
    local.sin_port = htons(port);
    if(bind(sock,(struct sockaddr*)&local,sizeof(local)) < 0 )//绑定失败
    {
        perror("bind");
        exit(3);
    }
    if(listen(sock,5) < 0)//监听
    {
        perror("listen");
        exit(4);
    }
    return sock;
}
//客户端可能发来的行分隔符有三种  \r  \n  \r\n
//统一按行'\n'来处理,一次读取一个字符
//按行获得文本，1.要从哪个套接字获得 2.获得之后要把获得的写到哪个缓冲区里 3.缓冲区多大
int get_line(int sock,char line[],int size)
    //从哪个套接字获得，将获得到的消息写到哪个缓冲区里，缓冲区多大
{
    int c = 'a';//只要初始化的值不是'\n'就可以
    int i = 0;
    ssize_t s = 0;
    while( i < size-1 && c != '\n')//因为最后一个应该放'\0'所以i要小于size-1
    {
        s = recv(sock,&c,1,0);//一次读1个
        if(s > 0)
        {
            if(c == '\r')//如果c是'\r'
            {
                // \r -> \n  or  \r\n -> \n
                recv(sock,&c,1,MSG_PEEK);//用MSG_PEEK进行窥探下一个元素
                if(c != '\n')//如果下一个字符不是'\n'说明此时的行分隔符只有'\r'
                {
                    c = '\n';//读进来的是'\r'，将'\r'转成'\n'
                }
                else//否则说明下一个字符是'\n'，但是'\n'还在缓冲区里，这次读不会被阻塞
                {
                    recv(sock,&c,1,0);
                }
            }
            //c == \n
            line[i++] = c;// \n 
        }
        else
        {
            break;
        }
    }
    line[i] = '\0';
    return i;//返回这一行有多少个字符
}
static void* handler_request(void* arg)
{
    int sock = (int)arg;
    char line[MAX];
    char method[MAX/32];//方法
    char url[MAX];//请求的资源
    int errCode;
    int cgi = 0;//cgi 通用网关接口，是Http服务内置的一种标准，方便后对Http的功能进行二次扩展
#if Debug
    do{
        get_line(sock,line,sizeof(line));
        printf("%s",line);
    }while(strcmp(line,"\n") != 0);
#else
    //首先要把第一行拿到
    if(get_line(sock,line,sizeof(line)) < 0)//获得行出错
    {
        errCode = 404;//错误码
        goto end;//第一行读取都有错误，http不能处理，直接不处理
    }
    //第一行获取成功
    //第一个提取方法字段
    int i = 0;
    int j = 0;
    while(i < sizeof(method) - 1 && j < sizeof(line) && !isspace(line[j]))
    {
        method[i] = line[j];
        i++;
        j++;
    }
    method[i] = '\0';
    if(strcasecmp(method,"GET") && strcasecmp(method,"POST"))
    {
        errCode = 404;
        goto end;
    }
    while(j < sizeof(line) && isspace(line[j]))
    {
        j++;
    }
    i = 0;
    while(i < sizeof(url)-1 && j < sizeof(line) && !isspace(line[j]))
    {
        url[i] = line[j];
        i++;
        j++;
    }
    url[i] = '\0';
    //方法要么是GET要么是POST，而且资源已经拿到
#endif
end:
    close(sock);//作用：1.回收了本地描述符资源   2.关闭了连接
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
        //从listen_sock中获取新连接，获得新连接放到client中
        if(new_sock < 0)
        {
            perror("accept");
            continue;//继续再获取新连接
        }
        //获取连接成功
        pthread_t id;
        pthread_create(&id,NULL,handler_request,(void*)new_sock);//创建线程
        //线程属性NULL，创建这个线程为了提供服务
        //(我们的浏览器在向我发起http请求之前要先建立连接，一旦建立连接
        //那么服务器就获得连接并创建新线程，新线程剩下的工作就是处理请求)
        //new_sock : 处理的是哪一个连接(直接传值的时候可能会遇到一些问题
        //强转成void* 以只拷贝的形式传过去)
        pthread_detach(id);//分离，继续获取新连接
    }
}
