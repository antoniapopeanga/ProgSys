#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define FILENAME "tmp"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <command1> <command2> [args for command2]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int out = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (out == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();

    if (pid == 0) {
        dup2(out, 1);
        close(out);
        execlp(argv[1], argv[1], NULL);
        perror(argv[1]);
        exit(127);
    }

    waitpid(pid, NULL, 0);
    int in = open(FILENAME, O_RDONLY);
    if (in == -1) {
        perror("open");
        return EXIT_FAILURE;
    }
    dup2(in, 0);
    close(in);

    // Execute the second command with its arguments
    execvp(argv[2], &argv[2]);
    perror(argv[2]);
    exit(127);

    // Add code to remove the temporary file after you're done with it.
    remove(FILENAME);

    return EXIT_SUCCESS;
}
