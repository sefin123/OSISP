#include "history.h"

int numHistoryResults = 0;
int selectedHistoryIndex = 0;
char* result[MAX_LENGTH];

void writePath(const char* path) {

    FILE *file = fopen(HISTORY_FILE_PATH, "a");
    if (file == NULL) {
        return;
    }

    fprintf(file, "%s\n", path);

    fclose(file);
    refresh();
}

int printHistory() {

    FILE* file = fopen(HISTORY_FILE_PATH, "r+");
    numHistoryResults = 0;
    char str[MAX_LENGTH];
    int i = 0;
    
    while (fgets(str, MAX_LENGTH, file) != NULL) {
        if (str[strlen(str) - 1] == '\n') {
            str[strlen(str) - 1] = '\0';
        }
        char *res = malloc(strlen(str) + 1);
        strcpy(res, str);
        result[numHistoryResults++] = res;

        i++;
    }

    fclose(file);
    return numHistoryResults;
}

char* keyEnterHistoryHandler() {
    return result[selectedHistoryIndex];
}

void keyUpHistoryHandler() {
    if (selectedHistoryIndex > 0) {
        if (selectedHistoryIndex > numHistoryResults) selectedHistoryIndex = numHistoryResults;
        selectedHistoryIndex--;
    }
}


void keyDownHistoryHandler() {
    if (selectedHistoryIndex < numHistoryResults - 1) {
        selectedHistoryIndex++;
    }
}

void deletePathFromHistory() {

    FILE* file = fopen(HISTORY_FILE_PATH ,"w");

    numHistoryResults--;
    if (selectedHistoryIndex < numHistoryResults) {
        for (int i = selectedHistoryIndex; i < numHistoryResults; i++) {
            result[i] = result[i + 1];
        }
    }

    for (int i = 0; i < numHistoryResults; i++) {
            fprintf(file, "%s\n", result[i]);
    }
    
    fclose(file);
    
}

void keyBackspaseHistoryHandler() {
    deletePathFromHistory();
}

WINDOW* createHistoryWindow() {
    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);
    WINDOW *win = newwin(windowHeight, windowWidth, 0, 0);
    keypad(win, TRUE);
    
    return win;
}

char* historyHandler() {
    
    WINDOW* win = createHistoryWindow();
    numHistoryResults = printHistory();
    renderHistoryWindow(&win, result, numHistoryResults, selectedHistoryIndex);

    while (true) {
        int ch = wgetch(win);
        
            if (ch ==KEY_F(4)) {
            return "";
            }
            if (ch == KEY_UP) {
                keyUpHistoryHandler();
            }
            if (ch == KEY_DOWN) {
                keyDownHistoryHandler();
            }
            if (ch == 10) { //Enter
                return keyEnterHistoryHandler();
            }
            if (ch == KEY_BACKSPACE) {
                keyBackspaseHistoryHandler();
            }
        renderHistoryWindow(&win, result, numHistoryResults, selectedHistoryIndex);
        numHistoryResults = printHistory();  
    }
    delwin(win);
    endwin();
}