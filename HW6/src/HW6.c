#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ev.h>
//define insert total to calcauate load factor
int HASH_SIZE=5;
#define LOAD_FACTOR_THRESHOLD 0.75
#define LOAD_FACTOR_THRESHOLD_MIN 0.2
#define HASH_SIZE_origin 5
#define MAX_ARGUMENT_LENGTH 100
unsigned int insert_number=0;
// Declare hashTable
typedef struct {
    char *key;
    char *field;
    char *value;
    struct ev_timer *timer; 
} KeyValue;


typedef struct HashNode {
    KeyValue *data;
    struct HashNode *next;
} HashNode;


unsigned int hashFunction(const char *str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}
HashNode **hashTable;
// Initialize the hash table
void initializeHashTable(int size) {
    hashTable = (HashNode **)malloc(size * sizeof(HashNode *));
    //initializes all the pointers in the hashTable array to NULL
    memset(hashTable, 0, size * sizeof(HashNode *));
    HASH_SIZE = size;
}

// Resize the hash table
void resizeHashTable(int newSize) {
    HashNode **newTable = (HashNode **)malloc(newSize * sizeof(HashNode *));
    memset(newTable, 0, newSize * sizeof(HashNode *));

    // Rehash all existing elements
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *current = hashTable[i];
        while (current != NULL) 
        {
            HashNode *next = current->next;
            unsigned int newIndex = hashFunction(current->data->key) % newSize;
            current->next = newTable[newIndex];
            newTable[newIndex] = current;

            current = next;
        }
    }
    // Free the old hash table
    free(hashTable);
    // Update hashTable pointer
    hashTable = newTable;
    HASH_SIZE = newSize;
}


void insertKeyValue(const char *key, const char *field, const char *value) {
    unsigned int index = hashFunction(key) % HASH_SIZE;

    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    KeyValue *newKeyValue = (KeyValue *)malloc(sizeof(KeyValue));

    newKeyValue->key = strdup(key);
    newKeyValue->field = strdup(field);
    newKeyValue->value = strdup(value);

    newNode->data = newKeyValue;
    newNode->next = NULL; 
    insert_number+=1;

    if (hashTable[index] == NULL) {
        hashTable[index] = newNode;
    } 
    else {
        HashNode *current = hashTable[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Calculate load factor
double calculateLoadFactor() {
    return (double)insert_number / HASH_SIZE;
}

const char *getHashValue(const char *key, const char *field) {
    unsigned int index = hashFunction(key) % HASH_SIZE;

    HashNode *current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->data->key, key) == 0 && strcmp(current->data->field, field) == 0) {
            return current->data->value;
        }
        current = current->next;
    }

    return NULL;
}

void deleteKeyValue(const char *key, const char *field) {
    unsigned int index = hashFunction(key) % HASH_SIZE;

    HashNode *current = hashTable[index];
    HashNode *prev = NULL;

    while (current != NULL) {
        if (strcmp(current->data->key, key) == 0 && strcmp(current->data->field, field) == 0) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            } 
            else {
                prev->next = current->next;
            }
            //return space
            free(current->data->key);
            free(current->data->field);
            free(current->data->value);
            free(current->data);
            free(current);
            insert_number-=1;
            return;
        }
        prev = current;
        current = current->next;
    }
}

void setKeyExpire(const char *key,const char *field,int seconds) {
    unsigned int index = hashFunction(key) % HASH_SIZE;

    HashNode *current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->data->key, key) == 0 && strcmp(current->data->field, field) == 0) {
            //ENSURE THE DATA IS IN HASHTABLE
            return;
        }
        current = current->next;
    }
    printf("CAN'T NOT FIND THE KEY %s in hashtable\n",key);
}

static void expireCallback(EV_P_ ev_timer *w, int revents) {
    KeyValue *data = (KeyValue *)(w->data);
    ev_timer_stop(EV_DEFAULT_ w);  
    deleteKeyValue(data->key, data->field);
    printf("THE key %s is deleted from hashtable\n",data->key);
    free(data->key);
    free(data->field);
    free(data);
    free(w);
}

static void commandCallback(EV_P_ ev_io *w, int revents) {
    char command[MAX_ARGUMENT_LENGTH];
    if (scanf("%s", command) != 1) {
        // Handle input error
        return;
    }
    if (strcmp(command, "HSET") == 0) {
        char key[MAX_ARGUMENT_LENGTH];
        char field[MAX_ARGUMENT_LENGTH];
        char value[MAX_ARGUMENT_LENGTH];
        if (scanf("%s %s %s", key, field, value) != 3) {
            // Handle input error
            return;
        }

        insertKeyValue(key, field, value);
        printf("THE KEY %s is insert in hashtable\n", key);
    } 
    else if (strcmp(command, "HGET") == 0) {
        char key[MAX_ARGUMENT_LENGTH];
        char field[MAX_ARGUMENT_LENGTH];
        if (scanf("%s %s", key, field) != 2) {
            // Handle input error
            return;
        }

        const char *value = getHashValue(key, field);
        printf("THE value of %s is %s\n", key, value ? value : "(null)");
    } 
    else if (strcmp(command, "HDEL") == 0) {
        char key[MAX_ARGUMENT_LENGTH];
        char field[MAX_ARGUMENT_LENGTH];
        if (scanf("%s %s", key, field) != 2) {
            // Handle input error
            return;
        }

        deleteKeyValue(key, field);
        printf("THE key %s is deleted from hashtable\n", key);
    } 
    else if (strcmp(command, "EXPIRE") == 0)
    {
        char key[MAX_ARGUMENT_LENGTH];
        char field[MAX_ARGUMENT_LENGTH];
        int seconds;

        if (scanf("%s %s %d", key, field, &seconds) != 3) {
            // Handle input error
            printf("Invalid input for EXPIRE command.\n");
            return;
        }

        setKeyExpire(key, field, seconds);
        // Create and start the timer
        struct ev_timer *expire_timer = (struct ev_timer *)malloc(sizeof(struct ev_timer));
        expire_timer->data = malloc(sizeof(KeyValue));
        ((KeyValue *)expire_timer->data)->key = strdup(key);
        ((KeyValue *)expire_timer->data)->field = strdup(field);
        ev_timer_init(expire_timer, expireCallback, seconds, 0.0);
        ev_timer_start(EV_DEFAULT_ expire_timer);
    }

    if (calculateLoadFactor() >= LOAD_FACTOR_THRESHOLD) {
        //addm hashtable
        printf("Rehashing now\n");
        printf("THE original size is %d\n",HASH_SIZE);
        resizeHashTable(HASH_SIZE * 2);
        printf("Rehashing finished\n");
        printf("THE latest size is %d\n",HASH_SIZE);
    }
    else if(calculateLoadFactor()<LOAD_FACTOR_THRESHOLD_MIN && HASH_SIZE!=HASH_SIZE_origin)
    {
        //reduce hashtable
        printf("Rehashing now\n");
        printf("THE original size is %d\n",HASH_SIZE);
        resizeHashTable(HASH_SIZE/2);
        printf("Rehashing finished\n");
        printf("THE latest size is %d\n",HASH_SIZE);
    }
}


int main() {
    ev_io command_watcher;
    ev_io_init(&command_watcher, commandCallback, 0, EV_READ);
    ev_io_start(EV_DEFAULT_ &command_watcher);

    initializeHashTable(HASH_SIZE);

    if (ev_run(EV_DEFAULT_ 0) == -1) {
        perror("Error in ev_run");
    }
    printf("After ev_run\n");
    return 0;
}

