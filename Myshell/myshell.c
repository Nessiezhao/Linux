#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
int main()
{
	char** arg = NULL;
	char cmd[128];
	char *my_arg[32];
	while(1)
	{
		printf("myshell#");
		fgets(cmd,sizeof(cmd),stdin);

		pid_t id = fork();
		if(id == 0)
		{
			//child
			cmd[strlen(cmd)-1]=0;

			char *p = cmd;
			int i = 1;
			my_arg[0] = cmd;
			while(*p)
			{

				if(isspace(*p))
				{

					*p = 0;

					p++;
					my_arg[i++] = p;
				}
				else
				{
					p++;
	
				}
			}
			my_arg[i] = NULL;
			execvp(my_arg[0],my_arg);

		}
		else
		{
			//father
			int status = 0;
			pid_t ret = waitpid(id,&status,0);
			if(ret>0)
			{
				printf("sig:%d,exit code:%d\n",status&0x7F,(status>>8)&0xFF);
			}
			else
			{
				printf("waitpid running error!\n");
			}
		}
	}
	return 0;
}
