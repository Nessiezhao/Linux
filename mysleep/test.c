#include <stdio.h>
#include<signal.h>
#include<unistd.h>
void sig_alarm(int signo)
{
    (void)signo;
}
unsigned int mysleep(unsigned int n)
{
    struct sigaction New,Old;
    unsigned int unslept = 0;
    New.sa_handler = sig_alarm;
    sigemptyset(&New.sa_mask);
    New.sa_flags = 0;
    sigaction(SIGALRM,&New,&Old);//注册信号处理函数
    alarm(n);//设置闹钟
    pause();
    unslept = alarm(0);//清空闹钟
    sigaction(SIGALRM,&Old,NULL);//恢复默认信号处理动作
    return unslept;
}
int main()
{
    while(1)
    {
        mysleep(3);
        printf("hello world!\n");
    }
    return 0;
}
