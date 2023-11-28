
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <semaphore.h>

#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t *forks;
Zem_t *seats;
int size = 0;
int thinktime = 0;
Zem_t seatsAreAvailable;
// int seatsAreAvailable = 1;

void think() { sleep(thinktime); }

void getForks(int id) {
  Zem_wait(&forks[id]);
  Zem_wait(&forks[(id + 1) % size]);
}

void eat() {}

void giveUpForks(int id) {
  Zem_post(&forks[id]);
  Zem_post(&forks[(id + 1) % size]);
  printf("%d\n", id);
}

void enterTable() { Zem_wait(&seatsAreAvailable); }

void exitTable() { Zem_post(&seatsAreAvailable); }

void *philosopher(void *arg) {
  long long int value = (long long int)arg;
  int id = (int)value;
  printf("philosopher %d started \n", id);
  while (1) {
    think();
    enterTable();
    getForks(id);
    eat();
    giveUpForks(id);
    exitTable();
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  size = atoi(argv[1]);
  thinktime = atoi(argv[2]);
  forks = malloc(sizeof(Zem_t) * size);
  seats = malloc(sizeof(Zem_t) * (size));
  int i;
  Zem_init(&seatsAreAvailable, size - 1);
  for (i = 0; i < size; ++i) {
    Zem_init(&(forks[i]), 1);
    Zem_init(&(seats[i]), 1);
    printf("zemaphore %d %p initialized\n", i, &(forks[i]));
  }

  printf("parent: begin\n");
  for (i = 0; i < size; ++i) {
    pthread_t c;
    long long int me = i;
    Pthread_create(&c, NULL, philosopher, (void *)me);
  }
  i = 0;
  while (1) {
    sleep(1);
  }
  printf("parent: finished \n");

  return 0;
}
