#include "about.h"


void aboutHandler() {
    WINDOW *win;
    renderInfoWindow(&win);

    keypad(win, TRUE);

    int simbol;
    while (true) {
        simbol = wgetch(win);
        if (simbol == KEY_F(4)) {
            break;
        }
    }
    delwin(win);
}