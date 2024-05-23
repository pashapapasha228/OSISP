#include "mainfunc.h"

int continuing = 1;
int MAX_MES_COUNT = 20;

STACKNODE* stackFiller = NULL;
STACKNODE* stackExtractor = NULL;

QUEUE* queue;

int main() {
    srand(time(NULL));

    sem_unlink(FILL_SEM_NAME);                        // Очистка семафоров, если существуют.
    sem_unlink(EXTRACT_SEM_NAME);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    signal(SIGUSR1, changeContinuingStatus);          // Сигнал остановки.

    queue = (QUEUE*)malloc(sizeof(QUEUE));            // Очередь.
    queue->ringHead = 0;
    queue->ringTail = 0;
    queue->countDeleted = 0;
    queue->countAdded = 0;

    sem_t* fillSem;                                   // Инициализация семафоров.
    if((fillSem = sem_open(FILL_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open filling semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t* extractSem;
    if((extractSem = sem_open(EXTRACT_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open extracting semaphore, code %d.\n", errno);
        exit(errno);
    }
    sem_t* queueAccess;
    if((queueAccess = sem_open(QUEUE_ACCESS_SEM_NAME, O_CREAT | O_EXCL, 0666, 1)) == SEM_FAILED) {
        printf("Error while open queue semaphore, code %d.\n", errno);
        exit(errno);
    }

    while(continuing) {
        char ch = getchar();
        switch (ch) {
            case 'w': { // Создать заполнитель.
                pthread_t threadId;
                if(pthread_create(&threadId, NULL, fillingMessages, NULL) != 0) {
                    printf("Error while creating filling thread.");
                    continuing = 0;
                } else
                    pushStack(&stackFiller, threadId);
                break;
            }
            case 's':   // Удалить заполнитель.
                if(stackFiller!=NULL) {
                    pthread_kill(stackFiller->threadId, SIGUSR1);
                    pthread_join(stackFiller->threadId, NULL);
                    popStack(&stackFiller);
                    continuing = 1;
                } else
                    printf("There is no fillers.\n");
                break;
            case 'e': { // Создать изыматель.
                pthread_t threadId;
                if(pthread_create(&threadId, NULL, extractingMessages, NULL) != 0) {
                    printf("Error while creating extracting thread.");
                    continuing = 0;
                } else
                    pushStack(&stackExtractor, threadId);
                break;
            }
            case 'd':  // Удалить изыматель.
                if(stackExtractor!=NULL) {
                    pthread_kill(stackExtractor->threadId, SIGUSR1);
                    pthread_join(stackExtractor->threadId, NULL);
                    popStack(&stackExtractor);
                    continuing = 1;
                } else
                    printf("There is no extractors.\n");
                break;
            case '+':  // Изменить размер очереди.
                MAX_MES_COUNT++;
                printf("Current size: %d\n", MAX_MES_COUNT);
                break;
            case '-':
                if(queue->countAdded - queue->countDeleted < MAX_MES_COUNT)
                    MAX_MES_COUNT--;
                printf("Current size: %d\n", MAX_MES_COUNT);
                break;
            case 'q':  // Конец.
                while(stackFiller != NULL) {
                    pthread_kill(stackFiller->threadId, SIGUSR1);
                    pthread_join(stackFiller->threadId, NULL);
                    popStack(&stackFiller);
                    continuing = 1;
                }
                while(stackExtractor != NULL) {
                    pthread_kill(stackExtractor->threadId, SIGUSR1);
                    pthread_join(stackExtractor->threadId, NULL);
                    popStack(&stackExtractor);
                    continuing = 1;
                }
                printf("All fillers are extractors are killed. End of program.\n");
                continuing = 0;
                break;
        }
    }
    // Закрыть семафоры и отсоединить их.
    sem_close(fillSem);
    sem_unlink(FILL_SEM_NAME);
    sem_close(extractSem);
    sem_unlink(EXTRACT_SEM_NAME);
    sem_close(queueAccess);
    sem_unlink(QUEUE_ACCESS_SEM_NAME);

    return 0;
}
