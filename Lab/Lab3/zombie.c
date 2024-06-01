#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char*argv[])
{
    __pid_t pid=fork();
    if(pid)
    {   sleep(3);
        printf("je m'appelle %d et je suis le père de %d\n",getpid(),pid);
    }
    else
    {   
        printf("je m'appelle %d et je suis le fils de %d\n",getpid(),getppid());
    }
}
