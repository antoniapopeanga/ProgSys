#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    int tube[2];
    if (pipe(tube) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == 0) {
        // Child process
        close(tube[1]);  // Close the write end
        char buffer[128];

        // Try to read from the closed write end
        ssize_t bytes_read = read(tube[1], buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("read");
        } else if (bytes_read == 0) {
            printf("Child: Read returned 0 (pipe closed for writing)\n");
        }

        close(tube[0]);  // Close the read end
        exit(EXIT_SUCCESS);
    } else {
        // Parent process
        close(tube[0]);  // Close the read end
        char message[] = "Hello, Pipe!";
        printf("Parent: Writing message to the closed read end of the pipe.\n");

        // Try to write to the closed read end
        ssize_t bytes_written = write(tube[0], message, sizeof(message));
        if (bytes_written == -1) {
            perror("write");
        } else {
            printf("Parent: Wrote %zd bytes\n", bytes_written);
        }

        close(tube[1]);  // Close the write end
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
