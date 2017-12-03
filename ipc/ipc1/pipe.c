#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    // TOVA ZNACHI LI CHE PAIPA E SVOEOBRAZEN NEPROBIVAEM BUNKER???????
    // IRRELEVANTNI
    // PERMISII
    // NE
    // OBSOLIIT
    //

    int pipefd[2];
    pid_t cpid;
    char buf;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    // When we pipe pipefd
    // pipefd[0] is for reading
    // pipefd[1] is for writing

    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        return -1;
    }

    if (cpid == 0) {
        // Child process
        close(pipefd[1]); // Close the writing pipe 

        while(read(pipefd[0], &buf, 1) > 0) {
            write(STDOUT_FILENO, &buf, 1);
        }

        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
    } else {
        // Parent process
        close(pipefd[0]); // Close the reading pipe
        
        char *str = "hyperspace";

        write(pipefd[1], str, strlen(str));
        close(pipefd[1]);

        wait(NULL);
    }

    return 0;
}
