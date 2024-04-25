#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdbool.h>

#define MAX_LENGTH 256
#define CHILD_NAME_LENGTH 9

extern char** environ;      // Среда пользователя
size_t size_environ;        // Её размер

// Компаратор для сортировки
int compare(const void* a, const void* b) {
    return strcoll(*(const char **)a, *(const char **)b);
}

// Вывод массива строк
void print_strings(char** text, size_t length) {
    for(size_t i = 0; i < length; i++) {
        printf("\n%s", text[i]);
    }
}

// Сканирование параметров среды
char* scanEnvpi(char* envpi[], char* key) {

    char* result = (char*) calloc(MAX_LENGTH, sizeof(char));
    size_t size_envp = 0;

    while(envpi[size_envp])
        size_envp++;

    for(size_t i = 0; i < size_envp; i++) {
        if(strstr(envpi[i], key))
            strncpy(result, envpi[i]+strlen(key)+1, MAX_LENGTH);
    }

    return result;
}

int main(int argc, char* argv[], char* envp[]) {
    setlocale(LC_COLLATE, "C");

    while(environ[size_environ])
        size_environ++;

    qsort(environ, size_environ-1, sizeof(char*), compare);
    print_strings(environ, size_environ);


    char* CHILD_PATH;                       // путь до программы child
    char* child_environ_path = argv[1];     // путь до файла с переменными сред
    char child_name[CHILD_NAME_LENGTH];     // имя дочернего процесса
    size_t child_index = 0;                 // индекс вызываемого дочернего процесса
    char param = 0;                         // считанный параметр создания child
    int child_status = 0;

    while(true) {
        snprintf(child_name, CHILD_NAME_LENGTH, "%s%02d", "child_", (int) child_index);

        do {
            printf("\nEnter creation type (+ * & q): ");
            param = getchar();

            while(getchar() != '\n')
                continue;

        } while(param != '+' && param != '*' && param != '&' && param != 'q');

        if(param == 'q')
            return 0;

        child_index++;
        if(child_index>99)
            child_index = 0;

        switch (param) {
            case '+':
                CHILD_PATH = getenv("CHILD_PATH");
                break;
            case '*':
                CHILD_PATH = scanEnvpi(envp, "CHILD_PATH");
                break;
            case '&':
                CHILD_PATH = scanEnvpi(environ, "CHILD_PATH");
                break;
        }

        if(CHILD_PATH == NULL) {
            perror("Error child path find");
            exit(errno);
        }

        pid_t pid = fork();
        
        if (pid == -1) {
            perror("Error child create");
            exit(errno);
        }
        if (pid == 0) {
            printf("%s\n", CHILD_PATH);
            printf("Child process created.\n\n");
            char* child_args[] = {child_name, child_environ_path, &param, NULL};
            execve(CHILD_PATH, child_args, envp);
        }

        wait(&child_status);
    }

    return 0;
}
