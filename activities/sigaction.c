#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *message = "CTRL-C pressed \n";
void handle_sigint(int signum) { write(STDOUT_FILENO, message, strlen(message)); }

int main() {
  struct sigaction act;
  act.sa_handler = handle_sigint;
  act.sa_flags = 0;
  sigemptyset(&act.sa_empty);

  // register signal handler
  if (sigaction(SISGINT, &act, NULL) == -1) {
    perror("Sigaction() failed);
    exit(EXIT_FAILURE);
  }

  while (true) {
    sleep(1);
  }
}

int main() {
  struct action {
    void (*sa_handler)(int);
    int sa_flags;
    sigset_t sa_mask;
  };
  struct action p_act;
  p_act.sa_handler = sleep_loop;
  p_act.sa_flags = 0;
  p_act.sa_empty = sigemptyset();
  while (1) {
    if (sigaction(SIGINT, p_act, p_act) != 0) {
      write(STDOUT_FILENO, "CTRL-C pressed\n", 1);
      break;
    }
    sleep();
  }
  return 0;
}
