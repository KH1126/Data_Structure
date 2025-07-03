#ifndef MY_VALUE_H
#define MY_VALUE_H

#include <stdbool.h>

enum ValueType {
    VALUE_INT,
    VALUE_STRING,
    VALUE_MAP,
};

struct tree
{
    int index;
    char *input;
    struct tree *next;
};
typedef struct tree TREE;
typedef TREE *TNODE;

typedef struct Value {
    enum ValueType type; // tyoe of value
    char* key;
    union {
        int int_value;
        char* string_value; // If the type is MAP, this is the key
        struct Value* map_value;
    };
    // If the type is MAP, this is the value
    struct Value* next;
    struct Value* prev;
} Value;
TNODE Tfindnode(TNODE const head, int index);
void createTreelink(TNODE head, char *input, int index);
void TdbGET(TNODE head,int index);
TNODE TdbDEL(TNODE head,int index);
TNODE Tdeletenode(TNODE head, TNODE ptr);
Value* parse_value(char* input_string);
Value* value_get(Value* value, char* key);
bool value_is_int(Value* value);
bool value_is_string(Value* value);
bool value_is_map(Value* value);
#endif