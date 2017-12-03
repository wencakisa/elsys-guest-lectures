#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#include "cyclic_buf.h"

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

    mem->pos %= 4096;
    int current_value = mem->array[mem->pos];

    while (true) {
        mem->pos++;
        mem->pos %= 4096;

        printf("Written: %d\n", current_value);
        mem->array[mem->pos] = current_value;
        current_value++;

        if (mem->pos % 1024 == 0) {
            sleep(1);
        }
    }

    return 0;
}
