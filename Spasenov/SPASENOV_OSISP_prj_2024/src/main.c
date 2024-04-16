#include "search.h"

int main() {
    
    initscr();
    cbreak();
    noecho();

    handleInput();

    endwin();

    return 0;
}