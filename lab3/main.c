#include <stdio.h>
#include "methods.h"

// Вершина стека
NODE* head = NULL;

int main() {
    printf("Start of the program.\n");

    signal(SIGINT, printStat);                                                  // Сигнал вывода информации.
    signal(SIGUSR1, allow);                                                     // Сигнал разрешения вывода данных.
    signal(SIGUSR2, forbid);                                                    // Сигнал запрещения вывода данных.

    while(true) {
        printf("\nEnter + to create child.");
        printf("\nEnter - to delete last child.");
        printf("\nEnter l to show all parents and children.");
        printf("\nEnter k to delete all children.");
        printf("\nEnter s (s<num>) to allow showing for all (num) children.");
        printf("\nEnter g (g<num>) to forbid showing for all (num) children.");
        printf("\nEnter p<num> to forbid showing for all children and shows num child.");
        printf("\nEnter q to end the program.");
        printf("\nYour enter:\n");
        char ch = getchar();
        char n;

        int num = -1;
        while((n=getchar()) != '\n') {
            if (num == -1) num++;
            num *= 10;
            num += n - '0';
        }
        //printf("\nReseived num: %d\n", num);

        switch(ch) {
            case '+':
                makeFork();                                                    // Создать процесс.
                break;
            case '-':
                delLastChild();                                                 // Удалить процесс.
                break;
            case 'l':
                printf("Process P with pid = %d\n", getpid());                 // Вывести все процессы.
                show(head);
                break;
            case 'k':
                while(head)
                    delLastChild();                                             // Удалить все.
                break;
            case 's':
                sendForbid(num);                                               // Запретить.
                break;
            case 'g':
                sendAllow(num);                                                // Разрешить.
                break;
            case 'p':
                forbidAndShow(num);                                            // Вывести данные одним процессом, запретив другим.
                break;
            case 'q':
                while(head)
                    delLastChild();                                             // Конец.
                printf("All processes are deleted. End of program.\n");
                return 0;
                break;
        }
    }

    return 0;
}


