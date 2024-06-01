#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <command1> <command2> <arg1> <arg2> ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    int tube[2];
    if (pipe(tube) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        // Child process
        close(tube[0]);  // Close the read end---really important
        dup2(tube[1], 1);  // Redirect stdout to the write end of the pipe
        close(tube[1]);  // Close the write end
        execlp(argv[1], argv[1], NULL);
        perror(argv[1]);
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(tube[1]);  // Close the write end---really important
        dup2(tube[0], 0);  // Redirect stdin to the read end of the pipe
        close(tube[0]);  // Close the read end
        execvp(argv[2], &argv[2]);//&argv[2] goes from the 3rd arg to the end
        perror(argv[2]);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}
