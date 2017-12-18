#ifndef _CYCLIC_BUF_H
#define _CYCLIC_BUF_H

#include <stdint.h>

#include "gen.h"

// Hints:
// buffer size: 512 blocks
#define SIZE 512

typedef struct cyclic_buf {
    // 64 bit unsigned integer... ;)
    volatile uint64_t pos;
    volatile uint64_t data[SIZE][GEN_BLOCK_SIZE];
} cyclic_buf;

#endif
