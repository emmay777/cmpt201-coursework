#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static pthread_mutex_t lockA_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lockB_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *func0(void *arg) {
  pthread_mutex_lock(&lockA_mutex);
  printf("thread0: locked A\n");
  pthread_mutex_lock(&lockB_mutex);
  printf("thread0: locked B\n");
  pthread_mutex_unlock(&lockA_mutex);
  pthread_mutex_unlock(&lockB_mutex);
  printf("thread0: unlocked A then B\n");
  pthread_exit(0);
}

static void *func1(void *arg) {
  pthread_mutex_lock(&lockB_mutex);
  printf("thread1: locked B\n");
  pthread_mutex_lock(&lockA_mutex);
  printf("thread1: locked A\n");
  pthread_mutex_unlock(&lockB_mutex);
  pthread_mutex_unlock(&lockA_mutex);
  printf("thread1: unlocked A then B\n");
  pthread_exit(0);
}

int main() {
  pthread_t thread0;
  pthread_t thread1;

  if (pthread_create(&thread0, NULL, func0, NULL) != 0) {
    perror("create thread failed");
  }
  if (pthread_create(&thread1, NULL, func1, NULL) != 0) {
    perror("create thread failed");
  }
  if (pthread_join(thread0, NULL) != 0) {
    perror("join thread failed");
  }
  if (pthread_join(thread1, NULL) != 0) {
    perror("join thread failed");
  }

  return 0;
}
