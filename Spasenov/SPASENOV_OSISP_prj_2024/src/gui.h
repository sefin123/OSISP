#ifndef GUI_H
#define GUI_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define MAX_LENGTH 256

#define search_win_height 3
#define results_win_height (LINES - search_win_height - 2)

#define ABOUT_PROGRAMM "NCURSES-BASED SHELL FOR FIND UTILITY WITH HISTORY STORAGE AND EDITING"
#define ABOUT_F1_WINDOW "F1 - WINDOW FOR SELECT PARAMETRS"
#define ABOUT_PARAMETRS "ENTER TO TURN PARAMETR, RIGHT ARROW TO WRITE PARAMETR VALUE"
#define ABOUT_F2_WINDOW "F2 - WINDOW FOR INTERACTION WITH THE QUERY HISTORY"
#define ABOUT_HISTORY "BACKSPACE TO REMOVE PATH, ENTER TO SELECT"
#define ABOUT_HITS_MOVEMENT_UP_DOWN "THE MOVEMENT IS DONE ON THE UP AND DOWN ARROWS FOR SELECTION,"
#define ABOUT_HITS_MOVEMENT_RIGHT_LEFT "THE RIGHT AND LEFT ARROWS ARE USED TO MOVE THE CUSSOR IN THE WINDOWS WHERE THE INPUT OCCURS"
#define NAVIGATION "F1: Parametrs    F2: History    F3: About    F4: Back/Exit"

typedef struct mainWindow {
    WINDOW *searchWin;
    WINDOW *resultWin;
} mainWindow;

void renderResultsWindow(WINDOW *win, const char *results[],
                         int numResults, int selectedIndex);

mainWindow* renderMainWindow();

void renderAboutWindow(WINDOW **win);

void renderParametrsWindow(WINDOW **win, int selectedIndex);

void renderHistoryWindow(WINDOW **win, char* result[], int numResult, int selectedIndex);

void renderWriteWindow(WINDOW **win, const char *result,
                       int cursorPosition);

#endif