#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include <assert.h>
int main(int argc, char *argv[])
{    assert(argc==2);
    int n=atoi(argv[1]);
    
    for(int i=0;i<n;i++)
    {  __pid_t pid=fork();
        if(pid==0)//fils
      {
        printf("Numero %d\n",i);
        exit(0);
      }
    for(int i=0;i<n;i++)
    wait(NULL);

    }
}
//parallelism