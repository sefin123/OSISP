#include "parametrs.h"

void parametrsHandler() {

    WINDOW* parametrsWin = newwin(0, 0, 0, 0);
    renderParametrsWindow(&parametrsWin);
    
    
    while (true) {
        int ch = wgetch(parametrsWin);
        if (ch == KEY_F(4)) {
            return;
        }
    }
    delwin(parametrsWin);
    endwin();
}