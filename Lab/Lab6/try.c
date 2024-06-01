#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <omp.h>  // Include OpenMP header

#define MAX 16

typedef bool echiquier[MAX][MAX];

static bool ok(int n, int ligne, int colonne, echiquier e);

void nreines(int n, echiquier e, int *cpt);

int main(int argc, char *argv[])
{
  echiquier e;
  int cpt = 0;
  char *endptr;

  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s entier\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int n = strtoul(argv[1], &endptr, 10);

  if (*endptr != 0)
  {
    fprintf(stderr, "Invalid input: %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  memset(e, 0, sizeof(e));

  #pragma omp parallel
  {
    #pragma omp single nowait
    nreines(n, e, &cpt);
  }

  printf("%d\n", cpt);

  return EXIT_SUCCESS;
}

static bool ok(int n, int ligne, int colonne, echiquier e)
{
  int l, c;
  for (l = 0; l < ligne; l++)
    if (e[l][colonne])
      return false;

  for (l = ligne - 1, c = colonne - 1; l >= 0 && c >= 0; l--, c--)
    if (e[l][c])
      return false;

  for (l = ligne - 1, c = colonne + 1; l >= 0 && c < n; l--, c++)
    if (e[l][c])
      return false;

  return true;
}

void nreines(int n, echiquier e, int *cpt)
{
  int col;

  #pragma omp parallel for reduction(+:cpt) private(col)
  for (col = 0; col < n; col++)
  {
    if (ok(n, 1, col, e))
    {
      e[1][col] = true;
      #pragma omp task
      nreines(n, e, cpt);
      e[1][col] = false;
    }
  }

  #pragma omp single nowait
  {
    #pragma omp taskwait
  }
}
