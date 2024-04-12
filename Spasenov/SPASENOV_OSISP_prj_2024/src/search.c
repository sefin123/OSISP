#include "search.h"

Parametrs* parametr;
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

void saveResult(const char *fullPath, const char *input, const char *results[],
                 int *numResults) {
    if (strstr(fullPath, input) != NULL) {
        char *result = malloc(strlen(fullPath) + 1);
        strcpy(result, fullPath);
        results[(*numResults)++] = result;
    }
}

void processSizeFlag(const char *filePath, const char *input, const char *results[],
                     int *numResults, Parametrs *param) {
    FILE* file = fopen(filePath, "r");

    if (file == NULL) {
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    fclose(file);

    if (param->sizeFileFlag->isMore && fileSize > param->sizeFileFlag->size) {
        saveResult(filePath, input, results, numResults);
    }

    if (!param->sizeFileFlag->isMore && fileSize < param->sizeFileFlag->size) {
        saveResult(filePath, input, results, numResults);
    }
}

void processTimeFlag(const char *filePath, const char *input, const char *results[],
                     int *numResults, Parametrs *param) {
    struct stat fileStat;
    if (lstat(filePath, &fileStat) == 0) {
        struct tm *modificationTime = localtime(&fileStat.st_mtime);

        if (param->timeFileFlag->isMore && mktime(modificationTime) > param->timeFileFlag->time) {
            saveResult(filePath, input, results, numResults);
        }

        if (!param->timeFileFlag->isMore && mktime(modificationTime) < param->timeFileFlag->time) {
            saveResult(filePath, input, results, numResults);
        }
    }

}

void processEntry(const char *fullPath, const char *input, const char *results[],
                  int *numResults, Parametrs *param) {
    struct stat fileStat;
    if (lstat(fullPath, &fileStat) == 0) {
        if (param->sizeFileFlag->isEnable) {
            processSizeFlag(fullPath, input, results, numResults, param);
        } else if (param->timeFileFlag->isEnable) {
            processTimeFlag(fullPath, input, results, numResults, param);
        } else {
            saveResult(fullPath, input, results, numResults);
        }
    }
}

void processEmptyFile(const char *fullPath, const char *input, const char *results[],
                 int *numResults) {
    FILE* file = fopen(fullPath, "r");

    if (file == NULL) {
        fprintf(stderr, "asdasdasdasdasd\n");
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    fclose(file);
    if (fileSize == 0) {
        saveResult(fullPath, input, results, numResults);
    }
    
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

            char fullPath[PATH_MAX];
            snprintf(fullPath, PATH_MAX, "%s/%s", path, dir[n]->d_name);

            struct stat fileStat;
            if (lstat(fullPath, &fileStat) == 0) {
                if (parametr->fileFlag && S_ISREG(fileStat.st_mode)) {
                    processEntry(fullPath, input, results, numResults, parametr);
                }
                if (parametr->directoryFlag && S_ISDIR(fileStat.st_mode)) {
                    processEntry(fullPath, input, results, numResults, parametr);
                }
                if (parametr->symlinkFlag && S_ISLNK(fileStat.st_mode)) {
                    processEntry(fullPath, input, results, numResults, parametr);
                }
                if (parametr->emptyFlag && S_ISREG(fileStat.st_mode)) {
                    processEmptyFile(fullPath, input, results, numResults);
                }
            }
            if (S_ISDIR(fileStat.st_mode)) {
                saveDirectory(fullPath, input, results, numResults);
            }

            free(dir[n]);
        }

        free(dir);
    }
}

int printDirectory(WINDOW *searchWin, WINDOW *resultsWin, const char *input,
                    const char *path, const char *results[], int selectedIndex,
                     int cousorPosition) {

    int numResults = 0;
    saveDirectory(path, input, results, &numResults);

    qsort(results, numResults, sizeof(const char *), compareFilenames);

    renderWriteWindow(&searchWin, input, cousorPosition);
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
    strcpy(input, historyHandler());
    cursorPosition = (int)strlen(input);
    inputLength = cursorPosition;
}

void keyF1Handler() {
    parametr = parametrsHandler(parametr);
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

Parametrs* initParametrs() {
    parametr = allocateMemory();

    parametr->directoryFlag = true;
    parametr->fileFlag = true;
    parametr->symlinkFlag = true;
    parametr->sizeFileFlag->isMore = true;
    parametr->timeFileFlag->isMore = true;

    return parametr;
}

void handleInput(WINDOW *searchWin, WINDOW *resultsWin) {
    
    parametr = initParametrs();

    numResults = printDirectory(searchWin, resultsWin, input,
                                path, results, 0,
                                cursorPosition);

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
                renderResultsWindow(resultsWin, results,
                                    numResults, selectedIndex);
                renderWriteWindow(&searchWin, input,
                                  cursorPosition);
                break;   
            }
            case KEY_F(2): {
                keyF2Handler();
                break;
            }
            case KEY_F(1): {
                keyF1Handler();
                renderResultsWindow(resultsWin, results,
                                    numResults, selectedIndex);

                renderWriteWindow(&searchWin, input,
                                  cursorPosition);
                break;
            }
            default: {
                writeHandler(ch);
            break;
            }
        }
        renderResultsWindow(resultsWin, results, numResults, selectedIndex);
        renderWriteWindow(&searchWin, input, cursorPosition);
        numResults = printDirectory(searchWin, resultsWin, input,
                                    path, results, selectedIndex,
                                    cursorPosition);
    }
}