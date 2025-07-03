#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//define sorted set linked list
struct sset
{
    int index;
    char *key;
    struct sset *prev;
    struct sset *next;
};
typedef struct sset SSET;
typedef SSET *SSNODE; 
//define sorted set name
struct sorted
{
    char *sorted_name;
    SSNODE head;
    struct sorted *next;
    struct sorted *prev;
};
typedef struct sorted SORTED;
typedef SORTED *SNODE;
//Find key's address
SSNODE findsort(const SSNODE  head,char *key);
SSNODE deletesort(const SSNODE head, char *key);
SSNODE createsort(SSNODE head,int index,char *key);
SNODE findname(const SNODE head, const char *name);
SNODE deletename(const SNODE head,char *name);
SNODE createname(SNODE head,char *name);
SNODE ZADD(SNODE SHEAD_1);
void ZCARD(const SNODE const head);
void ZCOUNT(const SNODE const head);
void ZINTERSTORE(const SNODE const head);
void intersection(SNODE head,SSNODE head1,SSNODE head2,int power1,int power2);
void ZRANGEBYSCORE(SNODE head);
void ZUNIONSTORE(SNODE head);
void ZRANGE(SNODE head);
void ZRANK(SNODE head);
void ZREM(SNODE head);
void ZREMRANGEBYSCORE(SNODE head);