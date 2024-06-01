#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <command1> <command2> <arg1> <arg2> ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    int tube[2];
    if (pipe(tube) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t pidp = fork();
    if (pidp == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pidp == 0) {
        close(tube[0]);
        dup2(tube[1], 1);
        close(tube[1]);
        execvp(argv[1], &argv[1]);
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    pid_t pidc = fork();
    if (pidc == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pidc == 0) {
        close(tube[1]);
        dup2(tube[0], 0);
        close(tube[0]);
        execvp(argv[2], &argv[2]);
        perror(argv[2]);
        exit(EXIT_FAILURE);
    }

    close(tube[1]);
    int status;
    waitpid(pidp, &status, 0);
    
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        int status2;
        waitpid(pidc, &status2, 0);
        return WEXITSTATUS(status2);
    } else {
        return WEXITSTATUS(status);
    }
}
