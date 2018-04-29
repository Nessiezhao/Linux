#include <stdio.h>
#include<unistd.h>
int main()
{
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
        return 1;
    }
    else if(id == 0)
    {
        //child
        while(1)
        {
            printf("child %d is running...\n",getpid());
            sleep(1);
        }
    }
    else
    {
        //father
        int i = 10;
        while(i)
        {
            printf("parent %d is going to dead... i = %d\n",getpid(),i--);
            sleep(1);
        }
    }
    return 0;
}
