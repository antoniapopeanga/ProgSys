#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int tube[2];
    if (pipe(tube) == -1) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    ssize_t bytes_written = 0;
    int buffer_size = 1;  // Initial buffer size
    char* data = (char*)malloc(buffer_size);

    // Keep writing to the pipe until it blocks
    while (1) {
        ssize_t result = write(tube[1], data, buffer_size);

        if (result == -1) {
            // The pipe is full, so we've reached its capacity
            perror("write");
            break;
        }

        bytes_written += result;
        printf("Bytes written: %zd\n", bytes_written);

        // Increase buffer size for the next write
        buffer_size++;
        data = (char*)realloc(data, buffer_size);
    }

    free(data);
    close(tube[0]);
    close(tube[1]);

    return EXIT_SUCCESS;
}
