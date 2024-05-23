#include "methods.h"

VALUES values = {0, 0};
bool makeValues = true;
int _0_0 = 0;
int _0_1 = 0;
int _1_0 = 0;
int _1_1 = 0;
bool canShow = true;

void makeFork() {
    pid_t chpid = fork();                                      // Создание процесса.
    if(chpid == 0) {
        signal(SIGALRM, takeStatistics);                       // Обработчик сигнала будильника.
        count();
    } else if (chpid == -1) {
        printf("Error occured, error code - %d\n", errno);     // Ошибка.
        exit(errno);
    } else {
        push(&head, chpid);                                    // Сохранение процесса.
        printf("Child process with pid = %d created.\n", chpid);
    }
}

void delLastChild() {
    kill(head->pid, SIGKILL);                                  // Сигнал завершения процессу.
    printf("Process C_%d with pid = %d killed.\n", head->num, head->pid);
    pop(&head);
}

void forbid() {
    canShow = true;
}

void allow() {
    canShow = false;
}

void sendForbid(int procNum) {
    if (procNum == -1) {
        NODE *temp = head;
        while (temp) {
            kill(temp->pid, SIGUSR2);                          // Сигнал запрщения вывода всем.
            temp = temp->next;
        }
    } else {
        NODE *temp = head;
        while (temp && temp->pid != procNum)
            temp = temp->next;
        if (temp)
            kill(temp->pid, SIGUSR2);                          // Сигнал запрещения вывода одному.
    }
}
void sendAllow(int procNum) {
    if (procNum == -1) {
        NODE *temp = head;
        while (temp) {
            kill(temp->pid, SIGUSR1);                          // Сигнал разрешения вывода всем.
            temp = temp->next;
        }
    } else {
        NODE *temp = head;
        while (temp && temp->pid != procNum)
            temp = temp->next;
        if (temp)
            kill(temp->pid, SIGUSR1);                          // Сигнал разрешения вывода одному.
    }
}

void forbidAndShow(int procNum) {
    sendForbid(-1);                                       // Запретить всем.
    NODE *temp = head;
    while (temp && temp->pid != procNum)
        temp = temp->next;
    if (temp)
        kill(temp->pid, SIGINT);                               // Вывести данные указанного процесса.

    signal(SIGALRM, allowAfterP);
    alarm(5);                                              // Таймер чтобы разрешить обратно вывод.
}

void printStat() {
    printf("Process pid = %d, ppid = %d, {0,0} - %d, {0,1} - %d, {1,0} - %d, {1,1} - %d\n",
           getpid(), getppid(), _0_0, _0_1, _1_0, _1_1);
}

void count() {
    int repeat = 0;

    while(1) {
        if(repeat == REPEAT_COUNT) {
            repeat = 0;
            if (canShow)
                printStat();
        }
        alarm(1);

        while (makeValues) {
            if (values.firstVal == 0 && values.secondVal == 0)
                values.secondVal = 1;
            else if (values.firstVal == 0 && values.secondVal == 1) {
                values.firstVal = 1;
                values.secondVal = 0;
            } else if (values.firstVal == 1 && values.secondVal == 0)
                values.secondVal = 1;
            else {
                values.firstVal = 0;
                values.secondVal = 0;
            }
        }
        makeValues = true;
        repeat++;
    }
}

void takeStatistics() {
    if (values.firstVal == 0 && values.secondVal == 0)
        _0_0++;
    else if (values.firstVal == 0 && values.secondVal == 1)
        _0_1++;
    else if (values.firstVal == 1 && values.secondVal == 0)
        _1_0++;
    else
        _1_1++;

    makeValues = false;
}

void allowAfterP() {
    sendAllow(-1);
}