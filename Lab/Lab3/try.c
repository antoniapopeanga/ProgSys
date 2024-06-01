#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd [args] motif\n", argv[0]);
        exit(1);
    }

    char *cmd = argv[1];
    char *motif = argv[argc - 1];

    DIR *dir = opendir(".");
    if (!dir) {
        perror("opendir");
        exit(2);
    }

    struct dirent *entry;
    int abnormal_exit = 0;

    while ((entry = readdir(dir))) {
        if (fnmatch(motif, entry->d_name, 0) == 0) {
            pid_t pid = fork();
            if (pid == -1) {
                perror("fork");
                exit(3);
            } else if (pid == 0) {
                // Child process
                execlp(cmd, cmd, entry->d_name, NULL);
                perror("execlp");
                exit(4);
            } else {
                // Parent process
                int status;
                wait(&status);
                if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
                    abnormal_exit = 1;
                }
            }
        }
    }

    closedir(dir);

    if (abnormal_exit) {
        fprintf(stderr, "At least one execution terminated abnormally.\n");
        exit(5);
    }

    return 0;
}
