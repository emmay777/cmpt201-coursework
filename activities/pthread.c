#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void *t_func(void *string_arg) {
  char *msg = string_arg; // defines void *

  // get thread's own ID
  pid_t tid = gettid();

  // print supplied string argument and own ID
  printf("%s\n", msg);
  printf("own thread ID: %d", tid);

  // return length of received string
  return (void *)strlen(msg);
}

int main() {
  // create pointer for new thread and return values
  pthread_t thread; // address of thread to be passed in later
  int create_ret_val = -1;
  void *join_ret_val = 0;

  // create + wait for thread to finish, store return values
  if (pthread_create(&thread, NULL, t_func, "hola") != 0) {
    perror("create thread failed");
  }
  if (pthread_join(thread, &join_ret_val) != 0) {
    perror("join thread failed");
  }

  printf("return value of pthread_create: %d\n", create_ret_val);
  // need to get out of void * and define the type
  // can typecast uint64_t retBal = (uint64_t) thread)ret;
  // or do it in printf with %lu and then typecast
  printf("return value of pthread_join: %d\n", (int)join_ret_val);

  return 0;
}
