#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#include "cyclic_buf.h"
#include "gen.h"

#define SHM_NAME "simple_shared_mem"

int main() {
    int mem_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, S_IRWXU);

    if (mem_fd < 0) {
        perror("shm_open");
        return -1;
    }

    int res = ftruncate(mem_fd, sizeof(cyclic_buf));

    if (res < 0) {
        perror("ftruncate");
        return res;
    }

    cyclic_buf* mem = mmap(NULL, sizeof(cyclic_buf), PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, 0);

    if (mem == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    mem->pos = 0;
    uint32_t seed = 0;

    while (true) {
        // Пише в цикличния буфер, използвайки generate.
        generate((void*) mem->data[mem->pos % SIZE], seed);

        mem->pos++;
        // Seed променливата трябва да се променя при всяко викане на функцията.
        seed++;
    }

    return 0;
}
