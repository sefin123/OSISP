#include "search.h"

int main() {
    
    initscr();
    cbreak();
    noecho();

    mainWindow *window = renderMainWindow();

    handleInput(window->searchWin, window->resultWin);

    delwin(window->searchWin);
    delwin(window->resultWin);
    endwin();

    return 0;
}