#include "mainfunc.h"

void *fillingMessages() {
    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);       // Открыть семафоры.
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* fillSem = sem_open(FILL_SEM_NAME, 0);
    if(fillSem == SEM_FAILED) {
        printf("Error while opening filling semaphore.\n");
        exit(EXIT_FAILURE);
    }
    while (continuing) {
        sem_wait(queueAccess);                                   // Ожидание доступа.
        sem_wait(fillSem);

        if (queue->countAdded - queue->countDeleted < MAX_MES_COUNT) {  // Добавить, если есть место.
            push(&queue->ringHead, &queue->ringTail);
            queue->countAdded++;
            printf("Added %d message:\n", queue->countAdded);
            printMes(queue->ringTail->message);
        } else
            printf("Queue is full!\n");

        sem_post(fillSem);                                       // Отпустить семафор.
        sem_post(queueAccess);
        sleep(3);
    }

    sem_close(queueAccess);                                      // Отсоединить семафор.
    sem_close(fillSem);
}
void *extractingMessages() {
    sem_t* queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, 0);     // Подключить семафор.
    if(queueAccess == SEM_FAILED) {
        printf("Error while opening queue access semaphore.\n");
        exit(EXIT_FAILURE);
    }

    sem_t* extractSem = sem_open(EXTRACT_SEM_NAME, 0);
    if(extractSem == SEM_FAILED) {
        printf("Error while opening extracting semaphore.\n");
        exit(EXIT_FAILURE);
    }

    while(continuing) {
        sem_wait(queueAccess);                                    // Ожидание доступа.
        sem_wait(extractSem);

        if(queue->countAdded - queue->countDeleted > 0) {         // Извлечь, если есть что.
            MESSAGE* temp = queue->ringHead->message;
            pop(&queue->ringHead, &queue->ringTail);
            queue->countDeleted++;
            printf("Delete %d message:\n", queue->countDeleted);
            printMes(temp);
            free(temp->data);
            free(temp);
        } else
            printf("Queue is empty!\n");

        sem_post(extractSem);                                     // Отпустить семафор.
        sem_post(queueAccess);
        sleep(3);
    }

    sem_close(queueAccess);                                       // Отсоединить семафор.
    sem_close(extractSem);
}

void changeContinuingStatus() {
    continuing ^= 1;
}