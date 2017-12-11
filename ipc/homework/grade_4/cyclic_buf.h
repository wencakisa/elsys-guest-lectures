#ifndef _CYCLIC_BUF
#define _CYCLIC_BUF

#include <stdint.h>

#define SIZE 4096

typedef struct cyclic_buf {
    volatile uint64_t pos;
    volatile int array[SIZE];
} cyclic_buf;

#endif
