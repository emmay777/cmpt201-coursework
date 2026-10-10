// used help from chatgpt to learn the differences/usage among
// void pointers, pointers of a type, and intptr_t

#define EXTRA_SIZE 256
#define BUF_SIZE 256
#define _DEFAULT_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// header for the two chunks of memory
struct header {
  uint64_t size;
  struct header *next;
};

// step 1: increase the heap
// pointer return type
void *increase_heap_size(intptr_t extra_size) {
  // sbrk returns a void *
  void *memory_start = sbrk(extra_size);
  if (memory_start == (void *)-1) {
    write(STDOUT_FILENO, "increase_heap_size failed\n", 26);
    exit(EXIT_FAILURE);
  }
  return memory_start;
}

// step 2: create the two memory blocks
void *initialize_block(void *block_ptr, void *next_ptr, int data) {
  struct header *block = (struct header *)block_ptr;
  block->size = 128;
  block->next = next_ptr;
  block_ptr = block;
  // don't initialize head, skip it
  // also subtract the size of the head from how many bytes needs to be initialized
  // first argument of memset is address void *
  memset(block_ptr + sizeof(struct header), data, (128 - sizeof(struct header)));
  return block;
}

// step 3: print address, values, data
void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];

  // used chatgpt to help explain the provided print code
  ssize_t len;
  if (data_size == sizeof(uint64_t)) {
    len = snprintf(buf, BUF_SIZE, format, *(uint64_t *)data);
  } else if (data_size == sizeof(char)) {
    len = snprintf(buf, BUF_SIZE, format, *(char *)data);
  } else {
    len = snprintf(buf, BUF_SIZE, format, *(void **)data);
  }
  if (len < 0) {
    write(STDOUT_FILENO, "print failed\n", 13);
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {
  void *memory_pointer = increase_heap_size(EXTRA_SIZE);
  struct header *first_block_pointer = initialize_block(memory_pointer, NULL, 0);
  // memory_pointer + 128 instead of struct + 128 so it's 128 bytes, not 128 of the struct size
  struct header *second_block_pointer =
      initialize_block(memory_pointer + 128, first_block_pointer, 1);
  print_out("first block: %p\n", &first_block_pointer, sizeof(&first_block_pointer));
  print_out("second block: %p\n", &second_block_pointer, sizeof(&second_block_pointer));
  // need type void * so get address with &
  print_out("first block size: %d\n", &first_block_pointer->size, sizeof(uint64_t));
  print_out("first block next: %p\n", &first_block_pointer->next, sizeof(struct header));
  print_out("second block size: %d\n", &second_block_pointer->size, sizeof(uint64_t));
  print_out("second block next: %p\n", &second_block_pointer->next, sizeof(struct header));
  // used chatgpt to help understand how struct/type size changes step size
  // starting from beginning of block + head b/c skipping it
  // want 1 step = 1 byte, typecast to char
  char *first = (char *)first_block_pointer + sizeof(struct header);
  char *second = (char *)second_block_pointer + sizeof(struct header);
  char buf[BUF_SIZE];
  for (int i = 0; i < (128 - sizeof(struct header)); i++) {
    print_out("%d\n", &first[i], sizeof(first[i]));
  }
  for (int i = 0; i < (128 - sizeof(struct header)); i++) {
    print_out("%d\n", &second[i], sizeof(second[i]));
  }
}
