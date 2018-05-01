#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<pthread.h>
void *thread_run(void *arg)
{
    sleep(5);
    printf("new pthread,thread is : %u , pid : %d\n",pthread_self(),getpid());
    pthread_exit((void*)123);
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,thread_run,NULL);

    void *ret;
    pthread_join(tid,&ret);
    printf("join new thread success,ret :%d\n",*(int *)ret);
    return 0;
}
