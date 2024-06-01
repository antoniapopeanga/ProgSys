#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

#include "error.h"
void copy(FILE* f1, FILE* f2){
char c;
while(fread(&c,sizeof(char),1,f1)!=0)
   fwrite(&c,sizeof(char),1,f2);
}
int main(int argc, char **argv)
{  assert(argc==3);
  FILE* f1;
  FILE* f2;
  f1=fopen("file.txt","r");
  f2 = fopen("file2.txt", "w");
  if (f1 == NULL || f2 == NULL) {
        perror("Error opening files");
        return EXIT_FAILURE;
    }
  copy(f1,f2);
  fclose(f1);
  fclose(f2);
  return EXIT_SUCCESS;
}
