#pragma once

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>
#define PATH_NAME "/tmp"
#define PROJ_ID 0x6666

#define SEREVR_TYPE 1
#define CLIENT_TYPE 2

struct msgbuf
{
    long mtype;
    char mtext[128];
};

int createMsgQueue();
int getMsgQueue();
int sendMsg(int msgid, char * msg,int t)
int recvMsg(int msg ,int t,char* msg)
void destroyMsgQueue(int);

