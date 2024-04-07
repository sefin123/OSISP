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
        wattron(search_win, A_STANDOUT);
        mvwprintw(search_win, 1, strlen(search_query) + 1, " ");
        wattroff(search_win, A_STANDOUT);
    }

    wmove(search_win, 1, 1 + cursor_position);
    wrefresh(search_win);
}

void render_info_window(WINDOW **info_win) {
    int rows, cols;
    getmaxyx(stdscr, rows, cols);  // Получение размеров терминала
    int info_win_height = rows - 2;  // Высота окна информации
    int info_win_width = cols - 2;  // Ширина окна информации
    int info_win_y = 1;  // Положение окна информации по вертикали
    int info_win_x = 1;  // Положение окна информации по горизонтали

    // Создание информационного окна
    *info_win = newwin(info_win_height, info_win_width, info_win_y, info_win_x);
    box(*info_win, 0, 0);  // Отрисовка рамки вокруг окна информации

    // Очистка и обновление информационного окна
    wclear(*info_win);  // Очистка окна информации
    box(*info_win, 0, 0);  // Отрисовка рамки вокруг окна информации

    int text_length = strlen(ABOUT_PROGRAMM);
    int position_x = (info_win_width - text_length) / 2;
    int position_y = info_win_height / 2;

    mvwprintw(*info_win, position_y, position_x, ABOUT_PROGRAMM);  // Вывод информации
    wrefresh(*info_win);  // Обновление окна информации

    // Пометка информационного окна для обновления
    touchwin(stdscr);
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
    int terminal_width = getmaxx(stdscr);  // Получаем ширину терминала
    int x = (terminal_width - strlen(NAVIGATION)) / 2; 
    mvprintw(LINES - 1, x, NAVIGATION);
    refresh();
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