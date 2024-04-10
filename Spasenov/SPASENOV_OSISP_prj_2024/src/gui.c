#include "gui.h"

void renderWriteWindow(WINDOW **win, const char *result, int cursorPosition) {
    werase(*win);
    box(*win, 0, 0);
    for (int i = 0; i < (int)strlen(result); i++) {
        if (i == cursorPosition) {
            wattron(*win, A_STANDOUT);
        }
        mvwprintw(*win, 1, 1 + i, "%c", result[i]);
        if (i == cursorPosition) {
            wattroff(*win, A_STANDOUT);
        }
    }
    if (cursorPosition == (int)strlen(result)) {
        wattron(*win, A_STANDOUT);
        mvwprintw(*win, 1, strlen(result) + 1, " ");
        wattroff(*win, A_STANDOUT);
    }

    wmove(*win, 1, 1 + cursorPosition);
    wrefresh(*win);
}

void renderParametrsWindow(WINDOW** win, int selectedIndex) {
    int windowHeight, windowWidth;
    const char* parameters[] = {
        "Parametrs for utility find: ",
        "Search files",
        "Search directoreis",
        "Search symbol links",
        "Search by size(g,m,k,c,w)",
        "Search by time modify(s,m,h)",
        "Search empty files"
    };
    getmaxyx(stdscr, windowHeight, windowWidth);

    *win = newwin(windowHeight - 1, windowWidth, 0, 0);
    box(*win, 0, 0);
    keypad(*win, TRUE);
    wclear(*win);
    box(*win, 0, 0);


    for (int i = 0; i < (int)(sizeof(parameters) / sizeof(parameters[0])); i++) {
        if (i == selectedIndex) {
            wattron(*win, A_STANDOUT);
        }
        mvwprintw(*win, i + 1, 2, "%s", parameters[i]);
        if (i == selectedIndex) {
            wattroff(*win, A_STANDOUT);
        }
    }
    
    touchwin(stdscr);
}

void renderHistoryWindow(WINDOW** win) {
    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);
    *win = newwin(windowHeight - 1, windowWidth, 0, 0);
    box(*win, 0, 0);
    keypad(*win, TRUE);
    wclear(*win);
    box(*win, 0, 0);

    mvwprintw(*win, 1, 1, "History:");

    touchwin(stdscr);

}

void renderAboutWindow(WINDOW **win) {
    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);

    *win = newwin(windowHeight - 1, windowWidth, 0, 0);
    box(*win, 0, 0);
    keypad(*win, TRUE);
    wclear(*win);
    box(*win, 0, 0);

    int textLength = strlen(ABOUT_PROGRAMM);
    int positionX = (windowWidth - textLength) / 2;
    int positionY = windowHeight / 2;

    mvwprintw(*win, positionY, positionX, ABOUT_PROGRAMM);
    wrefresh(*win);

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