#include "myredis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hiredis/hiredis.h"
#include "malloc.h"
#define MILLION 1000000
// 1 million data
char data[MILLION][5];
void generate_data();
char randoml(int amount);
void benchmark_insert(link head,redisContext *c,double *time,int *spaces);
void benchmark_get(link head, redisContext *c, double *time, int *spaces);
int main()
{
    double time[4];
    int space[2];
    char  key[8];
    link head, ptr;
    head = (link)malloc(sizeof(KV));
    head->next = NULL;
    ptr = head;
    // produce data
    generate_data();
    // Connrect to redis server
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err)
    {
        if (c) 
        {
            printf("Error: %s\n", c->errstr);
        // handle error
        } 
        else printf("Can't allocate redis context\n");
    }
    // Insert data into linked list and redis
    benchmark_insert(head,c,time,space);
    // Print memory and time for linked list and redis
    while(1)
    {
        scanf("%s", key);
        ptr = findnode(head, key);
        if (ptr != NULL) 
        {
            printf("value equal %s\n", ptr->value);
        } 
        else
        {
            printf("Didn't find key\n");
        }

    }
    return 0;
}

void generate_data()
{
    for (int i=0;i<5;i++)
    {
        for (int j=0;j<62;j++)
        {
            for (int k=0;k<62;k++)
            {
                for (int l=0;l<62;l++)
                {
                    if(i*62*62*62+j*62*62+k*62+l<MILLION)
                    {
                        data[i*62*62*62+j*62*62+k*62+l][0]=randoml(i);
                        data[i*62*62*62+j*62*62+k*62+l][1]=randoml(j);
                        data[i*62*62*62+j*62*62+k*62+l][2]=randoml(k);
                        data[i*62*62*62+j*62*62+k*62+l][3]=randoml(l);
                        data[i*62*62*62+j*62*62+k*62+l][4]='\0';
                    }
                    else break;
                }
            }
        }
    }
}

// Random 0-9, A-Z, a-z
char randoml(int amount)
{
    amount=amount%62;
    if(amount<10) return ('0'+amount);
    else if(amount<36) return ('a' + amount - 10);
    else return ('A' + amount - 36);
}
//insert function
void benchmark_insert(link head,redisContext *c,double *time,int *spaces)
{
    int i;
    link ptr1=head;
    char  key[8];
    clock_t start,finish;
    spaces[0]=-mallinfo2().uordblks;
    start=clock();
    for (i=0;i<MILLION;i++)
    {
        sprintf(key,"%d",i);
        createlink(ptr1,key,*(data+i));
    }
    // test insert time
    finish=clock();
    spaces[0]+=mallinfo2().uordblks;
    time[0]=(double)(finish-start)/CLOCKS_PER_SEC;
    start=clock();
    for (i=0;i<MILLION;i++)
    {
        sprintf(key,"%d",i);
        ptr1=findnode(head, key);
        // every 10000 times print the value
        if(i%10000==0) 
        {
            printf("%d\n",i);
            printf("value equal %s\n", ptr1->value);
        }
    }
    finish=clock();
    printf("value equal %s\n", ptr1->value);
    time[2]=(double)(finish-start)/CLOCKS_PER_SEC;
    i=MILLION;
    // mysql data
    printf("Benchmarks for myredis:\n");
    printf("Total time for inserting %d data need %.6fs\n",i,time[0]);
    printf("Avergae time for inserting %d data need %.6fus\n",i,(double)time[0]/i*MILLION);
    printf("Total memory for inserting %d data need %dbytes\n",i,spaces[0]);
    printf("Avergae memory for inserting %d data need %dbytes\n",i,spaces[0]/i);
    printf("Total time for getting %d data need %.6fs\n",i,time[2]);
    printf("Avergae time for getting %d data need %.6fms\n\n",i,(double)time[2]/i*1000);

    start=clock();
    for (i=0;i<MILLION;i++)
    {
        sprintf(key,"%d",i);
        redisCommand(c, "SET %s %s", key, *(data+i));
    }
    finish=clock();
    time[1]=(double)(finish-start)/CLOCKS_PER_SEC;
    printf("Benchmarks for hiredis:\n");
    printf("Total time for inserting %d data need %.6fs\n",i,time[1]);
    printf("Avergae time for inserting %d one data need %.6fus\n",i,(double)time[1]/i*MILLION);
    benchmark_get(head,c, time,spaces);
    //REDIS GET memory
    redisReply* info_reply=redisCommand (c,"INFO memory");
    char* info_str=info_reply->str;
    sscanf(info_str,"# Memory\nused_memory:%u\n",&spaces[1]);
    printf("Total memory for inserting %d datas need %dbytes\n",i,spaces[1]);
    printf("Avergae memory for inserting %d one data need %dbytes\n\n",i,spaces[1]/i);
}
void benchmark_get(link head, redisContext *c, double *time, int *spaces) {
    int i;
    link ptr1 = head;
    char key[8];
    clock_t start, finish;
    start = clock();
    for (i = 0; i < MILLION; i++) {
        sprintf(key, "%d", i);
        redisReply *reply = redisCommand(c, "GET %s", key);
        if (reply) {
            freeReplyObject(reply);
        }
    }
    finish = clock();
    time[3] = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("Total time for getting %d data need %.6fs\n", i, time[3]);
    printf("Average time for getting %d one data need %.6fus\n", i, (double)time[3] / i * MILLION);
}