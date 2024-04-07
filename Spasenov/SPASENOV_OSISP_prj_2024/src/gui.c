#include "gui.h"

void renderSearchWindow(WINDOW *searchWin, const char *searchQuery, int cursorPosition) {
    werase(searchWin);
    box(searchWin, 0, 0);
    for (int i = 0; i < (int)strlen(searchQuery); i++) {
        if (i == cursorPosition) {
            wattron(searchWin, A_STANDOUT);
        }
        mvwprintw(searchWin, 1, 1 + i, "%c", searchQuery[i]);
        if (i == cursorPosition) {
            wattroff(searchWin, A_STANDOUT);
        }
    }

    if (cursorPosition == (int)strlen(searchQuery)) {
        wattron(searchWin, A_STANDOUT);
        mvwprintw(searchWin, 1, strlen(searchQuery) + 1, " ");
        wattroff(searchWin, A_STANDOUT);
    }

    wmove(searchWin, 1, 1 + cursorPosition);
    wrefresh(searchWin);
}

void renderInfoWindow(WINDOW **infoWin) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    int infoWinHeight = rows - 2;
    int infoWinWidth = cols - 2;
    int infoWinY = 1;
    int infoWinX = 1;

    *infoWin = newwin(infoWinHeight, infoWinWidth, infoWinY, infoWinX);
    box(*infoWin, 0, 0);

    wclear(*infoWin);
    box(*infoWin, 0, 0);

    int textLength = strlen(ABOUT_PROGRAMM);
    int positionX = (infoWinWidth - textLength) / 2;
    int positionY = infoWinHeight / 2;

    mvwprintw(*infoWin, positionY, positionX, ABOUT_PROGRAMM);
    wrefresh(*infoWin);

    touchwin(stdscr);
}

void renderResultsWindow(WINDOW *win, const char *results[], int numResults, int selectedIndex) {
    werase(win);
    box(win, 0, 0);
    for (int i = 0; i < numResults; i++) {
        if (i == selectedIndex) {
            wattron(win, A_STANDOUT);
        }
        mvwprintw(win, i + 1, 1, "%s", results[i]);
        if (i == selectedIndex) {
            wattroff(win, A_STANDOUT);
        }
    }
    int terminalWidth = getmaxx(stdscr);
    int x = (terminalWidth - strlen(NAVIGATION)) / 2; 
    mvprintw(LINES - 1, x, NAVIGATION);
    refresh();
    wrefresh(win);
}

mainWindow* renderMainWindow() {
    mainWindow *window = malloc(sizeof(mainWindow));

    WINDOW *searchWin = newwin(search_win_height, COLS - 2, 0, 1);
    WINDOW *resultWin = newwin(results_win_height, COLS - 2, search_win_height + 1, 1);

    box(searchWin, 0, 0);
    box(resultWin, 0, 0);

    refresh();
    wrefresh(searchWin);
    wrefresh(resultWin);


    keypad(searchWin, TRUE);
    
    window->resultWin = resultWin;
    window->searchWin = searchWin;

    return window;
}