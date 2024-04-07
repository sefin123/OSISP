#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define search_win_height 3
#define results_win_height (LINES - search_win_height - 2)
#define ABOUT_PROGRAMM "NCURSES-BASED SHELL FOR FIND UTILITY WITH HISTORY STORAGE AND EDITING"
#define NAVIGATION "F1: Search    F2: Parametr    F3: About"

typedef struct mainWindow {
    WINDOW *searchWin;
    WINDOW *resultWin;
} mainWindow;

void renderSearchWindow(WINDOW *searchWin, const char *searchQuery, int cursorPosition);

void renderResultsWindow(WINDOW *win, const char *results[], int numResults, int selectedIndex);

mainWindow* renderMainWindow();

void renderInfoWindow(WINDOW **infoWin);