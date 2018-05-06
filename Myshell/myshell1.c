#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

void ParseArg(char* input,char* output[])
{
    //按照空格进行字符串切分
    char* p = strtok(input," ");
    int output_size = 0;
    while(p != NULL)
    {
        output[output_size++] = p;
        p = strtok(NULL," ");
    }
    output[output_size] = NULL;
    return;
}
void Exec(char* argv[])
{
    int ret = fork();
    if(ret > 0)
    {
        //father
        wait(NULL);
    }
    else if(ret == 0)
    {
        //child
        execvp(argv[0],argv);
        perror(argv[0]);
        exit(0);
    }
    else
    {
        perror("fork");
    }
    return;
}
int main()
{
    while(1)
    {
        printf("[myshell] ");
        fflush(stdout);
        //1.从标准输入读一行字符串
        char buf[1024] = {0};
        //'gets' is deprecated
        gets(buf);
        //2.解析字符串，解析出指令和参数
        //ls   -l     -t
        char* argv[100] = {NULL};
        ParseArg(buf,argv);
        //3.创建子进程，子进程进行程序替换，父进程进行等待
        Exec(argv);
    }
    return 0;
}
