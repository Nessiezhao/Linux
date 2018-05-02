#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#pragma warning(disable:4996)

typedef int LinkListType;
typedef struct LinkNode
{
    LinkListType data;
    struct LinkNode* next;
}LinkNode;

LinkNode* List = NULL;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

LinkNode* CreateNode(LinkListType value)
{
    LinkNode* new_node = (LinkNode*)malloc(sizeof(LinkNode));
    if(new_node == NULL)
    {
        perror("malloc");
        exit(1);
    }
    new_node->data = value;
    new_node->next = NULL;
    return new_node;
}
void PrintList(LinkNode* head)
{
    if(head == NULL)
    {
        return;
    }
    LinkNode* cur = head;
    while(cur != NULL)
    {
        printf("%d",cur->data);
        cur = cur->next;
    }
    printf("\n");
}
void PushFront(LinkNode** phead,LinkListType value)
{
    if(phead == NULL)
    {
        *phead = CreateNode(value);
        return;
    }
        LinkNode* new_node = CreateNode(value);
        new_node->next = (*phead);
        *phead = new_node;
}
void PopFront(LinkNode** phead,int* out)
{
    if(phead == NULL)
    {
        return;
    }
    if(*phead == NULL)
    {
        return;
    }
        LinkNode* to_erase = (*phead)->next;
        (*phead)->next = to_erase->next;
        *out = to_erase->data;
        free(to_erase);
}
void DestroyList(LinkNode** phead)
{
    if(phead == NULL)
    {
        return;
    }
    if(*phead == NULL)
    {
        return;
    }
    while(*phead != NULL)
    {
        LinkNode* cur = NULL;
        cur = (*phead)->next;
        free(*phead);
        *phead = cur;
    }
}
void *consume(void *arg)
{
    usleep(1000);
    int c = 0;
    while(1)
    {
        c = -1;
        pthread_mutex_lock(&lock);
        while(List->next == NULL)
        {
            printf("consumer begin waiting...\n");
            pthread_cond_wait(&cond,&lock);
        }
        PopFront(&List,&c);
        pthread_mutex_unlock(&lock);
        printf("consumer is done : %d\n",c);
    }
}
void *product(void* arg)
{
    sleep(1);
    while(1)
    {
        int num = rand()%1000+1;
        pthread_mutex_lock(&lock);
        PushFront(&List,num);
        
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond);
        printf("product is done : %d\n",num);
    }
}
int main()
{
    pthread_t c,p;
    pthread_create(&c,NULL,consume,NULL);
    pthread_create(&p,NULL,product,NULL);

    pthread_join(c,NULL);
    pthread_join(p,NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    DestroyList(&List);
    return 0;
}
