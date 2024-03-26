#include "command.h"

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

    // Запись команды и результата выполнения в файл истории
    FILE* history_file = fopen(HISTORY_FILE, "a");
    if (history_file == NULL) {
        fprintf(stderr, "Error opening history file\n");
        return;
    }

    fprintf(history_file, "%s\n", output);

    fclose(history_file);
}
