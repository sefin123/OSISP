#include "history.h"

void display_history() {
    FILE* history_file = fopen(HISTORY_FILE, "r");
    if (history_file == NULL) {
        fprintf(stderr, "Error opening history file\n");
        return;
    }

    char line[BUFFER_SIZE];
    int line_number = 1;

    while (fgets(line, sizeof(line), history_file) != NULL) {
        move(line_number, 0);
        printw("%s", line);
        line_number++;
    }

    fclose(history_file);
}