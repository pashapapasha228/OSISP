#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 256
#define ENV_COUNT 9

extern char** environ;      // Среда пользователя

char** readFile(char* filename) {
    FILE *f = NULL;
    if ((f = fopen(filename, "r")) != NULL) {
        char** result = (char**) calloc(ENV_COUNT, sizeof(char *));
        for (int i = 0; i < ENV_COUNT; i++) {
            result[i] = (char*)calloc(80, sizeof(char));
            fgets(result[i], 80, f);                                        // Чтение всех строк.
            result[i][strlen(result[i])-1] = '\0';
        }

        fclose(f);
        return result;
    }

    perror("Error while open file.");
    return NULL;
}

int main(int argc, char* argv[], char* envp[]) {
    char** from_file;
    char* temp;

    printf("Child process: \n\tname: %s\n\tpid: %d\n\tppid: %d\n\tenvironment:\n", argv[0], getpid(), getppid());

    from_file = readFile(argv[1]);

    switch(argv[2][0]) {
        case '+':
            for (size_t i = 0; i < ENV_COUNT; i++) {
                printf("%s", from_file[i]);
                printf("=%s\n", getenv(from_file[i]));
            }

            break;
        case '*':
            for (int i = 0; envp[i]; i++) {
                for (int j = 0; j < ENV_COUNT; j++) {
                    temp = strstr(envp[i], from_file[j]);

                    if(temp != NULL && temp[strlen(from_file[j])] == '=') {
                        printf("%s\n", temp);
                        break;
                    }
                }
            }
            break;
        case '&':
            for (int i = 0; environ[i]; i++) {
                for (int j = 0; j < ENV_COUNT; j++) {
                    temp = strstr(environ[i], from_file[j]);

                    if(temp != NULL && temp[strlen(from_file[j])] == '=') {
                        printf("%s\n", temp);
                        break;
                    }
                }
            }
            break;
        default:
            printf("Error! No parameter.");
            break;
    }

    return 0;
}