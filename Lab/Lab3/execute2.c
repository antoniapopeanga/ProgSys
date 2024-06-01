#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(argv[1], argv + 1) == -1) {
            perror("execvp");
            exit(2);
        }
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            int return_code = WEXITSTATUS(status);
            printf("Le code de retour est %d\n", return_code);
        }
    } else {
        // Fork failed
        perror("fork");
        exit(1);
    }

    return 0;
}