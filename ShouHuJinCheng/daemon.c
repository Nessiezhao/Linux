#include <stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
void mydaemon(void)
{
    int i;
    int fd0;
    //调用fork，创建子进程，父进程退出（保证调用setsid的进程不是进程组的Leader）
    pid_t pid = fork();
    struct sigaction sa;

    umask(0);//调用umask将文件模式创建屏蔽字设置为0
    if(pid < 0)
    {
        perror("fork");
    }
    else if(pid > 0)
    {
        //father退出
        exit(0);
    }
    else
    {
        //child
        setsid();//调用setsid创建一个新会话
        sa.sa_handler = SIG_IGN;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        if(sigaction(SIGCHLD,&sa,NULL) < 0)
        {
            return;
        }
        if(chdir("/") < 0)
        {
            //将当前工作目录更改为根目录
            printf("child dir error!\n");
            return;
        }
        //关闭不再需要的文件描述符，或者重定向到/dev/null
        close(0);
        fd0 = open("/dev/null",O_RDWR);
        dup2(fd0,1);
        dup2(fd0,2);
    }
}
int main()
{
    mydaemon();
    while(1)
    {
        sleep(1);
    }
    return 0;
}
