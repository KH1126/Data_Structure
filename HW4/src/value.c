#define __STDC_WANT_LIB_EXT1__ 1
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "value.h"
//check buffer overflow or not
#define can_access_at_index(buffer, index) ((buffer != NULL) && (((buffer)->offset + index) < (buffer)->length))
//!check buffer overflow or not
#define cannot_access_at_index(buffer, index) (!can_access_at_index(buffer, index))
//get buffer address
#define buffer_at_offset(buffer) ((buffer)->content + (buffer)->offset)
//checj bit is required or not
#define check_state(state, requir, expect) ((state & requir) == expect)
//enumerate all situation.
enum TokenizeType {
    TKE_INT = 1,
    TKE_STRING = 1 << 1,
    TKE_LEFT_BRACE = 1 << 2,
    TKE_RIGHT_BRACE = 1 << 3,
    TKE_COMMA = 1 << 4,
    TKE_COLON = 1 << 5,
};

typedef struct
{
    const unsigned char* content;
    size_t length;
    size_t offset;
    size_t depth; /* How deeply nested (in arrays/objects) is the input at the current offset. */
} parse_buffer;

typedef struct QueenItem {
    enum TokenizeType type;
    unsigned char* content;
    struct QueenItem* next;
    struct QueenItem* prev;
} QueenItem;

typedef struct {
    QueenItem* head;
    QueenItem* tail;
    size_t length;
} TokenizerQueen;

Value* value_new(char* key)
{
    Value* value = malloc(sizeof(Value));
    value->type = -1;
    value->key = key;
    value->int_value = 0;
    value->next = NULL;
    value->prev = NULL;
    return value;
}

static QueenItem* queen_item_new(enum TokenizeType type, unsigned char* content)
{
    QueenItem* item = malloc(sizeof(QueenItem));
    item->type = type;
    item->content = content;
    item->next = NULL;
    item->prev = NULL;
    return item;
}

static void queen_push(TokenizerQueen* queen, QueenItem* item)
{
    if (queen->head == NULL) {
        queen->head = item;
        queen->tail = item;
    } else {
        queen->tail->next = item;
        item->prev = queen->tail;
        queen->tail = item;
    }
    queen->length++;
}

static QueenItem* queen_pop(TokenizerQueen* queen)
{
    if (queen->length == 0) {
        return NULL;
    }
    QueenItem* item = queen->head;
    queen->head = item->next;
    queen->length--;
    return item;
}
//NEGLECT WHITSPACE
static void buffer_skip_whitespace(parse_buffer* const buffer)
{
    size_t skipped = 0;
    while (can_access_at_index(buffer, skipped) && isspace(buffer_at_offset(buffer)[skipped])) {
        skipped++;
    }
    buffer->offset += skipped;
}
//STORE INT AND JUDGE TYPE
static bool tokenize_int(QueenItem* item, parse_buffer* const buffer)
{
    size_t length = 0;
    while (can_access_at_index(buffer, length) && isdigit(buffer_at_offset(buffer)[length])) {
        length++;
    }
    if (length == 0) {
        return false;
    }
    item->type = TKE_INT;
    item->content = malloc(length + 1);
    memcpy(item->content, buffer_at_offset(buffer), length);
    item->content[length] = '\0';
    buffer->offset += length;
    return true;
}
//STORE STRING AND JUDGE TYPE
static bool tokenize_string(QueenItem* item, parse_buffer* const buffer)
{
    if (cannot_access_at_index(buffer, 0) || buffer_at_offset(buffer)[0] != '"') {
        return false;
    }
    size_t length = 1;
    
    // String length calculation
    while (can_access_at_index(buffer, length) && buffer_at_offset(buffer)[length] != '"') {
        length++;
    }

    if (cannot_access_at_index(buffer, length) || buffer_at_offset(buffer)[length] != '"') {
        return false;
    }
    item->type = TKE_STRING;
    item->content = malloc(length + 1);
    memcpy(item->content, buffer_at_offset(buffer) + 1, length - 1);
    item->content[length - 1] = '\0';
    buffer->offset += length + 1;
    return true;
}

static void queen_free(TokenizerQueen* queen)
{
    QueenItem* item = queen_pop(queen);
    while (item != NULL) {
        free(item->content);
        free(item);
        item = queen_pop(queen);
    }
    free(queen);
}
//Define JSON SYMBOL
static TokenizerQueen* tokenize(parse_buffer* const buffer)
{
    TokenizerQueen* queen = malloc(sizeof(TokenizerQueen));
    queen->head = NULL;
    queen->tail = NULL;
    queen->length = 0;

    while (can_access_at_index(buffer, 0)) {
        //WHITESPACE OFFSET+1
        buffer_skip_whitespace(buffer);
        if (cannot_access_at_index(buffer, 0)) {
            break;
        }
        QueenItem* item = queen_item_new(TKE_INT, NULL);
        if (tokenize_int(item, buffer) || tokenize_string(item, buffer)) {
            queen_push(queen, item);
            continue;
        }
        switch (buffer_at_offset(buffer)[0]) {
        case '{':
            item->type = TKE_LEFT_BRACE;
            item->content = malloc(2);
            item->content[0] = '{';
            item->content[1] = '\0';
            buffer->offset++;
            queen_push(queen, item);
            break;
        case '}':
            item->type = TKE_RIGHT_BRACE;
            item->content = malloc(2);
            item->content[0] = '}';
            item->content[1] = '\0';
            buffer->offset++;
            queen_push(queen, item);
            break;
        case ',':
            item->type = TKE_COMMA;
            item->content = malloc(2);
            item->content[0] = ',';
            item->content[1] = '\0';
            buffer->offset++;
            queen_push(queen, item);
            break;
        case ':':
            item->type = TKE_COLON;
            item->content = malloc(2);
            item->content[0] = ':';
            item->content[1] = '\0';
            buffer->offset++;
            queen_push(queen, item);
            break;
        default:
            free(item);
            queen_free(queen);
            return NULL;
        }
    }
    return queen;
}
//JUDGE JSON style correct or not
static bool valid_tokenize_map(QueenItem** quee_p)
{
    if (quee_p == NULL || *quee_p == NULL) {
        return false;
    }
    QueenItem* quee = *quee_p;
    if (quee->type != TKE_LEFT_BRACE) {
        return false;
    }

    quee = quee->next;
    unsigned int expect = TKE_STRING | TKE_RIGHT_BRACE;
    while (quee != NULL) {
        if ((expect & quee->type) == 0) {
            return false;
        }
        switch (quee->type) {
        case TKE_STRING:
            // string is key
            if (!(expect & TKE_INT)) {
                expect = TKE_COLON;
            } else {
                expect = TKE_COMMA | TKE_RIGHT_BRACE;
            }
            break;
        case TKE_INT:
            expect = TKE_COMMA | TKE_RIGHT_BRACE;
            break;
        case TKE_COLON:
            expect = TKE_INT | TKE_STRING | TKE_LEFT_BRACE;
            break;
        case TKE_COMMA:
            expect = TKE_STRING;
            break;
        case TKE_RIGHT_BRACE:
            *quee_p = quee;
            return true;
        case TKE_LEFT_BRACE:
            if (!valid_tokenize_map(&quee)) {
                return false;
            }
            expect = TKE_COMMA | TKE_RIGHT_BRACE;
            break;
        default:
            break;
        }

        quee = quee->next;
    }
    return false;
}

//Check JSON tokenize is valid or not
static bool valid_tokenize(TokenizerQueen* queen)
{
    if (queen == NULL || queen->head == NULL) {
        return false;
    }
    unsigned int expect = TKE_INT | TKE_STRING | TKE_LEFT_BRACE;

    switch (queen->head->type) {
    case TKE_INT:
    case TKE_STRING:
        return queen->length == 1;
        break;
    case TKE_LEFT_BRACE: {
        QueenItem* queen_item = queen->head;
        //if {, use valid_tokenize_map 
        if (!valid_tokenize_map(&queen_item)) {
            return false;
        }
        return queen_item->next == NULL;
    }
    default:
        return false;
    }
}

static Value* deserialization_map(TokenizerQueen* queen)
{
    //TokenizerQueen is not null：
    assert(queen != NULL && queen->head != NULL);

    Value* head = NULL;
    Value* curr_value = NULL;
    Value* new_value = NULL;

    QueenItem* item = queen_pop(queen);
    // While loop to parse key-value pairs until right brace is found
    while (item->type != TKE_RIGHT_BRACE) {
        switch (item->type) {
        case TKE_STRING:
            // string is key
            if (new_value == NULL) {
                new_value = value_new((char*)item->content);
                break;
            }
            // string is value
            new_value->type = VALUE_STRING;
            new_value->string_value = (char*)item->content;

            break;
        case TKE_INT:
            // int is value
            new_value->type = VALUE_INT;
            new_value->int_value = atoi((char*)item->content);
            break;
        case TKE_LEFT_BRACE:
            // map is value
            new_value->type = VALUE_MAP;
            // If left brace is found, it means a new {
            new_value->map_value = deserialization_map(queen);
            break;
        default:
            break;
        }
        free(item);
        item = queen_pop(queen);

        // if the kv pair is not complete, continue
        if (new_value == NULL || new_value->type == -1) {
            continue;
        }

        // add to map list
        if (head == NULL) {
            head = new_value;
        } else {
            curr_value->next = new_value;
            new_value->prev = curr_value;
        }
        curr_value = new_value;
        new_value = NULL;
    }
    return head;
}

static Value* deserialization(TokenizerQueen* queen)
{
    if (queen == NULL) {
        return NULL;
    }
    QueenItem* item = queen_pop(queen);
    if (item == NULL) {
        return NULL;
    }
    Value* value = NULL;
    switch (item->type) {
    case TKE_INT:
        value = value_new(NULL);
        value->type = VALUE_INT;
        value->int_value = atoi((char*)item->content);
        break;
    case TKE_STRING:
        value = value_new(NULL);
        value->type = VALUE_STRING;
        value->string_value = (char*)item->content;
        break;
    case TKE_LEFT_BRACE:
        value = value_new(NULL);
        value->type = VALUE_MAP;
        value->map_value = deserialization_map(queen);
        break;
    default:
        break;
    }
    free(item);
    return value;
}

Value* parse_value(char* input_string)
{
    parse_buffer buffer = {
        .content = (const unsigned char*)input_string,
        .length = strlen(input_string),
        .offset = 0,
        .depth = 0,
    };
    TokenizerQueen* queen = tokenize(&buffer);
    if (queen == NULL) {
        printf("tokenize error\n");
        return NULL;
    }
    if (!valid_tokenize(queen)) {
        printf("valid_tokenize error");
        return NULL;
    }
    Value* value = deserialization(queen);
    if (value == NULL) {
        printf("deserialization error\n");
        return NULL;
    }
    return value;
}

Value* value_get(Value* value, char* key)
{
    if (value == NULL || key == NULL || value->type != VALUE_MAP) {
        return NULL;
    }
    Value* curr_value = value->map_value;

    while (curr_value != NULL) {
        if (strcmp(curr_value->key, key) == 0) {
            return curr_value;
        }
        curr_value = curr_value->next;
    }
    return NULL;
}

bool value_is_int(Value* value)
{
    return value != NULL && value->type == VALUE_INT;
}

bool value_is_string(Value* value)
{
    return value != NULL && value->type == VALUE_STRING;
}

bool value_is_map(Value* value)
{
    return value != NULL && value->type == VALUE_MAP;
}
//create data, need initial address and string of key and value.
void createTreelink(TNODE head, char *input, int index) {
    TNODE ptr1 = (TNODE)malloc(sizeof(TREE));
    ptr1->index = index;
    ptr1->input = strdup(input);
    ptr1->next = head->next;  // Link the new node to the existing list
    head->next = ptr1;        // Update the head to point to the new node
}
void TdbGET(TNODE head,int index)
{
    TNODE ptr;
    ptr = Tfindnode(head, index);
    if (ptr != NULL) 
    {
        Value* value = parse_value(ptr->input);
        char inputs[100];
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        fgets(inputs, sizeof(inputs), stdin); 
        size_t len=strlen(inputs);
        if(len>0 && inputs[len-1]=='\n'){
            inputs[len-1]='\0';
        }
        char* token = strtok(inputs, " "); 
        char tmp[10];
        Value* name=value;
        while (token != NULL) {
            name=value_get(name, token);
            strcpy(tmp,token);
            token = strtok(NULL, " ");
        }
        if(name==NULL) printf("DIDN'T FIND\n");
        if(value_is_int(name)) printf("%s:%d\n", tmp,name->int_value);
        else if(value_is_string(name)) printf("%s:%s\n", tmp,name->string_value);
        else if(value_is_map(name)) printf("NOT come to base\n");
        //else printf("NOT find the value");
    }
    else
    {
        printf("Didn't find key\n");
    }
}
TNODE Tfindnode(TNODE const head, int index) {
    TNODE ptr = head->next;
    while (ptr != NULL) {
        if (ptr->index==index) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}
TNODE TdbDEL(TNODE head,int index)
{
    TNODE ptr = Tfindnode(head, index);
    if (ptr != NULL) 
    {
        head = Tdeletenode(head, ptr);
        free(ptr);
    } 
    else 
    {
        printf("Didn't find key\n");
    }
    return head;
}
//delete data function, need initial address and findnode return value
TNODE Tdeletenode(TNODE head, TNODE ptr) {
    TNODE previous = head;
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