#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "error.h"

void copy(int fd, int fd2, size_t buffer_size) {
    char *buffer = (char *)malloc(buffer_size); // Allocate buffer dynamically
    if (buffer == NULL) {
        perror("Error allocating buffer");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read, bytes_written;

    clock_t start_time = clock(); 

    while ((bytes_read = read(fd, buffer, buffer_size)) > 0) {
        bytes_written = write(fd2, buffer, bytes_read);
        if (bytes_written == -1) {
            perror("Error writing to output");
            exit(EXIT_FAILURE);
        }
    }

    clock_t end_time = clock(); 
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Buffer size: %zu bytes, Elapsed time: %lf seconds\n", buffer_size, elapsed_time);

    if (bytes_read == -1) {
        perror("Error reading from input");
        exit(EXIT_FAILURE);
    }

    free(buffer); 
}

int main(int argc, char **argv) {
    assert(argc == 4);
    int fd = open(argv[1], O_RDONLY);
    int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC |O_SYNC, 0644);
    size_t buffer_size = (size_t)atoi(argv[3]); 

    check(fd, "unable to open file %s", argv[1]);
    check(fd2, "unable to open file %s", argv[2]);
    copy(fd, fd2, buffer_size);
    close(fd);
    close(fd2);
    return EXIT_SUCCESS;
}


//pas d'O_SYNC 8152 bytes--0.083007 s
//avec O_SYNC 8152 bytes --2.018511 s