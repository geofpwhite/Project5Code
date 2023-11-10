
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"

Zem_t* forks;
Zem_t mutex;
int thinktime;
int size = 0 ;
void think(){
  sleep(thinktime);
}
void getForks(int id){
  if (id==0){
    Zem_wait(&forks[id+1 % size]);
    Zem_wait(&forks[id]);
  }else{
    Zem_wait(&forks[id]);
    Zem_wait(&forks[id+1 % size]);
  }
  printf("done waiting \n");
}

void eat(int id){
  sleep(10);
}

void giveUpForks(int id){
  Zem_post(&forks[id]);
  Zem_post(&forks[id+1%size]);
}

void *philosopher(void *arg) {
  long long int value = (long long int) arg;
  int id = (int) value;
  printf("philosopher %d started \n",id);
  while(1){
    // think();
    printf("\nffff");
    
    // getForks(id);
    Zem_wait(&forks[id]);
    printf("got fork %d\n",id);
    Zem_wait(&forks[(id+1) % size]);
    printf("got fork %d\n",id+1%size);
    // Zem_post(&mutex);
    printf("done waiting \n");
    printf("\n %d got their forks",id);
    // eat(id);
    // Zem_wait(&mutex);
    // giveUpForks(id);
    Zem_post(&forks[id]);
    Zem_post(&forks[(id+1)%size]);
    printf("\n %d gave up their forks",id);
  }

  printf("%d done\n",id);

  return NULL;
}

int main (int argc, char* argv[]) {
  // Zem_t forks[atoi(argv[1])];
  size = atoi(argv[1]);
  forks = malloc(sizeof(Zem_t) * size);
  thinktime = atoi(argv[2]);
  int i ;
  Zem_init(&mutex,1);
  for (i = 0 ; i < size; ++i){
    Zem_init(&(forks[i]),1) ;
    printf("zemaphore %d %p\n",i,&(forks[i]));
  }

  printf("\n%p",&(forks[0]));
  printf("\n%p",(&forks)[0]);
  printf("parent: begin\n");
  pthread_t threads[size];
  void* mes[size];
  for ( i = 0 ; i < size ; ++i){
    pthread_t c;
    long long int me = i;
    threads[i] = c;
    mes[i] = (void *) me;
    // sleep(1);
    Pthread_create(&c, NULL,philosopher, (void *) me);
  }

  // for ( i = 0 ; i < size ; ++i){
  //   Pthread_join(threads[(long long int)i], (void *) (long long int) i);
  // }
  while(1){
    sleep(10);
  }
  printf("parent: finished \n");
  
  return 0;
}
