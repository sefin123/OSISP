#include "gui.h"

#define HISTORY_FILE_PATH "/home/maxim/OSISP/Spasenov/SPASENOV_OSISP_prj_2024/History.txt"

void writePath(const char* path);

int printHistory();

char* keyEnterHistoryHandler();

void keyUpHistoryHandler();

void keyDownHistoryHandler();

void deletePathFromHistory();

void keyBackspaseHistoryHandler();

WINDOW* createHistoryWindow();

char* historyHandler();