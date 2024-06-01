#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define SUFFIXE ".idx"
#define BUF_SIZE 2048

int main(int argc, char *argv[]) {
    assert(argc == 2);

    char idx_filename[strlen(argv[1]) + strlen(SUFFIXE) + 1];
    strcpy(idx_filename, argv[1]);
    strcat(idx_filename, SUFFIXE);

    int in = open(argv[1], O_RDONLY);
    if (in == -1) {
        perror("Error opening input file");
        return 1;
    }

    int idx = open(idx_filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (idx == -1) {
        perror("Error opening index file");
        close(in);
        return 1;
    }

    char buf[BUF_SIZE];
    off_t pos = 0;
    off_t line_start = 0; 

    ssize_t bytesRead;
    while ((bytesRead = read(in, buf, BUF_SIZE)) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (buf[i] == '\n') {
                write(idx, &line_start, sizeof(off_t));
            }
            line_start = pos + 1; 
            pos++;
        }
    }

    close(in);
    close(idx);

    return 0;
}
