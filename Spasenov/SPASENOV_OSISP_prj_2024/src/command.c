#include "command.h"

char input[MAX_LENGTH] = "";
int inputLength = 0;
int selectedIndex = 0;
const char *results[MAX_LENGTH];
int numResults = 0;
char path[MAX_LENGTH] = ".";
int cursorPosition = 0;
int currentWindow = 1;

int compareFilenames(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void printDirectory(const char *path, const char *input, const char *results[], int *numResults) {
    struct dirent **dir;
    int n = scandir(path, &dir, NULL, NULL);

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
                    results[(*numResults)++] = result;
                }
            }

            if (dir[n]->d_type == DT_DIR) {
                char newPath[PATH_MAX];
                snprintf(newPath, PATH_MAX, "%s/%s", path, dir[n]->d_name);
                printDirectory(newPath, input, results, numResults);
            }

            free(dir[n]);
        }

        free(dir);
    }
}

int searchDirectory(WINDOW *searchWin, WINDOW *resultsWin, const char *input, const char *path, const char *results[], int selectedIndex, int cousorPosition) {
    int numResults = 0;
    printDirectory(path, input, results, &numResults);

    qsort(results, numResults, sizeof(const char *), compareFilenames);

    renderSearchWindow(searchWin, input, cousorPosition);
    renderResultsWindow(resultsWin, results, numResults, selectedIndex);
    
    return numResults;
}

void handleInput(WINDOW *searchWin, WINDOW *resultsWin) {

    numResults = searchDirectory(searchWin, resultsWin, input, path, results, 0, cursorPosition);

    while (true) {

        int ch = wgetch(searchWin);
        switch (ch) {
            case 27:  // Escape key
                return;
            case KEY_BACKSPACE:
                if (inputLength > 0) {
                    for (int i = cursorPosition; i < inputLength; i++) {
                        input[i - 1] = input[i];
                    }
                    inputLength--;
                    cursorPosition--;
                    if(cursorPosition == -1) cursorPosition = 0;
                    if(selectedIndex > numResults) selectedIndex = numResults;
                    input[inputLength] = '\0';
                }
            break;
            case KEY_DOWN:
                if (selectedIndex < numResults - 1) {
                    selectedIndex++;
                    renderResultsWindow(resultsWin, results, numResults, selectedIndex);
                }
            break;
            case KEY_UP:
                if (selectedIndex > 0) {
                    if (selectedIndex > numResults) selectedIndex = numResults;
                    selectedIndex--;
                    renderResultsWindow(resultsWin, results, numResults, selectedIndex);
                }
            break;
            case KEY_LEFT:
                if (cursorPosition > 0) {
                    cursorPosition--;
                }
            break;
            case KEY_RIGHT:
                if (cursorPosition < inputLength) {
                    cursorPosition++;
                }
            break;
            case KEY_F(3): {
                WINDOW *infoWin;
                renderInfoWindow(&infoWin);

                keypad(infoWin, TRUE);

                int simbol;
                while (true) {
                    simbol = wgetch(infoWin);
                    if (simbol == KEY_F(1)) {
                    break;
                    }
                }
                renderResultsWindow(resultsWin, results, numResults, selectedIndex);
                renderSearchWindow(searchWin, input, cursorPosition);
                delwin(infoWin);
            }
            break;   
            default:
                if (ch >= 32 && ch <= 126 && inputLength < MAX_LENGTH - 1) {
                    for (int i = inputLength; i > cursorPosition; i--) {
                        input[i] = input[i - 1];
                    }
                    input[cursorPosition++] = ch;
                    inputLength++;
                    input[inputLength] = '\0';
                }
            break;
        renderResultsWindow(resultsWin, results, numResults, selectedIndex);
        renderSearchWindow(searchWin, input, cursorPosition);
        }
        numResults = searchDirectory(searchWin, resultsWin, input, path, results, selectedIndex, cursorPosition);
    }
}
