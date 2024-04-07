#ifndef GUI_H
#define GUI_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define search_win_height 3
#define results_win_height (LINES - search_win_height - 2)
#define ABOUT_PROGRAMM "NCURSES-BASED SHELL FOR FIND UTILITY WITH HISTORY STORAGE AND EDITING"
#define NAVIGATION "F1: Parametrs    F2: History    F3: About    F4: Back/Exit"

typedef struct mainWindow {
    WINDOW *searchWin;
    WINDOW *resultWin;
} mainWindow;

void renderSearchWindow(WINDOW *win, const char *searchQuery, int cursorPosition);

void renderResultsWindow(WINDOW *win, const char *results[], int numResults, int selectedIndex);

mainWindow* renderMainWindow();

void renderInfoWindow(WINDOW **win);

void renderParametrsWindow(WINDOW** win);

#endif