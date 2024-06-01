#define _XOPEN_SOURCE 600

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

pthread_barrier_t barrier;

void *HelloGoodbye(void *p)
{ int n_thread=(intptr_t)p;
  printf("%d: bonjour\n", n_thread); 
  pthread_barrier_wait(&barrier);
  printf("%d: a bientot\n", n_thread); 

  return NULL;
}

int 
main(int argc, char *argv[])
{
 
  assert(argc==2);
  int nth=atoi(argv[1]);
  pthread_t pidth[nth];
  pthread_barrier_init(&barrier,NULL,nth+1);
  for(int i=0; i<nth;i++)
  {
    pthread_create(&pidth[i],NULL,HelloGoodbye,(void*)i);
  }


  pthread_barrier_wait(&barrier);

  for(int i=0;i<nth;i++)
  {
    pthread_join(pidth[i],NULL);
  }

pthread_barrier_destroy(&barrier);
  return EXIT_SUCCESS;
}
