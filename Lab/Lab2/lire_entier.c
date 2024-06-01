#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    assert(argc==3);
    int fd_err;
    fd_err=open("ERREURS-LIRE.log.",O_WRONLY|O_CREAT|O_APPEND,0644);
    if(fd_err==-1){
        perror("File could not be opened");
        return 1;
    }
    if(dup2(fd_err,2)==-1){
        perror("STDERR could not be redirected");
        return 1;
    }
    int fd=open(argv[1],O_WRONLY|O_TRUNC,0644);
    off_t val=(off_t)atoi(argv[2]);
    if(write(fd,&val,sizeof(off_t))==-1)
    {
        perror("Error writing in the file");
        return 1;
    }



    close(fd_err);


}
