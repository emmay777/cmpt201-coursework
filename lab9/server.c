#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 64
#define PORT 8000
#define LISTEN_BACKLOG 32

#define handle_error(msg)                                                                          \
  do {                                                                                             \
    perror(msg);                                                                                   \
    exit(EXIT_FAILURE);                                                                            \
  } while (0)

// Shared counters for: total # messages, and counter of clients (used for
// assigning client IDs)
int total_message_count = 0;
int client_id_counter = 1;

// Mutexs to protect above global state.
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t client_id_mutex = PTHREAD_MUTEX_INITIALIZER;

struct client_info {
  int cfd;
  int client_id;
};

void *handle_client(void *arg) {
  struct client_info *client = (struct client_info *)arg;

  char buffer[BUF_SIZE + 1];
  ssize_t num_read;
  while ((num_read = read(client->cfd, buffer, BUF_SIZE)) > 0) {
    buffer[num_read] = '\0';

    // lock mutex before changing/accessing global variable
    pthread_mutex_lock(&count_mutex);

    // TODO: print the message received from client
    printf("client id: %d\ntotal # of messages: %d\n, message: %s", client->client_id,
           total_message_count, buffer);

    // TODO: increase total_message_count per message
    total_message_count++;

    // unlock mutex
    pthread_mutex_unlock(&count_mutex);
  }
  return NULL;
}

int main() {
  struct sockaddr_in addr;
  int sfd;

  sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sfd == -1) {
    handle_error("socket");
  }

  memset(&addr, 0, sizeof(struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
    handle_error("bind");
  }

  if (listen(sfd, LISTEN_BACKLOG) == -1) {
    handle_error("listen");
  }

  for (;;) {
    // TODO: create a new thread when a new connection is encountered
    int fd = accept(sfd, NULL, NULL);
    if (fd == -1) {
      handle_error("accept()");
    }
    struct client_info *client = malloc(sizeof(struct client_info));
    if (client = NULL) {
      handle_error("struct malloc()");
    }
    client->cfd = fd;

    // lock mutex for updating global variable
    pthread_mutex_lock(&client_id_mutex);
    client->client_id = client_id_counter++;
    pthread_mutex_unlock(&client_id_mutex);

    pthread_t client_thread;
    if (pthread_create(&client_thread, NULL, handle_client, client) == -1) {
      handle_error("create thread");
    }
    // TODO: call handle_client() when launching a new thread, and provide
    // client_info
    handle_client(sfd);
    printf("client ID: %d", client_id_counter);

    // pthread_detach instead of pthread_join
    pthread_detach(client_thread);
  }

  if (close(sfd) == -1) {
    handle_error("close");
  }

  return 0;
}
