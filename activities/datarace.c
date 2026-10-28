#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// global var
int cnt = 0;

void *addCount() {
  for (int i = 0; i < 10000000; i++) {
    cnt++;
  }
  // don't need thread to return aything
  return NULL;
}

int main() {
  pthread_t thread1; // address of thread to be passed in later
  pthread_t thread2;

  // create + wait for threads in SEQUENCE instead of in PARALLEL
  if (pthread_create(&thread1, NULL, addCount, NULL) != 0) {
    perror("create thread failed");
  }
  if (pthread_join(thread1, NULL) != 0) {
    perror("join thread failed");
  }

  if (pthread_create(&thread2, NULL, addCount, NULL) != 0) {
    perror("create thread failed");
  }
  if (pthread_join(thread2, NULL) != 0) {
    perror("join thread failed");
  }

  // print cnt after waiting for the two threads
  printf("cnt: %d\n", cnt);

  return 0;
}
