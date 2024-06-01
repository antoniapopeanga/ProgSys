#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

unsigned long volatile k = 0;
const unsigned long MAX = 100 * 1000;
pthread_mutex_t mutex;
void *for_en_parallele(void *p)
{
  for(unsigned long i=0; i < MAX; i++)
    {   pthread_mutex_lock(&mutex);
        k++;
        pthread_mutex_unlock(&mutex);
    }
  return NULL;
}

int main(int argc, char *argv[])
{   

  int n = atoi(argv[1]);
  pthread_t tids[n];
  pthread_mutex_init(&mutex, NULL);
 
  for(int i = 0; i <n ; i++)
    pthread_create(tids + i, NULL, for_en_parallele, NULL);

  for(int i = 0; i <n ; i++)
    pthread_join(tids[i],NULL);

  printf("%lu\n",k);

  return EXIT_SUCCESS;
}

// 1. on peut avoir valeurs entre 100.000 et 100.000*MAX, parce que on perde des incrementations 
//quand 2 threads esseyent d'ecrire au meme temps
// 2. c'est 100.000