#include "history.h"

int numHistoryResults = 0;
int selectedHistoryIndex = 0;
char* result[MAX_LENGTH];

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

int printHistory() {

    FILE* file = fopen("/home/maxim/OSISP/Spasenov/SPASENOV_OSISP_prj_2024/History.txt", "r+");
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

void deletePathFromHistory() {

    FILE* file = fopen("/home/maxim/OSISP/Spasenov/SPASENOV_OSISP_prj_2024/History.txt" ,"w");

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

char* historyHandler() {
    
    WINDOW* win;
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