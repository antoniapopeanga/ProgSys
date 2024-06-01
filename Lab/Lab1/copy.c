#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

#include "error.h"
#define BUFSIZE 1024
void copy(int fd, int fd2){
char buffer[BUFSIZE];
size_t bytes_read,bytes_written;
bytes_read=read(fd,buffer,BUFSIZE);
if(bytes_read>0)
{
  bytes_written=write(fd2,buffer,bytes_read);
  if(bytes_written==-1)
  {
    perror("write");
    exit(EXIT_FAILURE);
  }
}
}
int main(int argc, char **argv)
{  assert(argc==3);
  int fd=open(argv[1],O_RDONLY);
  int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  check(fd,"unable to open file%s",argv[1]);
  check(fd2,"unable to open file%s",argv[2]);
  copy(fd,fd2);
  close(fd);
  close(fd2);
  return EXIT_SUCCESS;
}
