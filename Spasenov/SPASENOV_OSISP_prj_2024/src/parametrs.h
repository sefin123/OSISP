#include "gui.h"

typedef struct sizeFlag{
    long size;
    bool isEnable;
    bool isMore;
}sizeFlag;

typedef struct timeFlag{
    time_t time;
    bool isEnable;
    bool isMore;
}timeFlag;

typedef struct Parametrs {
    bool fileFlag;
    bool directoryFlag;
    bool symlinkFlag;
    sizeFlag* sizeFileFlag;
    timeFlag* timeFileFlag;
    bool emptyFlag;
} Parametrs;

#define numParametrs 7

long convertToLong();

time_t convertToTime();

void printIsTurnParametr();

bool turnFlag(bool flag);

void updateIsTurn();

char* writeValueParametrs(char* input);

void keyEnterParametrsHandler();

void keyUpParametrsHandler();

void keyDownParametrsHandler();

void keyRightParametrsHandler();

Parametrs* allocateMemory();

WINDOW* createParametrsWindow();

WINDOW* createParametrsWriteValueWindow();

Parametrs* parametrsHandler(Parametrs *param);