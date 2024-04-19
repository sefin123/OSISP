#include "gui.h"

#define HISTORY_FILE_PATH "/home/maxim/OSISP/Spasenov/SPASENOV_OSISP_prj_2024/History.txt"

char* historyHandler();

void keyBackspaseHistoryHandler();

void deletePathFromHistory();

char* keyEnterHistoryHandler();

int printHistory();

void keyDownHistoryHandler();

void keyUpHistoryHandler();

WINDOW* createHistoryWindow();