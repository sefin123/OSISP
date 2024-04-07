#include "parametrs.h"

void parametrsHandler() {

    WINDOW* win;
    renderParametrsWindow(&win);
    
    while (true) {
        int ch = wgetch(win);
        if (ch == KEY_F(4)) {
            return;
        }
    }
    delwin(win);
    endwin();
}