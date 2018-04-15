#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

int main()
{
    close(0);
    close(2);
    int fd = open("file",O_RDONLY,0644);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }
    int fd1 = open("file1",O_RDONLY,0644);
    int fd2 = open("file2",O_RDONLY,0644);
    int fd3 = open("file3",O_RDONLY,0644);
    printf("fd : %d\n",fd);
    printf("fd : %d\n",fd1);
    printf("fd : %d\n",fd2);
    printf("fd : %d\n",fd3);
//    const char* msg = "hello world\n";
//    int size = strlen(msg);
//    char buf[64];
//    while(1)
//    {
//        size_t s = read(0,buf,size);
//        if(s > 0)
//        {
//            buf[s] = 0;
//        }
//        else if(s == 0)
//        {
//            break;
//        }
//        printf("%s",buf);
//    }
//    close(fd);
}
