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
void dbinsert(link  head,char *key,char * value);
link deletenode(link head, link ptr);
void createlink(link head, char *key, char *value);
void updateNodeValue(link node, char *newValue);
link findnode(link const head, char *key);
void freelist(link head);
link rfindnode(link const head, char *key);
link lfindnode(link const head, char *key);
void dbGET(link head,char* key)
{
    link ptr;
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
link dbDEL(link head,char * key)
{
    link ptr = findnode(head, key);
    if (ptr != NULL) 
    {
        head = deletenode(head, ptr);
        free(ptr);
    } 
    else 
    {
        printf("Didn't find key\n");
    }
    return head;
}
void dbinsert(link head,char *key,char * value)
{
    createlink(head, key, value);
}
void dbUPD(link const head,char *key,char *value)
{
    link ptr = findnode(head, key);
    if (ptr != NULL) {
        updateNodeValue(ptr, value);
    } 
    else 
    {
        printf("Didn't find key\n");
    }
}

//delete data function, need initial address and findnode return value
link deletenode(link head, link ptr) {
    link previous = head;
    if (ptr == head) {
        head = head->next;
    } 
    else {
        while (previous->next != ptr) {
            previous = previous->next;
        }
        previous->next = ptr->next;
    }
    return head;
}
//create data, need initial address and string of key and value.
void createlink(link head, char *key, char *value) {
    link ptr1 = (link)malloc(sizeof(KV));
    ptr1->key = strdup(key);
    ptr1->value = strdup(value);
    ptr1->next = head->next;  // Link the new node to the existing list
    head->next = ptr1;        // Update the head to point to the new node
}
//find data by traverse the node  and return the address of the object.
link findnode(link const head, char *key) {
    link ptr = head->next;
    while (ptr != NULL) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}
link lfindnode(link const head, char *key) {
    link ptr = head;
    while (ptr != NULL) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}
link rfindnode(link const head, char *key) {
    link ptr = head;
    while (ptr != NULL) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr;
        }
        ptr = ptr->prev;
    }
    return NULL;
}
//updata data, have a new value
void updateNodeValue(link node, char *newValue) {
    free(node->value);
    node->value = strdup(newValue);
    printf("New value: %s\n", newValue);
}
//avoid memory leak
void freelist(link head) {
    link ptr1;
    while (head != NULL) {
        ptr1 = head;
        head = head->next;
        free(ptr1);
    }
}
