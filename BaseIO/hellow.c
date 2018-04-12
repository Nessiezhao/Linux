#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main()
{
    umask(0);
    int fd = open("myfile",O_WRONLY|O_CREAT,0644);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }
    int count = 5;
    const char* msg = "hello bit!\n";
    int len = strlen(msg);
    while(count--)
    {
        write(fd,msg,len);
        //msg：缓冲区首地址   len：本次读取，期望写入多少个字节的数据    返回值：实际写了多少字节数据
    }
    close(fd);
    return 0;
}
