#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>

enum PARAMETERS {
    L,  // Символические ссылки
    D,  // Каталоги
    F,  // Файлы
    S,   // Сортировка
    LDF // Каталоги, файлы и ссылки
};

// Какие сейчас выбраны
bool parameters[5] = {false, false, false, false, false};


// Структура для хранения считанного
typedef struct array {
    char** buffer;
    size_t capacity;
    size_t cur_size;
} array;

// Перевыделение памяти
void resize_array(array *arr) {
    arr->capacity *= 2;
    arr->buffer = (char**)realloc(arr->buffer, arr->capacity * sizeof(char*));
}

// Добавление нового элемента
void add_array(array* arr, char* str) {
    if(arr->cur_size+1 == arr->capacity)
        resize_array(arr);

    arr->buffer[arr->cur_size] = (char*)malloc(strlen(str) + 1);
    strncpy(arr->buffer[arr->cur_size++], str, strlen(str) + 1);
}

// Компаратор сортировки по алфавиту
int abc_compare(const void* first, const void* second) {
    return strcoll(*(const void**)first, *(const void**)second);
}

// Сортировка по алфавиту
void abc_sort(array* arr) {
    qsort(arr->buffer, arr->cur_size, sizeof(char*), abc_compare);
}

// Вывод массива на экран
void print_array(array* arr) {
    printf("\n");

    for (int i = 0; i < arr->cur_size; i++) {
        printf("%s\n", arr->buffer[i]);
    }
}

// Освобождение памяти
void free_array(array* arr) {
    for(int i = 0; i < arr->cur_size; i++)
        free(arr->buffer[i]);
    free(arr->buffer);
}

// Добавление пути к уже существующему
const char* add_path(const char* current, const char* new) {
    char* res = (char*) malloc(strlen(current) + strlen(new) + 2);

    strcpy(res, current);

    res[strlen(current)] = '/';

    strcat(res, new);

    res[strlen(current) + strlen(new) + 1] = '\0';
    return res;
}

// Просмотр содержимого каталога
void look_dir(const char* path, array* arr) {
    DIR *dir = opendir(path);                   // Открыть каталог.
    if (!dir) {                                            // Проверка успешности открытия.
        perror("opendir");
        exit(errno);
    }
    struct dirent* current_dir;

    while(true) {
        current_dir = readdir(dir);
        if (current_dir == NULL) {
            break;
        }

        if(!strcmp(current_dir->d_name, ".") || !strcmp(current_dir->d_name, ".."))
            continue;

        switch(current_dir->d_type) {
            case DT_DIR:
                if(parameters[D] || parameters[LDF]) {
                    add_array(arr, add_path(path, current_dir->d_name));
                }
                break;
            case DT_LNK:
                if(parameters[L] || parameters[LDF]) {
                    add_array(arr, add_path(path, current_dir->d_name));
                }
                break;
            case DT_REG:
                if(parameters[F] || parameters[LDF]) {
                    add_array(arr, add_path(path, current_dir->d_name));
                }
                break;
        }
    }

    if(parameters[S])
        abc_sort(arr);

    closedir(dir);
}

// Получение каталога из командной строки
const char* get_path(const int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if(argv[i][0] != '-')
            return argv[i];
    }

    return ".";
}

// Поиск параметров из командной строки
void find_params(int argc, char* argv[]) {
    int rez;

    while((rez = getopt(argc, argv, "ldfs")) != -1) {
        switch (rez) {
            case 'l':
                parameters[L] = true;
                break;
            case 'd':
                parameters[D] = true;
                break;
            case 'f':
                parameters[F] = true;
                break;
            case 's':
                parameters[S] = true;
                break;
            case '?':
                perror("Ошибка! Неизвестный параметр.");
                exit(0);
                break;
        }
    }

    if(!parameters[L] && !parameters[D] && !parameters[F])
        parameters[LDF] = true;

    return;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    char* path;
    array data = {NULL, 1, 0};

    path = get_path(argc, argv);
    find_params(argc, argv);

    look_dir(path, &data);

    print_array(&data);

    free_array(&data);

    return 0;
}
