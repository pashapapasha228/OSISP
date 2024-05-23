#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_MES_COUNT 20

typedef struct {
    u_int8_t type;          // тип сообщения
    u_int16_t hash;         // контрольные данные
    u_int8_t size;          // длинна данных
    u_int8_t data[256];     // сообщение
} MESSAGE;

typedef struct node {
    uintptr_t message;
    uintptr_t next;
    uintptr_t prev;
} NODE;

typedef struct {
    int countAdded;
    int countDeleted;
    uintptr_t ringHead;             // голова
    uintptr_t ringTail;             // хвост
    uintptr_t currentPlaceToWrite;  //
} QUEUE;

#define START_PLACE_TO_WRITE sizeof(QUEUE)
#define END_PLACE_TO_WRITE (sizeof(QUEUE)+MAX_MES_COUNT*sizeof(NODE)+MAX_MES_COUNT*sizeof(MESSAGE))
#define STEP_TO_WRITE (sizeof(NODE) + sizeof(MESSAGE))

void push(QUEUE*);
void pop(QUEUE*);

void initMes(MESSAGE*);
void printMes(MESSAGE*);