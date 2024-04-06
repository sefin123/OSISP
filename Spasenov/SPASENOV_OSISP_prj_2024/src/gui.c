#include "gui.h"

void render_search_window(WINDOW *win, const char *input) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "Search: %s", input);
    wrefresh(win);
}

void render_results_window(WINDOW *win, const char *results[], int num_results) {
    werase(win);
    box(win, 0, 0);
    for (int i = 0; i < num_results; i++) {
        mvwprintw(win, i + 1, 1, "%s", results[i]);
    }
    wrefresh(win);
}

mainWindow* render_main_window() {
    mainWindow *window = malloc(sizeof(mainWindow));

    WINDOW *searchWin = newwin(search_win_height, COLS - 2, 0, 1);
    WINDOW *resultWin = newwin(results_win_height, COLS - 2, search_win_height + 1, 1);

    box(searchWin, 0, 0);
    box(resultWin, 0, 0);

    refresh();
    wrefresh(searchWin);
    wrefresh(resultWin);


    keypad(searchWin, TRUE);
    
    window->resultWin = resultWin;
    window->searchWin = searchWin;

    return window;
}