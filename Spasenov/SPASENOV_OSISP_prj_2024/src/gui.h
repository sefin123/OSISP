#include <ncurses.h>
#include <stdlib.h>

#define search_win_height 3
#define results_win_height (LINES - search_win_height - 2)

typedef struct mainWindow {
    WINDOW *searchWin;
    WINDOW *resultWin;
} mainWindow;

void render_search_window(WINDOW *search_win, const char *search_query, int cursor_position);

void render_results_window(WINDOW *win, const char *results[], int num_results, int selected_index);
mainWindow* render_main_window();
