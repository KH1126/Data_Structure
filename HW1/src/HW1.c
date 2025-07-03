#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TWELVE 12
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

int main() {
    link head, ptr;
    char cmd[4], key[10], value[10];
    head = (link)malloc(sizeof(KV));
    head->next = NULL;
    ptr = head;
    char test_data[TWELVE][2][10] = {
    {"KEVIN", "GOOD"}, {"ETHAN", "EXCELLENT"}, {"JIM", "STUDENT"},
    {"LEE", "BIGGUY"}, {"KLAY", "SHOOTER"}, {"LAVINE", "DUNKER"},
    {"wyf", "handsome"}, {"vivian", "pretty"}, {"thomas", "123453"},
    {"CURRY", "SC30"}, {"Kyrie", "211"}, {"james", "king23"}
    };
    for (int i = 0; i < TWELVE; i++) {
        createlink(head, test_data[i][0], test_data[i][1]);
    }
    while (1) {
        printf("Input operation\n");
        scanf("%s %s", cmd, key);
        //get data
        if (strcmp(cmd, "get") == 0) 
        {
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
        //del data
        else if (strcmp(cmd, "del") == 0)
        {
            ptr = findnode(head, key);
            if (ptr != NULL) 
            {
                head = deletenode(head, ptr);
                free(ptr->key);
                free(ptr->value);
                free(ptr);
            } 
            else 
            {
                printf("Didn't find key\n");
            }
        }
        //create data
        else if (strcmp(cmd, "set") == 0) 
        {
            scanf("%s", value);
            head = createlink(head, key, value);
        }
        //update data
        else if (strcmp(cmd, "upd") == 0) 
        {
            scanf("%s", value);
            ptr = findnode(head, key);
            if (ptr != NULL) {
                updateNodeValue(ptr, value);
            } 
            else 
            {
                printf("Didn't find key\n");
            }
        } 
        else 
        {
            break;
        }
    }
    freelist(head);
    return 0;
}
//delete data function, need initial address and findnode return value
link deletenode(link head, link ptr) {
    link previous = head;
    if (ptr == head) {
        head = head->next;
    } else {
        while (previous->next != ptr) {
            previous = previous->next;
        }
        previous->next = ptr->next;
    }
    return head;
}
//create data, need initial address and string of key and value.
link createlink(link head, char *key, char *value) {
    link ptr1 = (link)malloc(sizeof(KV));
    ptr1->next = head->next;
    ptr1->key = strdup(key);
    ptr1->value = strdup(value);
    head->next = ptr1;
    return head;
}
//find data by traverse the node  and return the address of the object.
link findnode(link head, char *key) {
    link ptr = head->next;
    while (ptr != NULL) {
        if (strcmp(ptr->key, key) == 0) {
            return ptr;
        }
        ptr = ptr->next;
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
        //free(ptr1->key);
        //free(ptr1->value);
        free(ptr1);
    }
}