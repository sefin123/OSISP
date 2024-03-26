#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#define BUFFER_SIZE 1024

#pragma GCC visibility push(default) // Переключение видимости функций на значение по умолчанию
extern FILE* popen(const char* command, const char* mode) __attribute__((visibility("default")));
#pragma GCC visibility pop // Восстановление предыдущей видимости функций
// Остальной код

#pragma GCC visibility push(default)
extern int pclose(FILE* stream) __attribute__((visibility("default")));
#pragma GCC visibility pop

// Остальной код

// Добавьте следующую директиву препроцессора
#define _POSIX_C_SOURCE 200809L

void run_command(const char* command, char* buffer, size_t buffer_size) {
    FILE* pipe = popen(command, "r");
    if (pipe == NULL) {
        fprintf(stderr, "Error trying to command\n");
        return;
    }

    char* output = fgets(buffer, buffer_size, pipe);
    if (output == NULL) {
        fprintf(stderr, "Error to read command\n");
        pclose(pipe);
        return;
    }

    // Удаление символа новой строки из вывода команды, если он присутствует
    size_t output_length = strlen(output);
    if (output_length > 0 && output[output_length - 1] == '\n') {
        output[output_length - 1] = '\0';
    }

    pclose(pipe);
}

int main() {
    char user_input[BUFFER_SIZE];
    char output_buffer[BUFFER_SIZE];

    initscr(); // Инициализация ncurses

    printw("Enter command: ");
    refresh();

    getstr(user_input);

    // Удаление символа новой строки из пользовательского ввода, если он присутствует
    size_t input_length = strlen(user_input);
    if (input_length > 0 && user_input[input_length - 1] == '\n') {
        user_input[input_length - 1] = '\0';
    }

    // Проверка первого слова
    char* firstWord = strtok(user_input, " ");
    if (firstWord != NULL && strcmp(firstWord, "find") == 0) {
        // Выполнение команды только если первое слово равно "find"
        run_command(user_input, output_buffer, sizeof(output_buffer));

        clear(); // Очистка экрана

        printw("Result:\n%s\n", output_buffer);
    } else {
        printw("Invalid command. First word must be 'find'.");
    }

    getch(); // Ожидание нажатия клавиши
    endwin(); // Завершение работы с ncurses

    return 0;
}