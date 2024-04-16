#include "gui.h"

void printNavigation(WINDOW** win, int windowHeight) {
    int windowWidth;
    windowWidth = getmaxx(stdscr);
    int x = (windowWidth - strlen(NAVIGATION)) / 2; 
    mvwprintw(*win, windowHeight, x, NAVIGATION);
}

void renderWriteWindow(WINDOW **win, const char *result, int cursorPosition) {

    int windowWidth;
    windowWidth = getmaxx(stdscr);

    *win = newwin(search_win_height, windowWidth , 0, 0);
    keypad(*win, TRUE);
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

    wrefresh(*win);
}

void renderParametrsWindow(WINDOW** win, int selectedIndex) {
    const char* parameters[] = {
        "Parametrs for utility find: ",
        "Search files",
        "Search directoreis",
        "Search symbol links",
        "Search by size(g,m,k,c,w,b)",
        "Search by time modify(s,m,h)",
        "Search empty files"
    };

    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);

    *win = newwin(windowHeight, windowWidth, 0, 0);
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
    
    printNavigation(win, windowHeight - 2);

    touchwin(stdscr);
    wrefresh(*win);
}

void renderHistoryWindow(WINDOW** win, char* result[], int numResult, int selectedIndex) {

    int windowHeight, windowWidth, scrollOffset = 0;
    getmaxyx(stdscr, windowHeight, windowWidth);

    *win = newwin(windowHeight, windowWidth, 0, 0);
    keypad(*win, TRUE);
    wclear(*win);
    box(*win, 0, 0);

    int visibleRows = windowHeight - 5;
    
    if (selectedIndex >= visibleRows) {
        scrollOffset = selectedIndex - visibleRows + 1;
    }

    mvwprintw(*win, 1, 1, "History:");

    for(int i = 0; i < visibleRows && i + scrollOffset < numResult; i++) {
        int resultIndex = i + scrollOffset;

        if(resultIndex == selectedIndex) {
            wattron(*win, A_STANDOUT);
            mvwprintw(*win, i + 3, 1, "%s", result[resultIndex]);
            wattroff(*win, A_STANDOUT);
        } else {
            mvwprintw(*win, i + 3, 1, "%s", result[resultIndex]);
        }
    }

    printNavigation(win, windowHeight - 2);

    touchwin(stdscr);
    wrefresh(*win);
}

void renderAboutWindow(WINDOW **win) {

    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);

    *win = newwin(windowHeight, windowWidth, 0, 0);
    keypad(*win, TRUE);
    wclear(*win);
    box(*win, 0, 0);

    mvwprintw(*win, 1, 1, ABOUT_PROGRAMM);
    mvwprintw(*win, 2, 1, ABOUT_F1_WINDOW);
    mvwprintw(*win, 3, 1, ABOUT_PARAMETRS);
    mvwprintw(*win, 4, 1, ABOUT_F2_WINDOW);
    mvwprintw(*win, 5, 1, ABOUT_HISTORY);
    mvwprintw(*win, 6, 1, ABOUT_HITS_MOVEMENT_UP_DOWN);
    mvwprintw(*win, 7, 1, ABOUT_HITS_MOVEMENT_RIGHT_LEFT);
    
    printNavigation(win, windowHeight - 2);

    touchwin(stdscr);
    wrefresh(*win);
}

void renderResultsWindow(WINDOW **win, const char *results[], int numResults, int selectedIndex) {
    int visibleRows = results_win_height - 2;
    int scrollOffset = 0;
    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);

    *win = newwin(windowHeight - search_win_height, windowWidth, search_win_height, 0);
    keypad(*win, TRUE);
    werase(*win);
    box(*win, 0, 0);

    if (selectedIndex >= visibleRows) {
        scrollOffset = selectedIndex - visibleRows + 1;
    }

    for (int i = 0; i < visibleRows && i + scrollOffset < numResults; i++) {
        int resultIndex = i + scrollOffset;

        if (resultIndex == selectedIndex) {
            wattron(*win, A_STANDOUT);
        }
        mvwprintw(*win, i + 1, 1, "%s", results[resultIndex]);
        if (resultIndex == selectedIndex) {
            wattroff(*win, A_STANDOUT);
        }
    }

    printNavigation(win, windowHeight - 5);

    refresh();
    wrefresh(*win);
}