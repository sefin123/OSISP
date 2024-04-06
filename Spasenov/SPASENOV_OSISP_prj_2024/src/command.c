#include <dirent.h>
#include "command.h"

int compare_filenames(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int compare(const struct dirent **a, const struct dirent **b) {
    return strcmp((*a)->d_name, (*b)->d_name);
}

void printDirectory(const char *path, const char *input, char *results[], int *num_results) {
    struct dirent **dir;
    int n = scandir(path, &dir, NULL, compare);

    if (n != -1) {
        while (n--) {
            if (strcmp(dir[n]->d_name, ".") == 0 || strcmp(dir[n]->d_name, "..") == 0) {
                free(dir[n]);
                continue;
            }

            if (dir[n]->d_type == DT_REG || dir[n]->d_type == DT_DIR || dir[n]->d_type == DT_LNK) {
                char fullPath[PATH_MAX];
                snprintf(fullPath, PATH_MAX, "%s/%s", path, dir[n]->d_name);

                if (strstr(fullPath, input) != NULL) {
                    char *result = malloc(strlen(fullPath) + 1);
                    strcpy(result, fullPath);
                    results[(*num_results)++] = result;
                }
            }

            if (dir[n]->d_type == DT_DIR) {
                char newPath[PATH_MAX];
                snprintf(newPath, PATH_MAX, "%s/%s", path, dir[n]->d_name);
                printDirectory(newPath, input, results, num_results);
            }

            free(dir[n]);
        }

        free(dir);
    }
}

int search_directory(WINDOW *search_win, WINDOW *results_win, const char *input, const char *path, const char *results[], int selected_index, int cousor_position) {
    int num_results = 0;
    printDirectory(path, input, results, &num_results);

    qsort(results, num_results, sizeof(const char *), compare_filenames);

    render_search_window(search_win, input, cousor_position);
    render_results_window(results_win, results, num_results, selected_index);
    
    return num_results;
}

void handle_input(WINDOW *search_win, WINDOW *results_win) {
    char input[MAX_LENGTH];
    int input_length = 0;
    int selected_index = 0;
    char *results[MAX_LENGTH];
    int num_results = 0;
    char path[MAX_LENGTH] = ".";
    int cursor_position = 0;

    num_results = search_directory(search_win, results_win, input, path, results, 0, cursor_position);

    while (true) {
        int ch = wgetch(search_win);
        switch (ch) {
            case 27:  // Escape key
                return;
            case KEY_BACKSPACE:
                if (input_length > 0) {
                    for (int i = cursor_position; i < input_length; i++) {
                        input[i - 1] = input[i];
                    }
                    input_length--;
                    cursor_position--;
                    input[input_length] = '\0';
                }
                break;
            case KEY_DOWN:
                if (selected_index < num_results - 1) {
                selected_index++;
                render_results_window(results_win, results, num_results, selected_index);
                }
                break;
            case KEY_UP:
                if (selected_index > 0) {
                selected_index--;
                render_results_window(results_win, results, num_results, selected_index);
                }
                break;
            case KEY_LEFT:
                if (cursor_position > 0) {
                    cursor_position--;
                }
                break;
            case KEY_RIGHT:
                if (cursor_position < input_length) {
                    cursor_position++;
                }
            default:
                if (ch >= 32 && ch <= 126 && input_length < MAX_LENGTH - 1) {
                    for (int i = input_length; i > cursor_position; i--) {
                        input[i] = input[i - 1];
                    }
                    input[cursor_position++] = ch;
                    input_length++;
                    input[input_length] = '\0';
                    render_search_window(search_win, input, cursor_position);
                }
                break;

        render_search_window(search_win, input, cursor_position);
        }
            num_results = search_directory(search_win, results_win, input, path, results, selected_index, cursor_position);

    }
}
