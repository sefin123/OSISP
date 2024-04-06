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

void search_directory(WINDOW *search_win, WINDOW *results_win, const char *input, const char *path, const char *results[]) {
    int num_results = 0;
    printDirectory(path, input, results, &num_results);

    qsort(results, num_results, sizeof(const char *), compare_filenames);

    render_search_window(search_win, input);
    render_results_window(results_win, results, num_results);
}

void handle_input(WINDOW *search_win, WINDOW *results_win) {
    char input[MAX_LENGTH];
    int input_length = 0;
    int selected_index = 0;
    char *results[MAX_LENGTH];
    int num_results = 0;
    char path[MAX_LENGTH] = ".";

    search_directory(search_win, results_win, input, path, results);

    while (true) {
        int ch = wgetch(search_win);
        switch (ch) {
            case 27:  // Escape key
                return;
            case KEY_BACKSPACE:
                if (input_length > 0) {
                    input[--input_length] = '\0';
                }
                break;
            case KEY_DOWN:
                if (selected_index < num_results - 1) {
                    selected_index++;
                    render_results_window(results_win, results, num_results);
                }
                break;
            case KEY_UP:
                if (selected_index > 0) {
                    selected_index--;
                    render_results_window(results_win, results, num_results);
                }
                break;
            default:
                if (ch >= 32 && ch <= 126 && input_length < MAX_LENGTH - 1) {
                    input[input_length++] = ch;
                    input[input_length] = '\0';
                }
                break;
        }

        search_directory(search_win, results_win, input, path, results);
    }
}