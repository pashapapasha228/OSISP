#include <stdio.h>
#include <stdlib.h>

typedef struct stackNode {
    pid_t pid;
    struct stackNode* next;
} STACKNODE;

void pushStack(STACKNODE**, pid_t);
void popStack(STACKNODE**);