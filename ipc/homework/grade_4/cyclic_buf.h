#ifndef _DUMB_SHM_STRUCT
#define _DUMB_SHM_STRUCT

#include <stdint.h>

#define SIZE 4096

typedef struct cyclic_buf {
    volatile uint64_t pos;
    volatile int array[SIZE];
} cyclic_buf;

#endif
