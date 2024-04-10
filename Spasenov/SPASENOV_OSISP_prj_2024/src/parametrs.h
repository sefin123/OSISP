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
    bool fileFlag;                  // Поиск простых файлов (1 - включено, 0 - выключено)
    bool directoryFlag;            // Поиск каталогов (1 - включено, 0 - выключено)
    bool symlinkFlag;               // Поиск символических ссылок (1 - включено, 0 - выключено)
    sizeFlag* sizeFileFlag;               // Размер файла для фильтрации (-1 - отключено)
    timeFlag* timeFileFlag;               // Время изменения файла для фильтрации (-1 - отключено)
    bool emptyFlag;                  // Пустой ли (1 - включено, 0 - выключено)
} Parametrs;

#define numParametrs 7

Parametrs parametrsHandler();