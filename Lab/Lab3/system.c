#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>

void System(char *commande)
{
    pid_t pid=fork();
    if(pid==0)
    {
    printf("Executer la commande %s\n", commande);
    execl("/bin/sh","sh","-c",commande,NULL);
    perror("bin/sh");
    exit(2);
    }
       waitpid(pid,NULL,0);//on attend que le processus est fini
}


int main()
{   if(fork()==0)
      exit(0);
    System("echo bonjour"); 
    System("echo au revoir");
    //if (!fork()) 
    //exit(0); 
    System("sleep 1 ; echo bonjour"); 
    System("echo au revoir");
}

/*void System(char *commande)
{
    
    execl("/bin/sh","sh","-c",commande,NULL);
    --seulement la premier appel system
}*/