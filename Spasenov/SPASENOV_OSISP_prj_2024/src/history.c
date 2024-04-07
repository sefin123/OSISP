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

void printHistory(WINDOW* win, char *result[], int *numHistoryResults) {
    FILE* historyFile = fopen("History.txt", "r+");

    char str[MAX_LENGTH];
    int i = 0;
    while (fgets(str, MAX_LENGTH, historyFile) != NULL) {
        if (str[strlen(str) - 1] == '\n') {
            str[strlen(str) - 1] = '\0';
        }
        char *res = malloc(strlen(str) + 1);
        strcpy(res, str);
        result[(*numHistoryResults)++] = res;
        mvwprintw(win, i + 3, 1, "%s", result[i]);
        i++;
    }
    fclose(historyFile);
}

void switchWord(WINDOW* win, char *result[]) {
    werase(win);
    box(win, 0, 0);
    for (int i = 0; i < numHistoryResults; i++) {
        if (i == selectedHistoryIndex) {
            wattron(win, A_STANDOUT);
        }
        //mvwprintw(win, i + 1, 1, "%s", result[i]);
        if (i == selectedHistoryIndex) {
            wattroff(win, A_STANDOUT);
        }
    }
    int terminalWidth = getmaxx(stdscr);
    int x = (terminalWidth - strlen(NAVIGATION)) / 2; 
    mvprintw(LINES - 1, x, NAVIGATION);
    refresh();
    wrefresh(win);
}

void historyHandler() {


    WINDOW* win;
    renderHistoryWindow(&win);
    printHistory(win, result, &numHistoryResults);
    while (true) {
        int ch = wgetch(win);
        
            if (ch ==KEY_F(4)) {
            return;
            }
            if (ch == KEY_UP) {
                keyUpHistoryHandler();
    printHistory(win, result, &numHistoryResults);
                switchWord(win, result);
                break;
            }
            if (ch == KEY_DOWN) {
                keyDownHistoryHandler();
    printHistory(win, result, &numHistoryResults);
                switchWord(win, result);
                break;
            }
            if (ch == 10) { //Enter
                return;
            }
            if (ch == KEY_BACKSPACE) {
                return;
            }
    }
    delwin(win);
    endwin();
}