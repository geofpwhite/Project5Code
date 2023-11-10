
#include "common.h"
#include "common_threads.h"
#include "zemaphore.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

Zem_t shuttleZemaphore;
Zem_t mutex;
int attendeesWaiting = 0;
int numberOfAttendees;
int shuttleSleepTime;

void *attendee(void *arg) {
  long long int value = (long long int)arg;
  int id = (int)value;
  sleep(id % 20); // make the arrival times vary to see if anything gets screwed up
  Zem_wait(&mutex);
  attendeesWaiting++;
  // printf("\n%d", attendeesWaiting);
  Zem_post(&mutex);
  Zem_wait(&shuttleZemaphore);
  return NULL;
}

int min(int x, int y) {
  if (x < y)
    return x;
  return y;
}

void shuttle() {
  // printf("\nshuttle arrived");
  sleep(shuttleSleepTime);
  Zem_wait(&mutex); // acquiring mutex here and storing value of minimum enforces the
                    // rule that you can't board the shuttle if you arrived in
                    // the line while shuttle is boarding people
  int minimum = min(attendeesWaiting, 30);
  printf("\n%d\n", attendeesWaiting);
  printf("\n%d\n", numberOfAttendees);
  for (int i = 0; i < minimum; ++i) {
    Zem_post(&shuttleZemaphore);
  }
    numberOfAttendees-=minimum;
    attendeesWaiting-=minimum;
  // attendeesWaiting -= minimum; // update attendees waiting
  Zem_post(&mutex);
}

int main(int argc, char *argv[]) {
  Zem_init(&shuttleZemaphore, 30); // shuttle can hold 30 occupants at a time
  Zem_init(&mutex, 1);
  shuttleSleepTime = atoi(argv[2]);
  numberOfAttendees = atoi(argv[1]);
  for (int i = 0; i < numberOfAttendees; ++i) {
    pthread_t c;
    long long int me = i;
    Pthread_create(&c, NULL, attendee, (void *)me);
  }
  while (numberOfAttendees > 0) {
    shuttle();
  }
  printf("\n");
}

// attendees wait for shuttle Zem_wait(&shuttleZemaphore)
// when shuttle arrives, up to 30 of the people currently waiting gets on shuttle 
// if you arrive while people are boarding, 
// shuttle holds 30 people
// shuttle leaves once allowed attendees are boarded
// shuttle leaves immediately if no one is waiting
