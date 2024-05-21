#ifndef MEMHASH_H
#define MEMHASH_H

#define SUCCESS 0
#define NOT_FOUND 1
#define TABLE_SIZE 256
#define STRING 1
#define INTEGER 2
#define BINARY 3

typedef struct node {
    char *key;
    void *value;
    int binary_size;
    int datatype;
    struct node *next;
} node;

node *table[TABLE_SIZE];

void node_gen_str(char *, char *, int);
void node_gen_int(char *, int, int);
void node_gen_bin(char *, char *, int, int);
node *node_gen(char *, void *, int, int);
void init_table();
void insert(node *);
int del(char *);
int query(char *, node *);
unsigned int hash(char *);
#endif