#include <stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define CONSUMERS_COUNT 2
#define PRODUCERS_COUNT 2

struct msg
{
    struct msg *next;
    int num;
};
struct msg *head = NULL;

pthread_cond_t cond;
pthread_mutex_t mutex;
pthread_t threads[CONSUMERS_COUNT+PRODUCERS_COUNT];

void *consumer(void *p)
{
    int num = *(int*)p;
    free(p);
    struct msg *mp;
    for(;;)
    {
        pthread_mutex_lock(&mutex);
        while(head == NULL)
        {
            printf("%d begin wait a condition...\n",num);
            pthread_cond_wait(&cond,&mutex);
        }
        printf("%d end wait a condition...\n",num);
        printf("%d begin consume product...\n",num);
        mp = head;
        head = mp->next;
        pthread_mutex_unlock(&mutex);
        printf("Consume %d\n",mp->num);
        free(mp);
        printf("%d end consume product...\n",num);
        sleep(rand()%5);
    }
}

void *producer(void *p)
{
    struct msg *mp;
    int num = *(int*)p;
    free(p);
    for(;;)
    {
        printf("%d begin produce product...\n",num);
        mp = (struct msg*)malloc(sizeof(struct msg));
        mp->num = rand()%100+1;
        printf("produce %d\n",mp->num);
        pthread_mutex_lock(&mutex);
        mp->next = head;
        head = mp;
        printf("%d end produce product...\n",num);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(rand()%5);
    }
}
int main()
{
    srand(time(NULL));
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);

    int i = 0;
    for(;i < CONSUMERS_COUNT;i++)
    {
        int *p = (int*)malloc(sizeof(int));
        *p = i;
        pthread_create(&threads[i],NULL,consumer,(void*)p);
    }
    for(i = 0;i < PRODUCERS_COUNT;i++)
    {
        int *p = (int*)malloc(sizeof(int));
        *p = i;
        pthread_create(&threads[i+CONSUMERS_COUNT],NULL,producer,(void*)p);
    }
    for(i = 0;i < CONSUMERS_COUNT+PRODUCERS_COUNT;i++)
    {
        pthread_join(threads[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}
