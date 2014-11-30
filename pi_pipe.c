

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[]) {
  int iters = atoi(argv[2]);
  int procs = atoi(argv[1]);
  int i, p;
  int fd[2];

float pi;
int No = 0,ob = 0;

pipe(fd);

for (p = 1;p <= procs;p++){
  if (fork()) {} 
    else {
      float x, y;
      for (i = 1;i <= iters/procs;i++) {
        x=((1.0*rand())/RAND_MAX)*2 -1;
        y=((1.0*rand())/RAND_MAX)*2 -1;

        if ((x*x + y*y) < 1) 
          No++;
      }
      write (fd[1], &No, sizeof(int));
      exit(0);
    }
  }

  for (p = 1;p <= procs;p++) {
    int x;
    read (fd[0], &x, sizeof(int));
    No += x;
  }

  pi = (4.0*No)/iters;
  printf("Pi = %0.7f\n", pi);
  return 0;
}
