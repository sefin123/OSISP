#include "about.h"

WINDOW* createAboutWindow() {
    int windowHeight, windowWidth;
    getmaxyx(stdscr, windowHeight, windowWidth);
    WINDOW *win = newwin(windowHeight, windowWidth, 0, 0);
    keypad(win, TRUE);

    return win;
}

void aboutHandler() {

    WINDOW *win = createAboutWindow();
    renderAboutWindow(&win);

    while (true) {
        int ch = wgetch(win);
        if (ch == KEY_F(4)) {
            return;
        }
        renderAboutWindow(&win);
    }
    werase(win);
    delwin(win);
    endwin();
}