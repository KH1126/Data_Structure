#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct key_value {
    char *key;
    char *value;
    struct key_value *next;
};
typedef struct key_value KV;
typedef KV *link;

link deletenode(link head, link ptr);
link createlink(link head, char *key, char *value);
void updateNodeValue(link node, char *newValue);
link findnode(link head, char *key);
void freelist(link head);