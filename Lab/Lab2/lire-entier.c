#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
unsigned lire_entier(int fd)
{
    unsigned x;
    if(read(fd,&x,sizeof(unsigned))==-1)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
return x;
}
int main(int argc,char*argv[])
{   assert(argc==2);
    int fd=open(argv[1],O_RDONLY);
    printf("Valoarea este %d\n",lire_entier(fd));
    close(fd);
    return 0;

}