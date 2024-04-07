#include "search.h"

char input[MAX_LENGTH] = "";
int inputLength = 0;
int selectedIndex = 0;
const char *results[MAX_LENGTH];
int numResults = 0;
char path[MAX_LENGTH] = ".";
int cursorPosition = 0;

int compareFilenames(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void saveDirectory(const char *path, const char *input, const char *results[], int *numResults) {
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
                saveDirectory(newPath, input, results, numResults);
            }

            free(dir[n]);
        }

        free(dir);
    }
}

int printDirectory(WINDOW *searchWin, WINDOW *resultsWin, const char *input, const char *path, const char *results[], int selectedIndex, int cousorPosition) {
    int numResults = 0;
    saveDirectory(path, input, results, &numResults);

    qsort(results, numResults, sizeof(const char *), compareFilenames);

    renderSearchWindow(searchWin, input, cousorPosition);
    renderResultsWindow(resultsWin, results, numResults, selectedIndex);
    
    return numResults;
}

void writePath(const char* path) {

    FILE *file = fopen("History.txt", "a");
    if (file == NULL) {
        perror("Cant open file");
        return;
    }

    fprintf(file, "%s\n", path);

    fclose(file);
    refresh();
}

void keyDownHandler(WINDOW* win) {
    if (selectedIndex < numResults - 1) {
        selectedIndex++;
        renderResultsWindow(win, results, numResults, selectedIndex);
    }
}

void keyUpHandler(WINDOW* win) {
    if (selectedIndex > 0) {
        if (selectedIndex > numResults) selectedIndex = numResults;
        selectedIndex--;
        renderResultsWindow(win, results, numResults, selectedIndex);
    }
}

void keyBackspaseHandler() {
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
}

void keyRightHandler() {
    if (cursorPosition < inputLength) {
        cursorPosition++;
    }
}

void keyLeftHandler() {
    if (cursorPosition > 0) {
        cursorPosition--;
    }
}

void keyEnterHandler() {
    writePath(results[selectedIndex]);
    strcpy(input, results[selectedIndex]);
    inputLength = (int)strlen(results[selectedIndex]);
    cursorPosition = inputLength;
}

void keyF3Handler() {
    aboutHandler();
}

void keyF2Handler() {
    historyHandler();
}

void keyF1Handler() {
    parametrsHandler();
}

void writeHandler(int ch) {
    if (ch >= 32 && ch <= 126 && inputLength < MAX_LENGTH - 1) {
        for (int i = inputLength; i > cursorPosition; i--) {
            input[i] = input[i - 1];
        }
        input[cursorPosition++] = ch;
        inputLength++;
        input[inputLength] = '\0';
    }
}

void handleInput(WINDOW *searchWin, WINDOW *resultsWin) {

    numResults = printDirectory(searchWin, resultsWin, input, path, results, 0, cursorPosition);

    while (true) {

        int ch = wgetch(searchWin);
        switch (ch) {
            case KEY_F(4):
                return;
            case KEY_BACKSPACE: {
                keyBackspaseHandler();
            break;
            }
            case KEY_DOWN: {
                keyDownHandler(resultsWin);
            break;
            }
            case KEY_UP: {
                keyUpHandler(resultsWin);
            break;
            }
            case KEY_LEFT: {
                keyLeftHandler();
            break;
            }
            case KEY_RIGHT: {
                keyRightHandler();
            break;
            }
            case 10: { //Enter
                keyEnterHandler();
            break;
            }
            case KEY_F(3): {
                keyF3Handler();
                renderResultsWindow(resultsWin, results, numResults, selectedIndex);
                renderSearchWindow(searchWin, input, cursorPosition);
                break;   
            }
            case KEY_F(2): {
                keyF2Handler();
                break;
            }
            case KEY_F(1): {
                keyF1Handler();
                renderResultsWindow(resultsWin, results, numResults, selectedIndex);
                renderSearchWindow(searchWin, input, cursorPosition);
                break;
            }
            default: {
                writeHandler(ch);
            break;
            }
        }
        renderResultsWindow(resultsWin, results, numResults, selectedIndex);
        renderSearchWindow(searchWin, input, cursorPosition);
        numResults = printDirectory(searchWin, resultsWin, input, path, results, selectedIndex, cursorPosition);
    }
}