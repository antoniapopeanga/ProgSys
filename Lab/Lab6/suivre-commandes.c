#define _GNU_SOURCE

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <pthread.h>

#define NCOMMANDES 4
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
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



void modifier_etat(pid_t pid)
{
  pthread_mutex_lock(&mutex);

  for (int i = 0; i < NCOMMANDES; i++)
    if (etat_tableau[i].pid == pid)
    {
      etat_tableau[i].en_cours = 0;
      gettimeofday(&etat_tableau[i].fin, NULL);
      break;
    }

  pthread_mutex_unlock(&mutex);
}

void afficher_etat()
{
  pthread_mutex_lock(&mutex);

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

  pthread_mutex_unlock(&mutex);
}

int reste_commande()
{
  pthread_mutex_lock(&mutex);

  for (int i = 0; i < NCOMMANDES; i++)
    if (etat_tableau[i].en_cours)
    {
      pthread_mutex_unlock(&mutex);
      return 1;
    }

  pthread_mutex_unlock(&mutex);
  return 0;
}

void *thread_zombies_handler(void *arg)
{
  while (reste_commande())
  {
    pid_t w = waitpid(0, NULL, WNOHANG);
    if (w > 0)
      modifier_etat(w);
  }

  return NULL;
}

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
      abort();
    }

    pthread_mutex_lock(&mutex);

    etat_tableau[i].pid = cpid;
    strcpy(etat_tableau[i].commande, commandes[i][0]);
    strcpy(etat_tableau[i].arg, commandes[i][1]);
    etat_tableau[i].en_cours = 1;
    gettimeofday(&etat_tableau[i].debut, NULL);

    pthread_mutex_unlock(&mutex);
  }
}

int main(int argc, char *argv[])
{
  pthread_t thread;
  int pthread_create_status = pthread_create(&thread, NULL, thread_zombies_handler, NULL);
  if (pthread_create_status != 0)
  {
    fprintf(stderr, "Erreur lors de la création du thread.\n");
    exit(EXIT_FAILURE);
  }

  lancer_commandes();

  for (int cpt = 1; reste_commande(); cpt++)
  {
    char buf[1024];

    int r = read(0, buf, 1024);
    if (r == -1)
      perror("read");

    afficher_etat();
  }

  pthread_join(thread, NULL);

  printf("Tous les processus se sont terminés !\n");
  afficher_etat();

  exit(EXIT_SUCCESS);
}
