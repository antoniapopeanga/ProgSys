#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

#include "error.h"

void copy(int fd, int fd2, size_t buffer_size) {
    char buffer[buffer_size]; // Use a buffer of the specified size
    ssize_t bytes_read, bytes_written;
    bytes_read = read(fd, buffer, buffer_size);
    bytes_written = write(fd2, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Error writing to output");
            exit(EXIT_FAILURE);
        }

    if (bytes_read == -1) {
        perror("Error reading from input");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    assert(argc == 4);
    int fd = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    size_t buffer_size = (size_t)atoi(argv[3]); // Convert the third argument to an integer

    check(fd, "unable to open file %s", argv[1]);
    check(fd2, "unable to open file %s", argv[2]);
    copy(fd, fd2, buffer_size);
    close(fd);
    close(fd2);
    return EXIT_SUCCESS;
}
