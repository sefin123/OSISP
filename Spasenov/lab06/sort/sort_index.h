#include "../structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <threads.h>
#include <pthread.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

typedef struct {
    index_record* buf;
    int blockSize;
    int threadNum;
} threadArgs;

typedef struct {
    int blockSize;
    int threads;
    char* fileName;
} creatingData;

extern int size;
extern int blocks;
extern int threads;

extern pthread_barrier_t barrier;
extern pthread_mutex_t mutex;
extern index_record* cur;

int compare(const void* a, const void* b);

void *sorting(void* thread);

void *openMem(void* data);