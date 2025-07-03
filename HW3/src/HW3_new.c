#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myredis.h"
void LPUSH(link* head, link* tail, char* key, char* value);
void RPUSH(link* head, link* tail, char* key, char* value);
void LPOP(link* head, link* tail,link ptr);
void RPOP(link* head, link* tail,link ptr);
void LLEN(link const head,char *key);
void LRANGE(link const head, char *key, int start, int stop);
int main() {
    link head = NULL, tail = NULL;
    link ptr=NULL;
    link KVhead= (link)malloc(sizeof(KV));

    char cmd[7], key[20], value[20];
    char input[1000];
    while (1) {
        printf("Enter the operation (LPUSH, RPUSH, LPOP, RPOP, LLEN, LRANGE) OR (SET, GET, DEL, UPD): ");
        scanf("%s", cmd);
        if (strcmp(cmd, "LPUSH") == 0 || strcmp(cmd, "RPUSH") == 0) {
            scanf("%s", key);
            char input[100];
            fgets(input, sizeof(input), stdin);
            size_t len=strlen(input);
            if(len>0 && input[len-1]=='\n'){
                input[len-1]='\0';
            }
            char* token = strtok(input, " ");
            while (token != NULL) {
                //*token = '\0'; 
                if(strcmp(cmd, "LPUSH") == 0) LPUSH(&head, &tail, key, token);
                else RPUSH(&head, &tail, key, token);
                token = strtok(NULL, " ");
            }
        }
        else if (strcmp(cmd, "LPOP") == 0) {
            scanf("%s", key);
            ptr=lfindnode(head,key);
            LPOP(&head, &tail,ptr);
        } 
        else if (strcmp(cmd, "RPOP") == 0) {
            scanf("%s", key);
            ptr=rfindnode(tail,key);
            RPOP(&head, &tail,ptr);
        }
        else if (strcmp(cmd,"LLEN") ==0)
        {
            scanf("%s", key);
            LLEN(head,key);
        }
        else if (strcmp(cmd, "LRANGE") == 0) {
            int start, stop;
            scanf("%s %d %d", key, &start, &stop);
            LRANGE(head, key, start, stop);
        }
        else if(strcmp(cmd,"SET")==0){
            scanf("%s", key);
            scanf("%s", value);
            dbinsert(KVhead,key,value);
        }
        else if(strcmp(cmd,"GET")==0){
            scanf("%s", key);
            dbGET(KVhead,key);
        }
        else if(strcmp(cmd,"DEL")==0){
            scanf("%s", key);
            dbDEL(KVhead,key);
        }
        else if(strcmp(cmd,"UPD")==0){
            scanf("%s", key);
            scanf("%s", value);
            dbUPD(KVhead,key,value);
        }
        else
        {
            scanf("%s", key);
            char input[100];
            fgets(input, sizeof(input), stdin);
            char* token = strtok(input, " "); 
            while (token != NULL) {
                //*token = '\0'; 
                if(strcmp(cmd, "LPUSH") == 0) LPUSH(&head, &tail, key, token);
                else RPUSH(&head, &tail, key, token);
                token = strtok(NULL, " ");
            }
            printf("Wrong input!!!\n");
        }
    }
}

void LPUSH(link* head, link* tail, char* key, char* value) {
    link new_node = (link)malloc(sizeof(KV));
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->prev = NULL;
    new_node->next = *head;

    if (*head != NULL) {
        (*head)->prev = new_node;
    }

    if (*tail == NULL) {
        *tail = new_node;
    }

    *head = new_node;
}

void RPUSH(link* head, link* tail, char* key, char* value) {
    link new_node = (link)malloc(sizeof(KV));
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->prev = *tail;
    new_node->next = NULL;

    if (*tail != NULL) {
        (*tail)->next = new_node;
    }

    if (*head == NULL) {
        *head = new_node;
    }

    *tail = new_node;
}

void LPOP(link* head, link* tail, link ptr) {
    link current = NULL;
    if (ptr == NULL) {
        printf("No data can be popped from the left\n");
        return;
    }

    if (*head == ptr) {
        current = *head;
        if (current->next != NULL) {
            *head = current->next;
            (*head)->prev = NULL;
        } else {
            *head = NULL;
            *tail = NULL;
        }
    } else {
        current = ptr;
        if (current->prev != NULL) {
            current->prev->next = current->next;
        }
        if (current->next != NULL) {
            current->next->prev = current->prev;
        }
    }
    printf("%s\n", current->value);
    free(current->key);
    free(current->value);
    free(current);
}

void RPOP(link* head, link* tail, link ptr) {
    link current = NULL;
    if (ptr == NULL) {
        printf("No data can be popped from the right\n");
        return;
    }

    if (*tail == ptr) {
        current = *tail;
        if (current->prev != NULL) {
            *tail = current->prev;
            (*tail)->next = NULL;
        } else {
            *head = NULL;
            *tail = NULL;
        }
    } else {
        current = ptr;
        if (current->prev != NULL) {
            current->prev->next = current->next;
        }
        if (current->next != NULL) {
            current->next->prev = current->prev;
        }
    }
    printf("%s\n", current->value);
    free(current->key);
    free(current->value);
    free(current);
}
void LLEN(link const head,char *key)
{
    link ptr = head;
    int length=0;
    while(ptr!=NULL){
        if(strcmp(ptr->key, key) == 0) length++;
        else length=length;
        ptr=ptr->next;
    } 
    printf("Length: %d\n",length);
}
void LRANGE(link const head, char *key, int start, int stop) {
    if(head==NULL)
    {
        printf("Can't find this key\n");
        return ;
    }
    link ptr = head;
    int index = 0;
    int length = 0;
    // Calculate the length of the list
    while (ptr != NULL) {
        if (strcmp(ptr->key, key) == 0) {
            length++;
        }
        ptr = ptr->next;
    }
    // Handle negative indices
    if (start < 0) {
        start = length + start;
    }
    if (stop < 0) {
        stop = length + stop;
    }
    ptr = head;  // Reset pointer to the beginning
    while (ptr != NULL) {// Print elements within the specified range
        if(length==0)
        {
            printf("Can't find this key");
            break;
        }
        if (strcmp(ptr->key, key) == 0) {
            if (index >= start && index <= stop) {
                printf("%s:%s ", ptr->key, ptr->value);
            }
            index++;
        }
        ptr = ptr->next;
    }
    printf("\n");
}
