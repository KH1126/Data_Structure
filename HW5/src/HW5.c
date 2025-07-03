#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HW5_function.h"
int main()
{
    SNODE SHEAD=NULL,ptr;
    char cmd[20];
    printf("Three motion to choose\n");
    printf("(LPUSH, RPUSH, LPOP, RPOP, LLEN, LRANGE)\n(SET, GET, DEL, UPD)\n(ZADD,ZCARD,ZCOUNT,ZINTERSTORE,ZUNIONSTORE,ZRANGE,ZRANGEBYSCORE,ZRANK,ZREM,ZREMRANGEBYSCORE)\n");
    while (1) {
        char key[20], value[20],name[20];
        printf("Enter the operation:");
        scanf("%s", cmd);
        if (strcmp(cmd, "ZADD") == 0) {
            SHEAD=ZADD(SHEAD);
        }
        else if(strcmp(cmd,"ZCARD")==0)
        {
            ZCARD(SHEAD);
        }
        else if(strcmp(cmd,"ZCOUNT")==0)
        {
            ZCOUNT(SHEAD);
        }
        else if(strcmp(cmd, "ZINTERSTORE")==0)
        {
            ZINTERSTORE(SHEAD);
        }
        else if(strcmp(cmd, "ZUNIONSTORE")==0)
        {
           ZUNIONSTORE(SHEAD);
        }
        else if (strcmp(cmd, "ZRANGEBYSCORE")==0)
        {
            ZRANGEBYSCORE(SHEAD);
        }
        else if (strcmp(cmd, "ZRANGE")==0)
        {
            ZRANGE(SHEAD);
        }
        else if(strcmp(cmd, "ZRANK")==0)
        {
            ZRANK(SHEAD);
        }
        else if(strcmp(cmd,"ZREM")==0)
        {
            ZREM(SHEAD);
        }
        else if(strcmp(cmd,"ZREMRANGEBYSCORE")==0)
        {
            ZREMRANGEBYSCORE(SHEAD);
        }
        else {
            printf("Input error\n");
        }
    }
    return 0;
}