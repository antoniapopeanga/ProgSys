#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>

#include "error.h"
int main()
{
printf("hello\n");
write(1,"world", 5);
//il affiche premierment world et apres hello
//si on remplece hello by hello\n, l'ordre est printf et apres wrire
return 1;
}