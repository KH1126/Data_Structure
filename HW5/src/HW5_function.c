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
SSNODE findsort(const SSNODE  head,char *key)
{
    SSNODE ptr=head;
    while(ptr!=NULL)
    {
        if(strcmp(ptr->key, key) == 0)
        {
            return ptr;
        }
        else 
        {
            ptr=ptr->next;
        }
    }
    return NULL;
}
//delete key in Specified address
SSNODE deletesort(const SSNODE head, char *key)
{
    SSNODE ptr2 = findsort(head, key);
    if (ptr2 == head)
    {
        SSNODE ptr = ptr2;
        if (ptr2->next != NULL)
        {
            ptr2->next->prev = NULL;
        }
        ptr2 = ptr2->next;
        free(ptr);
        return ptr2;
    }
    else if (ptr2 != NULL)
    {
        ptr2->prev->next = ptr2->next;
        if (ptr2->next != NULL)
        {
            ptr2->next->prev = ptr2->prev;
        }
        free(ptr2);
    }
    else 
    {
        printf("Nothing can delete from linked list\n");
    }
    return head;
}

SSNODE createsort(SSNODE head, int index, char *key)
{
    SSNODE ptr = head;
    // Check if the key already exists in the list, and if so, delete it
    SSNODE existingNode = findsort(head, key);
    if (existingNode != NULL)
    {
        existingNode = deletesort(head, key);
        ptr=existingNode;
        //return existingNode;
    }
    

    // Create a new node
    SSNODE newNode = (SSNODE)malloc(sizeof(SSET));
    newNode->index = index;
    newNode->key = strdup(key);
    newNode->prev = NULL;
    newNode->next = NULL;

    // If the list is empty or the new node should be the head
    if (ptr == NULL || index < ptr->index)
    {
        newNode->next = ptr;
        if (ptr != NULL)
        {
            ptr->prev = newNode;
        }
        return newNode;
    }
    if(ptr->next==NULL)
    {
        ptr->next=newNode;
        newNode->prev=ptr;
        return head;
    }
    // Find the correct position to insert the new node
    while (ptr->next != NULL && ptr->next->index <= index)
    {
        ptr = ptr->next;
    }

    // Insert the new node at the correct position
    newNode->next = ptr->next;
    if (ptr->next != NULL)
    {
        ptr->next->prev = newNode;
    }
    newNode->prev = ptr;
    ptr->next = newNode;


    return head;
}

//find name address
SNODE findname(const SNODE head, const char *name)
{
    SNODE ptr = head;
    while (ptr != NULL)
    {
        if (strcmp(ptr->sorted_name, name) == 0)
        {
            return ptr;
        }
        else 
        {
            ptr = ptr->next;
        }
    }
    return NULL;
}

//delete name in Specified address

SNODE deletename(const SNODE head, char *name)
{
    SNODE ptr2 = findname(head, name);

    if (ptr2 == head)
    {
        SNODE newHead = ptr2->next;
        if (newHead != NULL)
        {
            newHead->prev = NULL;
        }
        free(ptr2);
        return newHead;
    }
    else if (ptr2 != NULL)
    {
        if (ptr2->prev != NULL)
        {
            ptr2->prev->next = ptr2->next;
        }
        if (ptr2->next != NULL)
        {
            ptr2->next->prev = ptr2->prev;
        }
        free(ptr2);
    }
    else
    {
        printf("NOTHING TO DELETE\n");
    }
    return head;
}


SNODE createname(SNODE head, char *name)
{
    SNODE ptr = head;
    
    if (ptr == NULL)
    {
        ptr = (SNODE)malloc(sizeof(SORTED));
        ptr->head = NULL;
        ptr->sorted_name = strdup(name);
        ptr->next = NULL;
        ptr->prev = NULL;
        return ptr;
    }
    else
    {
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }

        SNODE ptr2;
        ptr2 = (SNODE)malloc(sizeof(SORTED));
        ptr2->head = NULL;
        ptr2->sorted_name = strdup(name);
        ptr2->next = ptr->next;
        if (ptr->next != NULL)
        {
            ptr->next->prev = ptr2;
        }
        ptr2->prev = ptr;
        ptr->next = ptr2;

        return head;
    }
}
SNODE ZADD(SNODE SHEAD_1)
{
    SNODE SHEAD=NULL,ptr;
    char name[20];
    scanf("%s", name);
    SHEAD=createname(SHEAD_1,name);
    ptr=findname(SHEAD,name);
    char input[100];
    fgets(input, sizeof(input), stdin);
    size_t len=strlen(input);
    if(len>0 && input[len-1]=='\n'){
        input[len-1]='\0';
    }
    char* token = strtok(input, " ");
    int index;
    while (token != NULL) {
        //*token = '\0'; 
        //if(strcmp(cmd, "ZADD") == 0) //LPUSH(&head, &tail, key, token);
        index = atoi(token); // Convert token to integer
        token = strtok(NULL, " ");
        ptr->head=createsort(ptr->head, index, token);
        token = strtok(NULL, " ");
    }
    return SHEAD;
}
void ZCARD(const SNODE const head)
{
    SNODE ptr;
    SSNODE ptr2;
    char name[20];
    scanf("%s", name);
    ptr=findname(head,name);
    ptr2=ptr->head;
    int total=0;
    while (ptr2!=NULL)
    {
        total+=1;
        ptr2=ptr2->next;
    }
    printf("The number of elements in %s has %d elements.\n",name,total);
    return;
}
void ZCOUNT(const SNODE const head)
{
    SNODE ptr;
    SSNODE ptr2;
    char name[20];
    int min=-1,max=-1;
    scanf("%s", name);
    ptr=createname(head,name);
    ptr=findname(head,name);
    scanf("%d",&min);
    scanf("%d",&max);
    if(min>max) 
    {
        printf("Minimum is larger than Maximum\n");
        return ;
    }
    ptr=findname(head,name);
    ptr2=ptr->head;
    int total=0;
    while (ptr2!=NULL)
    {
        if(ptr2->index>=min && ptr2->index<=max)
        {
            total+=1;
            ptr2=ptr2->next;
        }
        else if(ptr2->index<min)
        {
            ptr2=ptr2->next;
        }
        else if(ptr2->index>max) break;
    }
    printf("The number of elements between %d and %d in %s has %d elements.\n",min,max,name,total);
    return;
}
void intersection(SNODE head, SSNODE head1, SSNODE head2, int power1, int power2) {
    // Clear the original linked list
    if (head->head != NULL) {
        free(head->head);
        head->head = NULL;
    }

    // head1 is the set to be stored
    SSNODE returnhead = NULL;
    
    if (head1 == NULL || head2 == NULL) {
        return;
    } else {
        SSNODE ptr = head1;
        while (ptr != NULL) {
            SSNODE ptr2 = findsort(head2, ptr->key);
            if (ptr2 == NULL) {
                ptr = ptr->next;
                continue;
            } else {
                int power = (ptr->index) * power1 + (ptr2->index) * power2;
                returnhead = createsort(returnhead, power, ptr->key);
            }
            ptr = ptr->next;
        }

        // Update the head with the result
        head->head = returnhead;
        return;
    }
}
void ZINTERSTORE(SNODE head)
{
    int power1, power2;
    SSNODE ptr1, ptr2;
    SNODE headfake, ptrname, ptrname2;
    char name[20];
    char result_name[20];
    
    scanf("%s", result_name);
    scanf("%s", name);
    ptrname = findname(head, name);
    if (ptrname == NULL) {
        printf("Set '%s' not found\n", name);
        return;
    }
    scanf("%s", name);
    ptrname2 = findname(head, name);
    if (ptrname2 == NULL) {
        printf("Set '%s' not found\n", name);
        return;
    }
    ptr1 = ptrname->head;
    ptr2 = ptrname2->head;
    scanf("%d", &power1);
    scanf("%d", &power2);

    if ((headfake = findname(head, result_name)) != NULL) {
        intersection(headfake, ptr1, ptr2, power1, power2);
    } else {
        headfake = createname(head, result_name);
        headfake = findname(head, result_name);
        intersection(headfake, ptr1, ptr2, power1, power2);
    }


    return;
}
void ZUNIONSTORE(const SNODE head) {
    int power1, power2;
    SSNODE ptr1, ptr2;
    SNODE headfake, ptrname, ptrname2;
    char name[20];
    char result_name[20];
    
    scanf("%s", result_name);
    scanf("%s", name);
    ptrname = findname(head, name);
    if (ptrname == NULL) {
        printf("Set '%s' not found\n", name);
        return;
    }
    scanf("%s", name);
    ptrname2 = findname(head, name);
    if (ptrname2 == NULL) {
        printf("Set '%s' not found\n", name);
        return;
    }
    ptr1 = ptrname->head;
    ptr2 = ptrname2->head;
    scanf("%d", &power1);
    scanf("%d", &power2);

    if ((headfake = findname(head, result_name)) != NULL) {
        intersection(headfake, ptr1, ptr2, power1, power2);
    } else {
        headfake = createname(head, result_name);
        headfake = findname(head, result_name);
        intersection(headfake, ptr1, ptr2, power1, power2);
    }
    while(ptr1!=NULL)
    {
        SSNODE ptr3=headfake->head;
        int flag=0;
        while(ptr3!=NULL)
        {
            if(strcmp(ptr1->key,ptr3->key)==0)
            {
                flag=1;
            }
            ptr3=ptr3->next;
        }
        if(flag==0)
        {
            headfake->head=createsort(headfake->head,(ptr1->index)*power1,ptr1->key);
        }
        ptr1=ptr1->next;
    }
    while(ptr2!=NULL)
    {
        SSNODE ptr3=headfake->head;
        int flag=0;
        while(ptr3!=NULL)
        {
            if(strcmp(ptr2->key,ptr3->key)==0)
            {
                flag=1;
            }
            ptr3=ptr3->next;
        }
        if(flag==0)
        {
            headfake->head=createsort(headfake->head,(ptr2->index)*power2,ptr2->key);
        }
        ptr2=ptr2->next;
    }
    return;
}
void ZRANGE(SNODE head) {
    char name[20];
    scanf("%s",name);
    SNODE ptr=findname(head,name);
    SSNODE ptr1=ptr->head;
    if(ptr1==NULL)
    {
        printf("Can't find this key\n");
        return ;
    }
    int length = 0;
    int start,stop;
    int index=0;
    scanf("%d",&start);
    scanf("%d",&stop);
    // Calculate the length of the list
    while (ptr1 != NULL) {
        length++;
        ptr1 = ptr1->next;
    }
    if (start < 0) {
        start = length + start;
    }
    if (stop < 0) {
        stop = length + stop;
    }
    ptr1=ptr->head;  // Reset pointer to the beginning
    while (ptr1 != NULL) {// Print elements within the specified range
        if(length==0)
        {
            printf("Can't find this key");
            break;
        }
        if (index >= start && index <= stop) {
            printf("%s:%d ", ptr1->key, ptr1->index);
        }
        index++;
        ptr1 = ptr1->next;
    }
    printf("\n");
}
void ZRANGEBYSCORE(SNODE head) {
    char name[20];
    scanf("%s",name);
    SNODE ptr=findname(head,name);
    SSNODE ptr1=ptr->head;
    if(ptr1==NULL)
    {
        printf("Can't find this key\n");
        return ;
    }
    int length = 0;
    int start,stop;
    scanf("%d",&start);
    scanf("%d",&stop);
    // Calculate the length of the list
    while (ptr1 != NULL) {// Print elements within the specified range
        if (ptr1->index >= start && ptr1->index <= stop) {
            printf("%s:%d ", ptr1->key, ptr1->index);
        }
        ptr1 = ptr1->next;
    }
    printf("\n");
}
void ZRANK(SNODE head) {
    char name[20];
    scanf("%s", name);
    SNODE ptr = findname(head, name);
    if (ptr == NULL) {
        printf("Set '%s' not found\n", name);
        return;
    }
    scanf("%s", name);
    SSNODE ptr1 = ptr->head;
    int index = 0;
    while (ptr1 != NULL) {
        if (strcmp(ptr1->key, name) == 0) {
            printf("%d\n", index);
            return;
        } else {
            index++;
            ptr1 = ptr1->next;
        }
    }
    // If the key is not found in the set
    printf("Key '%s' not found in set '%s'\n", name, ptr->sorted_name);
}
void ZREM(SNODE head)
{
    char name[20];
    scanf("%s", name);
    SNODE ptr = findname(head, name);
    if (ptr == NULL) {
        printf("Set '%s' not found\n", name);
        return;
    }
    SSNODE ptr1=ptr->head;
    char input[100];
    fgets(input, sizeof(input), stdin);
    size_t len=strlen(input);
    if(len>0 && input[len-1]=='\n'){
        input[len-1]='\0';
    }
    char* token = strtok(input, " ");
    while (token != NULL) {
        ptr1=deletesort(ptr1, token);
        token = strtok(NULL, " ");
    }
    ptr->head=ptr1;
}
void ZREMRANGEBYSCORE(SNODE head)
{
    char name[20];
    scanf("%s", name);
    SNODE ptr = findname(head, name);
    if (ptr == NULL) {
        printf("Set '%s' not found\n", name);
        return;
    }
    SSNODE ptr1=ptr->head;
    SSNODE ptr2=ptr->head;
    int min,max;
    scanf("%d",&min);
    scanf("%d",&max);
    if(min>max) 
    {
        printf("MIN LARGER THAN MAX\n");
        return ;
    }
    while(ptr1!=NULL)
    {
        if(ptr1->index>=min && ptr1->index<=max)
        {
            ptr2=deletesort(ptr2,ptr1->key);
            ptr1=ptr1->next;
        }
        else if(ptr1->index>max)
        { 
            ptr->head=ptr2;
            return ;
        }
        else if(ptr1->index<min)
        {
            ptr1=ptr1->next;
        }
    }
}

