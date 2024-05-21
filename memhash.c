#include "memhash.h"

#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_table() {
    for (int i = 0; i < TABLE_SIZE; i++) table[i] = NULL;
}

node* node_gen(char* key, void* value, int datatype, int binery_size) {
    unsigned int index = hash(key) % TABLE_SIZE;
    node* newnode = (node*)malloc(sizeof(node));
    if (!newnode) {
        printf("Fail to malloc\n");
        return NULL;
    }
    int key_len = strlen(key);
    newnode->key = (char*)malloc(sizeof(char) * (key_len + 1));
    if (!newnode->key) {
        printf("Fail to malloc\n");
        return NULL;
    }
    strcpy(newnode->key, key);
    switch (datatype) {
        case STRING: {
            int value_len = strlen(value);
            newnode->value = (void*)malloc(sizeof(void) * (value_len + 1));
            if (!newnode->value) {
                printf("Fail to malloc\n");
                return NULL;
            }
            strcpy(newnode->value, value);
            //strdup
            // printf("%s", newnode->char_value);
            break;
        }
        case INTEGER: {
            newnode->value = value;
            // printf("%d", newnode->int_value);
            break;
        }
        case BINARY: {
            newnode->value = (void*)malloc(sizeof(void) * (binery_size));
            if (!newnode->value) {
                printf("Fail to malloc\n");
                return NULL;
            }
            memcpy(newnode->value, value, binery_size);
            /*for (int i = 0; i < binery_size; i++) {
                if (((char*)newnode->value)[i] == '\0') printf(" 0 ");
                printf("%c ", ((char*)newnode->value)[i]);
            }*/
            break;
        }
    }
    newnode->datatype = datatype;
    return newnode;
}

/*
若該筆資料 (Key) 不存在，則新增此筆資料
若該筆資料 (Key) 已存在，則更新其 Value
*/
void insert(node* newnode) {
    unsigned int index = hash(newnode->key) % TABLE_SIZE;
    node* temp = table[index];
    while (temp != NULL && strcmp(temp->key, newnode->key) != 0)
        temp = temp->next;
    if (temp != NULL) {
        // strcpy(temp->value, value);
        /*switch (newnode->datatype) {
            case STRING: {
                temp->char_value = newnode->char_value;
                break;
            }
            case INTEGER: {
                temp->int_value = newnode->int_value;
                break;
            }
            case BINARY: {
                temp->binery_value = newnode->binery_value;
                break;
            }
        }*/
        temp->value = newnode->value;
        temp->datatype = newnode->datatype;
    } else {
        newnode->next = table[index];
        table[index] = newnode;
    }
}

/*
若該筆資料不存在，則回傳 NOT FOUND
若該筆資料已存在，則進行刪除動作
*/
int del(char* key) {
    unsigned int index = hash(key) % TABLE_SIZE;
    node* temp = table[index];
    node* prev = NULL;
    while (temp != NULL && strcmp(temp->key, key) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        return NOT_FOUND;
    } else {
        if (prev == NULL) {
            table[index] = table[index]->next;
        } else {
            prev->next = temp->next;
        }

        //free(temp->key);
        //free(temp->value);
        free(temp);
        return SUCCESS;
    }
}

/*
若該筆資料不存在，則回傳 NOT FOUND
若該筆資料已存在，則是可取得該資料的 Value
*/
int query(char* key, node* result) {
    node* temp = table[hash(key) % TABLE_SIZE];
    // printf("%d",returnnode->datatype);
    while (temp != NULL && strcmp(temp->key, key) != 0) temp = temp->next;
    if (temp == NULL) {
        printf("Key doesn't exist!\n");
        return NOT_FOUND;
    } else {
        *result = *temp;
    }
    return SUCCESS;
}

unsigned int hash(char* key) {
    // MD5
    unsigned char digest[MD5_DIGEST_LENGTH];
    unsigned int hash_value;
    MD5(key, strlen(key), digest);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        hash_value = (hash_value << 8) | digest[i];
    }
    return hash_value % TABLE_SIZE;
}
node duplicate(node* result) {
    node copy = {.key = result->key,
                 .value = result->value,
                 .datatype = result->datatype,
                 .binary_size = result->binary_size};
    //*result = copy;
    return copy;
}