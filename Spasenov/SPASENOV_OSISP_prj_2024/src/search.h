#include "parametrs.h"
#include "about.h"
#include "history.h"

#define PATH_MAX 257

enum {  
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
  };

extern int scandir (const char *__restrict __dir,
		    struct dirent ***__restrict __namelist,
		    int (*__selector) (const struct dirent *),
		    int (*__cmp) (const struct dirent **,
				  const struct dirent **))
     __nonnull ((1, 2));

int compareFilenames(const void *a, const void *b);

void saveDirectory(const char *path, const char *input, const char *results[], int *numResults);

int printDirectory(WINDOW *searchWin, WINDOW *resultsWin, const char *input, const char *path, const char *results[], int selectedIndex, int cousorPosition);

void writePath(const char* path);

void keyDownHandler(WINDOW* win);

void keyBackspaseHandler();

void keyUpHandler(WINDOW* win);

void keyRightHandler();

void keyLeftHandler();

void keyEnterHandler();

void keyF3Handler();

void keyF2Handler();

void keyF1Handler();

void writeHandler(int ch);

void handleInput(WINDOW *searchWin, WINDOW *resultsWin);
