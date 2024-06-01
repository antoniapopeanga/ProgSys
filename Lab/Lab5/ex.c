#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/mman.h>
//le miroir d'un fichier
//mmap,paralleliser le traitement
//ouvrir le fichier, fork()
void miroir(char *tab,int d, int f, int nb)
{
    for(int i=d;i<f;i++)
    swap(tab+i,tab+nb-i-1);
}
int main(int argc,char **argv)
{
    assert(argc==2);
    int fd=open(argv[1],O_RDWR,0644);
    if(fd==-1)
    {
        perror("open");
    }
    off_t len=lseek(fd,0,SEEK_END);
    lseek(fd,0,SEEK_SET);
    char *addr= mmap(NULL,len, PROT_READ|PROT_WRITE, MAP_SHARED,fd,0 );

    /*tranche=nb/p;
    debut=id*tranche;
    fin=(id!=p)?(id+1)*tranche:nb;
    */
   int p=8;
   int nb;
   for(int id=0;id<p;id++)
   { if(fork()==0)
   {
    int tranche=nb/p;
    int debut=id*tranche;
    int fin=(id!=p)?(id+1)*tranche:nb;
    miroir(addr,debut,fin,len/2);
    exit(0);
   }
   }
   for(int id=0;id<p;id++)
   {
    wait(NULL);
   }
    close(fd);

    return 0;
}