#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <assert.h>

#include "error.h"

void quelques_prints (void)
{
  printf ("Juste");
  printf (" histoire");
  printf (" de tester...\n");
  printf ("...que la redirection");
  printf (" marche !\n");
}

void rediriger_vers (void (*f)(void), char *file)
{ int fd_out = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);//open le fichier pour la redirection de la sdtout
  int stdout=dup(1);//on fait un copie de stdout
  if(fd_out==-1)
  {
    perror("Error when opening the file");
  }
  if(dup2(fd_out,1)==-1)//on redirect la sortie std
   {
    perror("Error when redirecting stdout");
   }
   close(fd_out);
   f();//on execute la fonction
   if(dup2(stdout,1)==-1)//on retablisse la sortie std
   {
    perror("Error when redirecting the file");

   }
   
   close(stdout);
  
}

int main(int argc, char *argv[])
{
  printf ("*** DEBUT ***\n");

  rediriger_vers (quelques_prints, "sortie.txt");

  printf ("*** FIN ***\n");

  return EXIT_SUCCESS;
}
  
