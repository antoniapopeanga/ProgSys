#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

#include "error.h"
#define BUFSIZE 1024
void print_write(int fd2)
{ char buffer[BUFSIZE];
  ssize_t bytes_read,bytes_written;
  bytes_read=read(0,buffer,BUFSIZE);
  if(bytes_read>0){
  bytes_written=write(fd2,buffer,bytes_read);
  write(1,buffer,bytes_read);
  }
  if(bytes_written==-1)
  {
    perror("write");
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char **argv)
{
  assert (argc == 2);
  int fd,fd2;
  fd2=open(argv[1],O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fd2==-1)
  {
    printf("This file could not be opened: %s\n", argv[1]); // Corrected printf
   return EXIT_FAILURE;
  }


  print_write(fd2);
  close(fd2);
  return EXIT_SUCCESS;
}
