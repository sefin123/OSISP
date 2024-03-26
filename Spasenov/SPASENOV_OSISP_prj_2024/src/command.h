#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>

#define BUFFER_SIZE 1024
#define HISTORY_FILE "history.txt"


#pragma GCC visibility push(default)
extern FILE* popen(const char* command, const char* mode) __attribute__((visibility("default")));
#pragma GCC visibility pop

#pragma GCC visibility push(default)
extern int pclose(FILE* stream) __attribute__((visibility("default")));
#pragma GCC visibility pop

void run_command(const char* command, char* buffer, size_t buffer_size);