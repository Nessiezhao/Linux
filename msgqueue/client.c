#include"comm.h"
int main()
{
    char buf[256];
    int msgid = createMsgQueue();
    while(1)
    {
        //send
        printf("Please Enter: ");
        scanf("%s",buf);
        sendMsg(msgid,buf,CLIENT_TYPE);
        if(strcmp(buf,"quit") == 0)
        {
            printf("client quit,me too!\n");
            break;
        }
        //recv
        recvMsg(msgid,SERVER_TYPE,buf);
        printf("server# %s\n",buf);
    }
    return 0;
}
