#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
void *thread_run(void *arg)
{
    while(1)
    {
        printf("new pthread,thread is : %u,pid :%d\n",pthread_self(),getpid());
        sleep(1);
    }
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,thread_run,NULL);
    while(1)
    {
        printf("main pthread,thread is : %u,pid %d\n",pthread_self(),getpid());
        sleep(3);
    }
    return 0;
}
