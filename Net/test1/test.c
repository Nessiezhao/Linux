#include <stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>
int main()
{
    pthread_t tid1 = 0;
    struct sockaddr_in addr1;
    struct sockaddr_in addr2;
    addr1.sin_addr.s_addr = 0;
    addr2.sin_addr.s_addr = 0xffffffff;
    pthread_create(&tid1,NULL,Func1,&addr1);
    pthread_t tid2 = 0;
    pthread_create(&tid2,NULL,Func2,&addr2);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    return 0;
}
