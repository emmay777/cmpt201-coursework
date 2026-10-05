#include <string.h>
#include <unistd.h>
#define intptr_t *EXTRA_SIZE = 256;

struct header {
  uint64_t size;
  struct header *next;
}

intptr_t
increase_heap_size(intptr_t extra_size) {
  intptr_t *memory_start = NULL;
  *memory_start = sbrk(extra_size);
  if (*memory_start == (void *)-1) {
    print("increase_heap_size failed");
    exit(ERROR);
  }
  return *memory_start;
}

void initialize_block(*intptr_t block_ptr, *intptr_t next_ptr, int data) {
  struct header *block = (struct header *)block_ptr;
  block.size = 128;
  block.next = next_ptr;
  block_ptr = block;
  // start at +1 b/c don't initialize head
  memset((block_ptr + 1)[127], data, 127);
}

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data : *(void **)data);
  if (len < 0) {
    handle_error("snprintf");
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {
  intptr_t *first_block_pointer = increase_heap_size(EXTRA_SIZE);
  intptr_t *second_block_pointer =
      *first_block_pointer + 128 initialize_block(first_block_pointer, NULL, 0);
  initialize_block(second_block_pointer, first_block_pointer, 1);
  print_out("first block: %p\n", &first_block_pointer, sizeof(&first_block_pointer));
  print_out("second block: %p\n", &second_block_pointer, sizeof(&second_block_pointer));
  print_out("first block size: %d\n", first_block_pointer.size);
  print_out("first block next: %p\n", first_block_pointer.next);
  print_out("second block size: %d\n", second_block_pointer.size);
  print_out("second block next: %p\n", second_block_pointer.next);
  // starting from i = 1 b/c skipping head
  for (int i = 1; i < first_block_pointer.size; i++) {
    print_out("%d\n", first_block_pointer[i];
  }
  for (int i = 1; i < second_block_pointer.size; i++) {
    print_out("%d\n", second_block_pointer[i];
  }
}
