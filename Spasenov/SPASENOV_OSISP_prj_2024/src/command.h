#include "gui.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_LENGTH 256

#define DT_DIR 4

void handle_input(WINDOW *search_win, WINDOW *results_win);

void search_directory(WINDOW *search_win, WINDOW *results_win, const char *input, const char *search_dir, const char *results[]) {