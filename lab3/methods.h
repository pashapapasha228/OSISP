#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <stdbool.h>

#define REPEAT_COUNT 21

typedef struct {
    int firstVal;
    int secondVal;
} VALUES;

extern NODE* head;

void makeFork();           // Создать процесс
void delLastChild();        // Удалить последний созданный
void sendForbid(int);      // Сигнал запрещение
void sendAllow(int);       // Сигнал разрешение
void forbid();
void allow();
void forbidAndShow(int);   // Вывести данные одного

void printStat();          // Вывод статистики
void count();              // Цикл подсчёта
void takeStatistics();     // Сбор статистики
void allowAfterP();        // Разрешение вывода всем