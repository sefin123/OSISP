#include "about.h"

void aboutHandler() {
    WINDOW *win;
    renderAboutWindow(&win);

    while (true) {
        int ch = wgetch(win);
        if (ch == KEY_F(4)) {
            return;
        }
    }
    delwin(win);
    endwin();
}