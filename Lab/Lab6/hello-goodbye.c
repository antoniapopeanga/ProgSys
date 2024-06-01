#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

void *HelloGoodbye(void *p)
{ intptr_t n_thread=(intptr_t)p;
  printf("%ld: bonjour\n", n_thread); 
  usleep(500000);
  printf("%ld: a bientot\n", n_thread); 

  return NULL;
}

int 
main(int argc, char *argv[])
{ 
  /*pthread_t pid;
  HelloGoodbye(NULL);
  pthread_create(&pid, NULL, HelloGoodbye, (void*)pid);
  pthread_join(pid,NULL);*/
  assert(argc==2);
  int nth=atoi(argv[1]);
  pthread_t pidth[nth];
  for(int i=0; i<nth;i++)
  {
    pthread_create(&pidth[i],NULL,HelloGoodbye,(void*)(intptr_t)i);
  }

  for(int i=0;i<nth;i++)
  {
    pthread_join(pidth[i],NULL);
  }


  return EXIT_SUCCESS;
}
