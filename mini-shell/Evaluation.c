#include "Evaluation.h"
#include "Shell.h"


#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int valstatus(int s)
{
    if(WIFEXITED(s))
     return WEXITSTATUS(s);
    return WTERMSIG(s) +128;
}

void handleSIGCHILD(int sign)
{
       while (waitpid(-1, NULL, WNOHANG) > 0) {
        printf("Child process terminated\n");
    }
}

void initializeSignalHandler() {
    struct sigaction sa;
    sa.sa_handler = handleSIGCHILD;
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
}

int cmd_simple(Expression *expr) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) { // child process
        if (execvp(expr->argv[0], expr->argv)) {
            perror("Execvp failed.");
            exit(EXIT_FAILURE);
        }
    }
    // parent process
        int status;
        waitpid(pid, &status, 0);
      
    return valstatus(status);
}

void redirect_out(Expression *expr)
{  int fd = open(expr->redirect.fileName, O_CREAT | O_WRONLY | O_TRUNC, 0640);
   if (fd == -1) {
     perror("Error when opening the file");
     exit(EXIT_FAILURE); 
    }
    //redirection
    dup2(fd, 1);
    if ( expr->redirect.fd== -1) 
    { dup2(fd,2);
    }
  close(fd);

}
void redirect_in(Expression *expr)
{ int fd=open(expr->redirect.fileName,O_RDONLY);
  if (fd == -1) {
     perror("Error when opening the file");
     return -1; 
    }

    //redirection
    if (dup2(fd, 0)== -1) 
    {
     perror("Error when redirecting stdout");
     close(fd);
     return -1; 
    }
  close(fd);

}

void redirect_app(Expression *expr)
{ int fd = open(expr->redirect.fileName, O_CREAT | O_WRONLY | O_APPEND, 0644);
   if (fd == -1) {
     perror("Error when opening the file");
     return -1; 
    }
    //redirection
     dup2(fd, 1);
    if ( expr->redirect.fd== -1) 
    { dup2(fd,2);
    }
  close(fd);

}


int evaluateExpr(Expression *expr) {
  static int first = 1;
  int in,out,err,status;
  if (first) {
    // code d'initialisation
    initializeSignalHandler() ;

  }
  if(expr->type == ET_EMPTY)
  {
    return 0;
  }
  else

    if (expr->type == ET_SIMPLE) {
      shellStatus=cmd_simple(expr);
    }
  else 
    if (expr->type == ET_REDIRECT) {
        if(expr->redirect.type==REDIR_OUT)
          {out=dup(1);
          err=dup(2);
          redirect_out(expr);
          expr=expr->left;
          status=evaluateExpr(expr);
          dup2(out,1);
          dup2(err,2);
          close(out);
          close(err);
          return status;
          }
        else
          if(expr->redirect.type==REDIR_IN)
          { in=dup(0);
            redirect_in(expr);
            expr=expr->left;
            status=evaluateExpr(expr);
            dup2(in,0);
            close(in);
            return status;
          }
        else
          if(expr->redirect.type==REDIR_APP)
          {   out=dup(1);
              err=dup(2);
              redirect_app(expr);
              expr=expr->left;
              status=evaluateExpr(expr);
              dup2(out,1);
              dup2(err,2);
              close(out);
              close(err);
              return status;
          }
    }
  else
    if(expr->type==ET_SEQUENCE)
    { 
      status = evaluateExpr(expr->left);
      status= evaluateExpr(expr->right);
      return status;
    }
else
if (expr->type == ET_SEQUENCE_AND) {
    status = evaluateExpr(expr->left);
    if (status == 0) {  
        status = evaluateExpr(expr->right);
    }
    return status;
}

else
  if(expr->type == ET_SEQUENCE_OR)
  {
      status = evaluateExpr(expr->left);
      if(status!=0)
      {
       status= evaluateExpr(expr->right);
      }
      
      return status;
  }
else
  if (expr->type == ET_PIPE) {
        int pipe_fd[2];

        if (pipe(pipe_fd) == -1) {
            perror("pipe");
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            // enfant
            close(pipe_fd[0]);  // Close the read end of the pipe
            dup2(pipe_fd[1], 1); //Redirect
            close(pipe_fd[1]);
            status = evaluateExpr(expr->left);
            exit(status);
        } else {
            // pere
            close(pipe_fd[1]);  // Close the write end of the pipe
            dup2(pipe_fd[0], 0); // Redirect
            close(pipe_fd[0]);
            wait(&status);
            if (WIFEXITED(status)) {
                status = evaluateExpr(expr->right); 
            }
        return status;
      }
  }
else
   if (expr->type == ET_BG) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        setpgid(0, 0);
        status = evaluateExpr(expr->left);
        
      
    }
   return status;
   
}

  else
    {
      fprintf(stderr, "sorry, this shell is not yet implemented\n");
      shellStatus = 1;
    }
      
    first = 0;
  


  return shellStatus;
}
