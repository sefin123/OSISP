#include "parametrs.h"
#include "about.h"
#include "history.h"

#define PATH_MAX 512

int lstat(const char *path, struct stat *buf);

int compareFilenames(const void *a, const void *b);

void saveResult(const char *fullPath, const char *input,
                const char *results[], int *numResults);

void processSizeFlag(const char *filePath, const char *input,
                     const char *results[], int *numResults, Parametrs *param);

void processTimeFlag(const char *filePath, const char *input,
                     const char *results[], int *numResults, Parametrs *param);

void processEntry(const char *fullPath, const char *input,
                  const char *results[], int *numResults, Parametrs *param);

void processEmptyFile(const char *fullPath, const char *input,
                      const char *results[], int *numResults);

void saveDirectory(const char *path, const char *input,
                   const char *results[], int *numResults);

int printDirectory(const char *input, const char *path,
                   const char *results[], int selectedIndex,
                   int cousorPosition);

void writePath(const char* path);

void keyDownHandler();

void keyBackspaseHandler();

void keyUpHandler();

void keyRightHandler();

void keyLeftHandler();

void keyEnterHandler();

void keyF3Handler();

void keyF2Handler();

void keyF1Handler();

void writeHandler(int ch);

void handleInput();
