#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define NAMED_PIPE_FILENAME "namedPipe"

int main() {
    mkfifo(NAMED_PIPE_FILENAME, 0666);
    int fd = open(NAMED_PIPE_FILENAME, O_WRONLY);

    const char* message = "Hello from writer!";

    while (1) {
        write(fd, message, strlen(message));
        sleep(3);
    }

    close(fd);

    return 0;
}
