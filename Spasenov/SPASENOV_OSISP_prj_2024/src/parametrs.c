#include "parametrs.h"

Parametrs* parametrs;
WINDOW* win;
WINDOW* writeWin;
bool isTurnFlags[5] = {true, true, true, false , false};
int selectedparametrIndex = 1;
bool isSizeFileFlagFocused = false;
bool isTimeFileFlagFocused = false;
char sizeFlagInput[MAX_LENGTH] = "";
int sizeFlagInputLength = 0;
char timeFlagInput[MAX_LENGTH] = "";
int timeFlagInputLength = 0;
int inputLengthParametrsValue = 0;
int cursorPositionParametrsValue = 0;

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

long convertToLong() {
    char* endPtr;
    long size = strtol(sizeFlagInput, &endPtr, 10);
    
    if (*endPtr == 'G' || *endPtr == 'g') {
        size = labs(size) * 1024 * 1024 * 1024;  
    } else if (*endPtr == 'M' || *endPtr == 'm') {
        size = labs(size) * 1024 * 1024;         
    } else if (*endPtr == 'K' || *endPtr == 'k') {
        size = labs(size) * 1024;                
    } else if (*endPtr == 'c') {
        size = labs(size) * 512;                 
    }  else if (*endPtr == 'w') {
        size = labs(size) * 2;                   
    } else if (*endPtr == 'b') {
        return labs(size);
    } else {
        return -1;                   // Возвращаем ошибку или другое значение по вашему усмотрению
    }

    return size;
}

time_t convertToTime() {
    int length = strlen(timeFlagInput);
    char unit = timeFlagInput[length - 1];
    int value = atoi(timeFlagInput);
    time_t currentTime = time(NULL);
    struct tm *timeInfo = localtime(&currentTime);

        if (unit == 's') {
            timeInfo->tm_sec -= labs(value);
        } else if (unit == 'm') {
            timeInfo->tm_min -= labs(value);
        } else if (unit == 'h') {
            timeInfo->tm_hour -= labs(value);
        } else {
            return -1;
        }

    return mktime(timeInfo);
}

void printIsTurnParametr() {
    for (int i = 1; i < numParametrs; i++) {
        if (isTurnFlags[i - 1]) {
            mvwprintw(win, i + 1, 35, "Enable");
        } else {
            mvwprintw(win, i + 1, 35, "Disable");
        }
    }
    if (isTurnFlags[3]) {
        if (parametrs->sizeFileFlag->size == -1) {
            mvwprintw(win, 5, 45, "Invalid value");
        } else {
            mvwprintw(win, 5, 45, "%s", sizeFlagInput);
        }
    }
    if (isTurnFlags[4]) {
        if (parametrs->timeFileFlag->time == -1) {
            mvwprintw(win, 6, 45, "Invalid value");
        } else {
            mvwprintw(win, 6, 45, "%s", timeFlagInput);
        }
    }
}

bool turnFlag(bool flag) {
    return flag ? false : true;
}

void updateIsTurn() {
        isTurnFlags[0] = parametrs->fileFlag ? true : false; 
        isTurnFlags[1] = parametrs->directoryFlag ? true : false;
        isTurnFlags[2] = parametrs->symlinkFlag ? true : false;
        isTurnFlags[3] = parametrs->sizeFileFlag->isEnable ? true : false;
        isTurnFlags[4] = parametrs->timeFileFlag->isEnable ? true : false;
        isTurnFlags[5] = parametrs->emptyFlag ? true : false;
}

void keyEnterParametrsHandler() {
    if (selectedparametrIndex == 1) {
        parametrs->fileFlag = turnFlag(parametrs->fileFlag);
        isTurnFlags[0] = parametrs->fileFlag ? true : false; 
    }
    if (selectedparametrIndex == 2) {
        parametrs->directoryFlag = turnFlag(parametrs->directoryFlag);
        isTurnFlags[1] = parametrs->directoryFlag ? true : false;
    }
    if (selectedparametrIndex == 3) {
        parametrs->symlinkFlag = turnFlag(parametrs->symlinkFlag);
        isTurnFlags[2] = parametrs->symlinkFlag ? true : false;
    }
    if (selectedparametrIndex == 4) {
        parametrs->sizeFileFlag->isEnable = turnFlag(parametrs->sizeFileFlag->isEnable);
        isTurnFlags[3] = parametrs->sizeFileFlag->isEnable ? true : false;
    }
    if (selectedparametrIndex == 5) {
        parametrs->timeFileFlag->isEnable = turnFlag(parametrs->timeFileFlag->isEnable);
        isTurnFlags[4] = parametrs->timeFileFlag->isEnable ? true : false;
    }
    if (selectedparametrIndex == 6) {
        parametrs->emptyFlag = turnFlag(parametrs->emptyFlag);
        isTurnFlags[5] = parametrs->emptyFlag ? true : false;
    }
}

char* writeValueParamtrs(char* input) {

    inputLengthParametrsValue = (int)strlen(input);
    cursorPositionParametrsValue = (int)strlen(input);

    while(true) {
        int ch = wgetch(win);
        switch (ch) {
            case KEY_F(4): {
                return "";
            }
            case 10: {
                return input;
            }
            case KEY_BACKSPACE: {
                if (inputLengthParametrsValue > 0) {
                    for (int i = cursorPositionParametrsValue; i < inputLengthParametrsValue; i++) {
                        input[i - 1] = input[i];
                    }
                    inputLengthParametrsValue--;
                    cursorPositionParametrsValue--;
                    if(cursorPositionParametrsValue == -1) cursorPositionParametrsValue = 0;
                    input[inputLengthParametrsValue] = '\0';
                }
            break;
            }
            case KEY_RIGHT: {
                if (cursorPositionParametrsValue < inputLengthParametrsValue) {
                    cursorPositionParametrsValue++;
                }
            break;
            }
            case KEY_LEFT: {
                if (cursorPositionParametrsValue > 0) {
                cursorPositionParametrsValue--;
                }
            break;
            }
            default: {
                if (ch >= 32 && ch <= 126 && inputLengthParametrsValue < MAX_LENGTH - 1) {
                    for (int i = inputLengthParametrsValue; i > cursorPositionParametrsValue; i--) {
                        input[i] = input[i - 1];
                    }
                    input[cursorPositionParametrsValue++] = ch;
                    inputLengthParametrsValue++;
                    input[inputLengthParametrsValue] = '\0';
                }
            }
        }
        renderWriteWindow(&writeWin, input, cursorPositionParametrsValue);
    }
}

void keyRightParametrsHandler() {

    updateIsTurn();
    
    if (selectedparametrIndex == 4) {
        renderWriteWindow(&writeWin, sizeFlagInput, cursorPositionParametrsValue);
        strcpy(sizeFlagInput, writeValueParamtrs(sizeFlagInput));
        parametrs->sizeFileFlag->size = convertToLong();
        parametrs->sizeFileFlag->isMore = (sizeFlagInput[0] == '+') ? true : false;
        printIsTurnParametr();
    }

    if (selectedparametrIndex == 5) {
        renderWriteWindow(&writeWin, timeFlagInput, cursorPositionParametrsValue);
        strcpy(timeFlagInput, writeValueParamtrs(timeFlagInput));
        parametrs->timeFileFlag->time = convertToTime();
        parametrs->timeFileFlag->isMore = (timeFlagInput[0] == '+') ? true : false;
        printIsTurnParametr();
    }
}

Parametrs* allocateMemory() {
    
    Parametrs *parametrs = malloc(sizeof(Parametrs));

    parametrs->fileFlag = true;
    parametrs->directoryFlag = true;
    parametrs->symlinkFlag = true;
 
    parametrs->sizeFileFlag = malloc(sizeof(sizeFlag));
    parametrs->sizeFileFlag->size = 0;
    parametrs->sizeFileFlag->isEnable = false;
    parametrs->sizeFileFlag->isMore = true;
 
    parametrs->timeFileFlag = malloc(sizeof(timeFlag));
    parametrs->timeFileFlag->time = 0;
    parametrs->timeFileFlag->isEnable = false;
    parametrs->timeFileFlag->isMore = false;
 
    parametrs->emptyFlag = false;

    return parametrs;
}

WINDOW* createParametrsWindow() {
    
    WINDOW* win;
    int windowHeight;
    int windowWidth;

    getmaxyx(stdscr, windowHeight, windowWidth);

    win = newwin(windowHeight - 1, windowWidth, 0, 0);

    return win;
}

WINDOW* createParametrsWriteValueWindow() {
    WINDOW* win = newwin(search_win_height, COLS - 2, 8, 1);
    keypad(win, TRUE);

    return win;
}

Parametrs* parametrsHandler(Parametrs *param) {

    win = createParametrsWindow();
    renderParametrsWindow(&win, selectedparametrIndex);

    writeWin = createParametrsWriteValueWindow();
    
    parametrs = param;
    printIsTurnParametr();
    
    while (true) {
        int ch = wgetch(win);
        if (ch == KEY_F(4)) {
            return parametrs;
        }
        if (ch == KEY_UP) {
            keyUpParametrsHandler();
        }
        if (ch == KEY_DOWN) {
            keyDownParametrsHandler();
        }
        if (ch == 10) { //Enter
            keyEnterParametrsHandler();
        }
        if (ch == KEY_RIGHT) {
            keyRightParametrsHandler();
        }
        renderParametrsWindow(&win, selectedparametrIndex);
        printIsTurnParametr();
    }
    delwin(win);
    endwin();
}