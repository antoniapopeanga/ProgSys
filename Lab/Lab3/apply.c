#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/wait.h>
#include <string.h>
int valstatus(int s)
{
    if(WIFEXITED(s))
     return WEXITSTATUS(s);
    return WTERMSIG(s) +128;
}
int execute(char*cmd,char *name)
{ pid_t pid=fork();
    if(pid==0){
    execlp(cmd, cmd, name, NULL);
    perror(cmd);
    exit(2);
    }
    else{
    int status;
    waitpid(pid,&status,0);
    return(valstatus(status));
    }
    
}
int main(int argc,char*argv[])

{   char*cmd =argv[1];
    DIR *dir = opendir(".");

    if (!dir) {
        perror("opendir");
        exit(2);
    }

    char *motif = argv[argc-1];
    struct dirent *entry;
    while(entry=readdir(dir))
    {  if(fnmatch(motif,entry->d_name,0)==0)
        {
          if(execute(cmd,entry->d_name))
            exit(1);
        }

    }
  closedir(dir);
  return 0;

}