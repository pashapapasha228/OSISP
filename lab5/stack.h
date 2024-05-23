#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct stackNode {
    pthread_t threadId;
    struct stackNode* next;
} STACKNODE;

void pushStack(STACKNODE**, pthread_t);
void popStack(STACKNODE**);