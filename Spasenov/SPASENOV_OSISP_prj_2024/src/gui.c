#include "gui.h"

void render_search_window(WINDOW *search_win, const char *search_query, int cursor_position) {
    werase(search_win);
    box(search_win, 0, 0);
    for (int i = 0; i < strlen(search_query); i++) {
        if (i == cursor_position) {
            wattron(search_win, A_STANDOUT);  // Включение выделения для выбранного символа
        }
        mvwprintw(search_win, 1, 1 + i, "%c", search_query[i]);
        if (i == cursor_position) {
            wattroff(search_win, A_STANDOUT);  // Выключение выделения
        }
    }

    if (cursor_position == strlen(search_query)) {
        wattron(search_win, A_STANDOUT);  // Включение режима обратного видимости
        mvwprintw(search_win, 0, strlen(search_query) + 1, " ");  // Вывод пробела после строки поиска
        wattroff(search_win, A_STANDOUT);  // Отключение режима обратного видимости
    }
    wmove(search_win, 1, 1 + cursor_position);
    wrefresh(search_win);
}

void render_results_window(WINDOW *win, const char *results[], int num_results, int selected_index) {
    werase(win);
    box(win, 0, 0);
    for (int i = 0; i < num_results; i++) {
        if (i == selected_index) {
            wattron(win, A_STANDOUT);
        }
        mvwprintw(win, i + 1, 1, "%s", results[i]);
        if (i == selected_index) {
            wattroff(win, A_STANDOUT);
        }
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