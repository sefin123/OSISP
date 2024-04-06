#include "command.h"

int main() {

    initscr();
    cbreak();
    noecho();

    mainWindow *window = render_main_window();

    handle_input(window->searchWin, window->resultWin);

    delwin(window->searchWin);
    delwin(window->resultWin);
    endwin();

    return 0;
}