#include "search.h"

Parametrs* param;
WINDOW* resultsWin;
WINDOW* searchWin;
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
        for (int i = 0; i < *numResults; i++) {
            if (strcmp(results[i], fullPath) == 0) {
                return;
            }
        }

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

void processRegex(const char *filePath, const char *input, const char *results[],
                  int *numResults, Parametrs *param) {
    regex_t regex;
    int ret = regcomp(&regex, (const char*)param->regexFlag->pattern, REG_EXTENDED | REG_NOSUB);
    if (ret != 0) {
        return;
    }

    ret = regexec(&regex, filePath, 0, NULL, 0);
    if (ret == 0) {
        saveResult(filePath, input, results, numResults);
    }

    regfree(&regex);
}

void processEntry(const char *fullPath, const char *input, const char *results[],
                  int *numResults, Parametrs *param) {
    struct stat fileStat;
    if (lstat(fullPath, &fileStat) == 0) {
        if (param->sizeFileFlag->isEnable) {
            processSizeFlag(fullPath, input, results, numResults, param);
        } else if (param->timeFileFlag->isEnable) {
            processTimeFlag(fullPath, input, results, numResults, param);
        } else if (param->regexFlag->isEnable) {
            processRegex(fullPath, input, results, numResults, param);
        } else {
            saveResult(fullPath, input, results, numResults);
        }
    }
}

void processEmptyFile(const char *fullPath, const char *input, const char *results[],
                 int *numResults) {
    FILE* file = fopen(fullPath, "r");

    if (file == NULL) {
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);

    fclose(file);
    if (fileSize == 0) {
        saveResult(fullPath, input, results, numResults);
    }
    
}

void saveDirectory(const char *path, const char *input, const char *results[],
                     int *numResults) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(path);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char fullPath[PATH_MAX];
        snprintf(fullPath, PATH_MAX, "%s/%s", path, entry->d_name);

        struct stat fileStat;
        if (lstat(fullPath, &fileStat) == 0) {
            if (param->fileFlag && S_ISREG(fileStat.st_mode)) {
                processEntry(fullPath, input, results, numResults, param);
            }
            if (param->directoryFlag && S_ISDIR(fileStat.st_mode)) {
                processEntry(fullPath, input, results, numResults, param);
            }
            if (param->symlinkFlag && S_ISLNK(fileStat.st_mode)) {
                processEntry(fullPath, input, results, numResults, param);
            }
            if (param->emptyFlag && S_ISREG(fileStat.st_mode)) {
                processEmptyFile(fullPath, input, results, numResults);
            }
        }
        if (S_ISDIR(fileStat.st_mode)) {
            saveDirectory(fullPath, input, results, numResults);
        }
    }

    closedir(dir);
}

int printDirectory(const char *input, const char *path, const char *results[], int selectedIndex, int cursorPosition) {

    int numResults = 0;
    saveDirectory(path, input, results, &numResults);

    qsort(results, numResults, sizeof(const char *), compareFilenames);

    renderWriteWindow(&searchWin, input, cursorPosition);
    renderResultsWindow(&resultsWin, results, numResults, selectedIndex);
    
    return numResults;
}

void keyDownHandler() {
    if (selectedIndex < numResults - 1) {
        selectedIndex++;
    }
}

void keyUpHandler() {
    if (selectedIndex > 0) {
        if (selectedIndex > numResults) selectedIndex = numResults;
        selectedIndex--;
    }
}

void keyBackspaseHandler() {
    if (cursorPosition == 0) { 
        return;
    }
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
    param = parametrsHandler(param);
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

WINDOW* createSearchWindow() {
    int windowWidth;
    windowWidth = getmaxx(stdscr);
    WINDOW* win = newwin(search_win_height, windowWidth, 0, 0);
    keypad(win, TRUE);

    return win;
}

WINDOW* createResultsWindow() {
    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);

    WINDOW* win = newwin(windowHeight - search_win_height, windowWidth, search_win_height, 0);
    keypad(win, TRUE);

    return win;
}

void handleInput() {

    searchWin = createSearchWindow();
    resultsWin = createResultsWindow();
    
    renderResultsWindow(&resultsWin, results, numResults, selectedIndex);

    param = allocateMemory();

    numResults = printDirectory(input, path, results, 0, cursorPosition);

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
                keyDownHandler();
            break;
            }
            case KEY_UP: {
                keyUpHandler();
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
                break;   
            }
            case KEY_F(2): {
                keyF2Handler();
                break;
            }
            case KEY_F(1): {
                keyF1Handler();
                break;
            }
            default: {
                writeHandler(ch);
            break;
            }
        }
        renderResultsWindow(&resultsWin, results, numResults, selectedIndex);
        renderWriteWindow(&searchWin, input, cursorPosition);
        numResults = printDirectory(input, path, results, selectedIndex, cursorPosition);
    }
    delwin(searchWin);
    delwin(resultsWin);
}
