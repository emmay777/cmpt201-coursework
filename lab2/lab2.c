#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// get input of path of program to run
// execute command
// repeat 2 steps forever
// use fork(), exec(), waitpid()

int main() {
  char *p_command = NULL;
  size_t buffer_size = 0;
  while (true) {
    // loop this part forever
    printf("enter programs to run:");
    // same concept as lab 1
    if ((getline(&p_command, &buffer_size, stdin)) > -1) {
      // printf("debug: %s", p_command);
      char *p_delim = "\n";
      char *p_saveptr = NULL;
      p_command = strtok_r(p_command, p_delim, &p_saveptr);
      // printf("debug: %s", p_command);
    }
    // fork the exec so child becomes new process and parent still contains while loop w/ no exec
    pid_t pid = fork();
    if (pid > 0) {
      int wstatus = 0;
      if (waitpid(pid, &wstatus, 0) == -1) {
        perror("waitpid");
        exit(EXIT_FAILURE);
      }
    } else {
      if (execlp(p_command, p_command, NULL) == -1) {
        perror("execl");
        exit(EXIT_FAILURE);
      }
    }
  }
  free(p_command);
  return 0;
}
