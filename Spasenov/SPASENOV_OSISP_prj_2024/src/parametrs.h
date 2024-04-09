#include "gui.h"

typedef struct sizeFlag{
    long fileSize;
    bool isEnable;
}sizeFlag;

typedef struct timeFlag{
    time_t time;
    bool isEnable;
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

void parametrHandler();