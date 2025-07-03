#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TWELVE 12
#define MILLION 1000000
struct key_value {
    char *key;
    char *value;
    struct key_value *next,* prev;
};
typedef struct key_value KV;
typedef KV *link;
void dbUPD(link const head,char *key,char *value);
void dbGET(link const head,char* key);
link dbDEL(link head,char * key);
void dbinsert(link head,char *key,char * value);
link deletenode(link head, link ptr);
void createlink(link head, char *key, char *value);
void updateNodeValue(link node, char *newValue);
link findnode(link const head, char *key);
void freelist(link head);
link rfindnode(link const head, char *key);
link lfindnode(link const head, char *key);