#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>

#include "cyclic_buf.h"
#include "gen.h"

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

    // Starting reader position
    uint64_t pos = mem->pos;

    printf("Starting at position [%ld]\n", pos);

    int64_t current_seed = verify((void*) mem->data[pos % SIZE]);
    int64_t prev_seed = current_seed - 1;

    while (true) {
        if (mem->pos == pos) {
            sleep(1);
            continue;
        }

        if (mem->pos > pos + SIZE) {
            printf("Too far away... Position: [%ld]\n", mem->pos);
            break;
        }

        // Verifying the current reader position block
        // using % SIZE in order not to overflow the buffer
        // e.g.
        // 0 % SIZE = 0
        // SIZE % SIZE = 0
        current_seed = verify((void*) mem->data[pos % SIZE]);

        // "Verify чете блок, верифицира дали блока е консистентен, и връща seed променливата,
        // с която е генериран (или -1 ако забележи неконсистенция)."
        if (current_seed == -1) {
            printf("Failed to verify due to inconsistent block.\n");
            break;
        }

        // Block interrupted
        if (prev_seed != current_seed - 1) {
            printf("Failed to verify due to interrupted block.\n");
            break;
        }

        printf("Verified [%lu] | Seed [%ld]\n", pos, current_seed);

        prev_seed = current_seed;
        pos++;
    }

    return 0;
}
