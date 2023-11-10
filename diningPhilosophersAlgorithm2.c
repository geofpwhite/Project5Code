
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <semaphore.h>

#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t *forks;
int size = 0;
int thinktime = 0;
Zem_t mutex;
void think() { sleep(thinktime); }

void getForks(int id){
  
  Zem_wait(&mutex);
  Zem_wait(&forks[id]);
  Zem_wait(&forks[(id + 1) % size]);
  Zem_post(&mutex);
}

void eat() {  }

void giveUpForks(int id) {
  Zem_post(&forks[id]);
  Zem_post(&forks[(id + 1) % size]);
  printf("\n%d",id);
}

void *philosopher(void *arg) {
  long long int value = (long long int)arg;
  int id = (int)value;
  printf("philosopher %d started \n", id);
  while (1) {
    think();
    getForks(id);
    eat();
    giveUpForks(id);
  }


  return NULL;
}

int main(int argc, char *argv[]) {
  size = atoi(argv[1]);
  thinktime = atoi(argv[2]);
  forks = malloc(sizeof(Zem_t) * size);
  int i;
  Zem_init(&mutex,1);
  for (i = 0; i < size; ++i) {
    Zem_init(&(forks[i]), 1);
    printf("zemaphore %d %p\n", i, &(forks[i]));
  }

  printf("parent: begin\n");
  for (i = 0; i < size; ++i) {
    pthread_t c;
    long long int me = i;
    Pthread_create(&c, NULL, philosopher, (void *)me);
  }
  i=0;
  while(1){
    sleep(1);
  }
  printf("parent: finished \n");

  return 0;
}
