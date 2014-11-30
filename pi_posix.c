/*  Для компиляции исходника необходимо использовать ключ -lrt 
    При запуске объектного файла указываются два параметра - количество процессов 
    для создания и количество точек
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>

#define QUEUE_NAME "/myque"
#define SIZE   256

int main (int argc, char *argv[]) {
  mqd_t ds;
  int iters=atoi(argv[2]);
  int procs=atoi(argv[1]);
  int i,p;
  struct mq_attr queue_attr;
  char text[SIZE];
  float pi;
  int No=0, ob=0;

  queue_attr.mq_maxmsg = 10;
  queue_attr.mq_msgsize = 10;
  queue_attr.mq_flags = 0;
  queue_attr.mq_curmsgs = 0;

  if ((ds = mq_open(QUEUE_NAME, O_CREAT | O_RDWR, 0600, &queue_attr)) == -1) { perror("open"); }

  for (p=0; p<procs;p++){
    if (fork()) {} 
      else {
        float x,y;

        for (i=1;i<=iters/procs;i++) {
          x=((1.0*rand())/RAND_MAX)*2 -1;
          y=((1.0*rand())/RAND_MAX)*2 -1;

          if ((x*x + y*y) < 1) 
            No++;
        }

        //sprintf(text,"%d",No);
        printf(" Send %d\n",No);
        mq_send(ds, (char*) &No, sizeof(int),0);

        exit(0);
      }
  }

    for (p=0; p<procs;p++) {
      int x[1000];

      if ((mq_receive(ds, (char*) x, sizeof(int) * 1000, 0)) == -1) { perror("recieve"); }

      No += *x;
    }

  mq_close(ds);    
  printf("Pi = %0.7f\n", (4.0*No)/iters);
  mq_unlink(QUEUE_NAME);

return 0;
}