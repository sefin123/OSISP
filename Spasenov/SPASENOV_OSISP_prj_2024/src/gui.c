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

    wresize(*win,search_win_height, windowWidth);
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
    const char* parametrs[PARAMETRS_COUNT] = {
        PARAMETRS_FIND,
        PARAMETR_SEARCH_FILES,
        PARAMETR_SEARCH_DIRECTORIES,
        PARAMETR_SEARCH_SYMBOL_LINKS,
        PARAMETR_SEARCH_BY_SIZE,
        PARAMETR_SEARCH_BY_TIME_MODIFY,
        PARAMETR_SEARCH_EMPTY_FILES
    };

    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);

    wresize(*win,windowHeight, windowWidth);
    wclear(*win);
    box(*win, 0, 0);


    for (int i = 0; i < PARAMETRS_COUNT; i++) {
        if (i == selectedIndex) {
            wattron(*win, A_STANDOUT);
        }
        mvwprintw(*win, i + 1, 2, "%s", parametrs[i]);
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

    wresize(*win,windowHeight, windowWidth);
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
    const char *about[ABOUT_STRINGS_COUNT] = {
        ABOUT_PROGRAMM,
        ABOUT_F1_WINDOW,
        ABOUT_PARAMETRS,
        ABOUT_F2_WINDOW,
        ABOUT_HISTORY,
        ABOUT_MOVEMENT_UP_DOWN,
        ABOUT_MOVEMENT_LEFT_RIGHT,
        ABOUT_BACK_EXIT,
    };

    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);

    wresize(*win, windowHeight, windowWidth);
    wclear(*win);
    box(*win, 0, 0);

    for (int i = 0; i < ABOUT_STRINGS_COUNT; i++) {
        mvwprintw(*win, i + 1, 1, "%s", about[i]);
    }

    printNavigation(win, windowHeight - 2);

    touchwin(stdscr);
    wrefresh(*win);
}

void renderResultsWindow(WINDOW **win, const char *results[], int numResults, int selectedIndex) {
    int scrollOffset = 0;
    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);
    int visibleRows = windowHeight - search_win_height - 3;

    wresize(*win, windowHeight - search_win_height, windowWidth);
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

    printNavigation(win, windowHeight - search_win_height - 2);

    refresh();
    wrefresh(*win);
}