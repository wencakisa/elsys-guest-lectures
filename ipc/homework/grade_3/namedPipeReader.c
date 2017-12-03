#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdio.h>

#define NAMED_PIPE_FILENAME "namedPipe"

int main() {
    int fd = open(NAMED_PIPE_FILENAME, O_RDONLY);
    
    char buf[PIPE_BUF];
    while (read(fd, buf, PIPE_BUF) > 0) {
        printf("%s\n", buf);
    }

    close(fd);

    return 0;
}
