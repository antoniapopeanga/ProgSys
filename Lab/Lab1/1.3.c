#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "error.h"

int main(int argc, char *argv[]) {
    assert(argc == 4);
    int fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(fd==-1){
        exit(EXIT_FAILURE);
    }
    off_t pos=(off_t)atoi(argv[2]);
    unsigned val=(unsigned)atoi(argv[3]);
    int l=lseek(fd,pos,SEEK_SET);
    check_syscall(l,"lseek");
    int w=write(fd,&val,sizeof(unsigned));
      if (w == -1) {
        perror("Error writing to file");
        exit(EXIT_FAILURE);
    }
    close(fd);
    
    
}

