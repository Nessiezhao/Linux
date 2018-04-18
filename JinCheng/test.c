#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
	pid_t id = fork();
	if(id < 0)
	{
		perror("fork");
		return 1;
	}
	else if(id > 0 )
	{
		//father
		printf("father [%d] is sleeping...\n",getpid());
		sleep(30);
	}
	else
	{
		printf("child [%d] is begin Z...\n",getpid());
		sleep(5);
		exit(EXIT_SUCCESS);
	}
	return 0;
}
