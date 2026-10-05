#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// will point to the address of the input string
char *prompt_input() {
  char *buffer = NULL;
  size_t n = 0;
  printf("Enter input: ");
  if (getline(&buffer, &n, stdin) != -1) {
    return buffer;
  } else { // o/w error
    return NULL;
  }
}

// pointer to a pointer (since is array of pointers)
void print_check(char *current_input, char **string_array, int num_lines) {
  if (strcmp(current_input, "print\n") == 0) {
    for (int i = 0; i < num_lines; i++) {
      printf("%s", string_array[i]);
    }
  }
}

int main() {
  int lines = 0;
  // array of 5 pointers
  char *history[5] = {NULL};

  // loop until ctrl + c
  while (1) {
    char *current_input = prompt_input();

    if (lines == 5) { // if max history inputs get rid of the earliest one
      free(history[0]);
      for (int i = 0; i < 4; i++) { // update the pointers to make space for new one
        history[i] = history[i + 1];
      }
      lines--; // decrement counter
    }

    // add newest input
    history[lines] = current_input;
    lines++;

    // check if user did print
    print_check(current_input, history, lines);
  }
  // free mem bfore exiting
  for (int i = 0; i < lines; i++) {
    free(history[i]);
  }
  return 0;
}
