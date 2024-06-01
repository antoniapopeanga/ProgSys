#define _GNU_SOURCE

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>

#define NCOMMANDES 4

// Returns duration in secs
#define TIME_DIFF(t1, t2) \
  ((t2.tv_sec - t1.tv_sec) + ((double)(t2.tv_usec - t1.tv_usec)) / 1000000)

struct etat
{
  pid_t pid;
  char commande[100];
  char arg[100];
  int en_cours;
  struct timeval debut;
  struct timeval fin;
} etat_tableau[NCOMMANDES];

char *commandes[NCOMMANDES][10] = {
    {"sleep", "0", NULL},
    {"sleep", "3", NULL},
    {"sleep", "4", NULL},
    {"sleep", "5", NULL}};

// Function to update the state of a process when it finishes
void modifier_etat(pid_t pid)
{
  for (int i = 0; i < NCOMMANDES; i++)
    if (etat_tableau[i].pid == pid)
    {
      etat_tableau[i].en_cours = 0;
      gettimeofday(&etat_tableau[i].fin, NULL);
      return;
    }
  fprintf(stderr, "%d non enregistré\n", pid);
}

// Function to display the state of all processes
void afficher_etat()
{
  int i;
  struct timeval now;
  gettimeofday(&now, NULL);
  for (i = 0; i < NCOMMANDES; i++)
  {
    if (etat_tableau[i].pid > 0)
    {
      printf("%d : %s(%s)", etat_tableau[i].pid, etat_tableau[i].commande, etat_tableau[i].arg);
      if (etat_tableau[i].en_cours)
        printf(" en cours depuis : %gs\n", TIME_DIFF(etat_tableau[i].debut, now));
      else
        printf(" terminé durée détectée : %gs\n", TIME_DIFF(etat_tableau[i].debut, etat_tableau[i].fin));
    }
  }
  printf("\n");
}

// Function to check if there are remaining commands
int reste_commande()
{
  for (int i = 0; i < NCOMMANDES; i++)
    if (etat_tableau[i].en_cours)
      return 1;
  return 0;
}

// Function to launch commands
void lancer_commandes()
{
  int i;
  pid_t cpid;

  /* Lancement */
  for (i = 0; i < NCOMMANDES; i++)
  {
    cpid = fork();

    if (cpid == -1)
    {
      perror("fork");
      exit(EXIT_FAILURE);
    }

    if (cpid == 0)
    {
      execvp(commandes[i][0], commandes[i]);
      perror(commandes[i][0]);
      exit(EXIT_FAILURE);
    }

    etat_tableau[i].pid = cpid;
    strcpy(etat_tableau[i].commande, commandes[i][0]);
    strcpy(etat_tableau[i].arg, commandes[i][1]);
    etat_tableau[i].en_cours = 1;
    gettimeofday(&etat_tableau[i].debut, NULL);
  }
}

// Function to handle SIGCHLD signal
void traiter_SIGCHILD()
{
  pid_t w;
  while (waitpid(-1, NULL, WNOHANG) > 0) // wait for all child processes
    modifier_etat(w);
}

int main(int argc, char *argv[])
{
  struct sigaction sa;
  sa.sa_handler = traiter_SIGCHILD;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  sigaction(SIGCHLD, &sa, NULL); // Register signal handler for SIGCHLD

  lancer_commandes();

  for (int i = 0; i < NCOMMANDES; i++)
  {
    sigset_t mCHILD;
    sigemptyset(&mCHILD);
    sigaddset(&mCHILD, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mCHILD, NULL); // Block SIGCHLD signal
  }

  while (reste_commande())
  {
    pid_t w;
    char buf[1024];

    w = wait(NULL); // Wait for any child process
    if (w > 0)
    {
      modifier_etat(w);
    }

    int r = read(0, buf, 1024);
    if (r == -1)
      perror("read");
    afficher_etat();
  }

  printf("Tous les processus se sont terminés !\n");
  afficher_etat();
  exit(EXIT_SUCCESS);
}
