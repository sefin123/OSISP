#include "history.h"
#include "ui.h"
#include "command.h"

int main() {
    char user_input[BUFFER_SIZE];
    char output_buffer[BUFFER_SIZE];
    
    initscr();

    while (1) {
        printw("Enter command: ");
        refresh();

        getstr(user_input);

        // Удаление символа новой строки из пользовательского ввода, если он присутствует
        size_t input_length = strlen(user_input);
        if (input_length > 0 && user_input[input_length - 1] == '\n') {
            user_input[input_length - 1] = '\0';
        }

        if (strcmp(user_input, "exit") == 0) {
            break;
        }

        run_command(user_input, output_buffer, sizeof(output_buffer));

        move(1, 0);
        printw("Result:\n%s\n", output_buffer);

        display_history();
    }

    endwin(); // Завершение работы с ncurses

    return 0;
}