#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  // allocate buffer
  char *buffer = NULL;
  size_t buffer_size = 0;

  printf("Please enter some text: ");
  if ((getline(&buffer, &buffer_size, stdin)) > -1) {
    // tokenize string
    char *input = buffer;
    char *delim = " ";
    char *token = NULL;
    char *saveptr = NULL;

    printf("Tokens:");

    while ((token = strtok_r(input, delim, &saveptr))) {
      printf("%s\n", token);
      input = NULL;
    }
  } else {
    printf("failed :(");
  }
  // free buffer
  free(buffer);
  return 0;
}
