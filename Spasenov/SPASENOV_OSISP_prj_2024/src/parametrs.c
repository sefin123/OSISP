#include "parametrs.h"

Parametrs* parametrs;
WINDOW* win;
bool isTurnFlags[5];

int selectedparametrIndex = 1;

void keyUpParametrsHandler() {
    if (selectedparametrIndex > 1) {
        if (selectedparametrIndex > numParametrs) selectedparametrIndex = numParametrs;
        selectedparametrIndex--;
    }
}

void keyDownParametrsHandler() {
    if (selectedparametrIndex < numParametrs - 1) {
        selectedparametrIndex++;
    }
}

void printIsTurnParametr() {
    for (int i = 1; i < numParametrs; i++) {
        if (isTurnFlags[i - 1] == true) {
            mvwprintw(win, i + 1, 40, "Enable");
        } else {
            mvwprintw(win, i + 1, 40, "Disable");
        }
    }
}

bool turnFlag(bool *flag) {
    if (*flag) {
        *flag = false;
    } else {
        *flag = true;
    }
    return *flag;
}

void keyEnterParametrHandler() {
    if (selectedparametrIndex == 1) {
        parametrs->fileFlag = turnFlag(&parametrs->fileFlag);
        isTurnFlags[0] = parametrs->fileFlag ? true : false;    }
    if (selectedparametrIndex == 2) {
        parametrs->directoryFlag = turnFlag(&parametrs->directoryFlag);
        isTurnFlags[1] = parametrs->directoryFlag ? true : false;
    }
    if (selectedparametrIndex == 3) {
        parametrs->symlinkFlag = turnFlag(&parametrs->symlinkFlag);
        isTurnFlags[2] = parametrs->symlinkFlag ? true : false;
    }
    if (selectedparametrIndex == 4) {
        parametrs->sizeFileFlag->isEnable = turnFlag(&parametrs->sizeFileFlag->isEnable);
        isTurnFlags[3] = parametrs->sizeFileFlag->isEnable ? true : false;
    }
    if (selectedparametrIndex == 5) {
        parametrs->timeFileFlag->isEnable = turnFlag(&parametrs->timeFileFlag->isEnable);
        isTurnFlags[4] = parametrs->timeFileFlag->isEnable ? true : false;
    }
    if (selectedparametrIndex == 6) {
        parametrs->emptyFlag = turnFlag(&parametrs->emptyFlag);
        isTurnFlags[5] = parametrs->emptyFlag ? true : false;
    }
}

Parametrs parametrsHandler() {

    renderParametrsWindow(&win, selectedparametrIndex);
    
    parametrs = malloc(sizeof(Parametrs)); // Выделение памяти для Parametrs

    parametrs->fileFlag = false;
    parametrs->directoryFlag = false;
    parametrs->symlinkFlag = false;
    parametrs->sizeFileFlag = malloc(sizeof(sizeFlag)); // Выделение памяти для sizeFileFlag
    parametrs->sizeFileFlag->fileSize = 0;
    parametrs->sizeFileFlag->isEnable = false;
    parametrs->timeFileFlag = malloc(sizeof(timeFlag)); // Выделение памяти для timeFileFlag
    parametrs->timeFileFlag->time = 0;
    parametrs->timeFileFlag->isEnable = false;
    parametrs->emptyFlag = false;
    
    printIsTurnParametr();
    
    while (true) {
        int ch = wgetch(win);
        if (ch == KEY_F(4)) {
            return;
        }
        if (ch == KEY_UP) {
            keyUpParametrsHandler();
            renderParametrsWindow(&win, selectedparametrIndex);
            printIsTurnParametr();
        }
        if (ch == KEY_DOWN) {
            keyDownParametrsHandler();
            renderParametrsWindow(&win, selectedparametrIndex);
            printIsTurnParametr();
        }
        if (ch == 10) { //Enter
            keyEnterParametrHandler();
            printIsTurnParametr();
        }
    }
    delwin(win);
    endwin();
}