#include <stdio.h>
#include <stdlib.h>

typedef struct NODE {
    int num;
    pid_t pid;
    struct NODE* next;
} NODE;

void push(NODE**, pid_t);
void pop(NODE**);
void show(NODE*);
void clear(NODE**);
int size(NODE*);