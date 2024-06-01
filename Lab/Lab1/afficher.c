#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#define SUFFIXE ".idx"

int main(int argc, char *argv[]) {
    assert(argc == 3);

    int l = strlen(argv[1]);
    char idx_filename[l + strlen(SUFFIXE) + 1];

    strncpy(idx_filename, argv[1], l);
    strcpy(idx_filename + l, SUFFIXE);

    off_t line = atoi(argv[2]);

    int idx = open(idx_filename, O_RDONLY);
    if (idx == -1) {
        perror("Error opening index file");
        return 1;
    }

    off_t pos1, pos2;
    if (lseek(idx, (line-2) * sizeof(off_t), SEEK_SET) == -1) {
        perror("Error seeking to line in index");
        close(idx);
        return 1;
    }
    
    if (read(idx, &pos1, sizeof(off_t)) == -1) {
        perror("Error reading position from index");
        close(idx);
        return 1;
    }

    if (read(idx, &pos2, sizeof(off_t)) == -1) {
        perror("Error reading position from index");
        close(idx);
        return 1;
    }

    close(idx);

    int in = open(argv[1], O_RDONLY);
    if (in == -1) {
        perror("Error opening input file");
        return 1;
    }

    if (lseek(in, pos1, SEEK_SET) == -1) {
        perror("Error seeking to position in input file");
        close(in);
        return 1;
    }

    char buf[pos2 - pos1 + 1];
    ssize_t bytes_read = read(in, buf, pos2 - pos1);
    if (bytes_read == -1) {
        perror("Error reading line from input file");
        close(in);
        return 1;
    }

    buf[bytes_read] = '\0'; 

    printf("Line %ld: %s\n", line, buf);

    close(in);

    return 0;
}
