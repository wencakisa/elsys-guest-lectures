#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include "cyclic_buf.h"

#define SHM_NAME "simple_shared_mem"

int main() {
    int mem_fd = shm_open(SHM_NAME, O_RDONLY, S_IRUSR);

    if (mem_fd == -1) {
        perror("shm_open");
        return -1;
    }

    cyclic_buf* mem = mmap(NULL, sizeof(cyclic_buf), PROT_READ, MAP_SHARED, mem_fd, 0);

    if (mem == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    uint64_t pos = (mem->pos + 2048) % 4096;
    while (true) {
        if (mem->pos == pos) {
            sleep(1);
            continue;
        }

        pos++;
        pos %= 4096;

        printf("Read: %d\n", mem->array[pos]);
    }

    return 0;
}
