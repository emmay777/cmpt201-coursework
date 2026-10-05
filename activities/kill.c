#define _POSIX_C_SOURCE 199309L
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *message = "CTRL-C pressed";
void handle_sigint(int signum) { write(STDOUT_FILENO, message, strlen(message)); }

int main() {

  pid_t pid = fork();

  struct action {
    void (*sa_handler)(int);
    int sa_flags;
    sigset_t sa_mask;
  };
  if (fork() != 0) {
    // parent

    struct sigaction act;
    act.sa_handler = handle_sigint;
    act.sa_flags = 0;
    sigemptyset(&act.sa_empty);

    // register signal handler
    int ret = sigaction(SIGINT, &act, NULL);
    if (ret == -1) {
      perror("sigaction() failed");
      exit(EXIT_FAILURE);
    }
    printf("parent dozing\n");

    while (1) {
      sleep(1);
    }
  } else {
    // child
    // infinite loop
    while (1) {
      sleep(3);
      if (kill(getppid(), SIGINT) == -1) {
        perror("unable to send signal to parent.");
        exit(EXIT_FAILURE);
      }
    }
  }
}
