#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next, int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  int first_fit_id = -1;
  // traverse through the linked list,
  // return the id of the first block big enough
  struct header *current = free_list_ptr;
  while (current != NULL) {
    if (current->size >= size) {
      first_fit_id = current->id;
      return first_fit_id;
    } else {
      current = current->next;
    }
  }
  // if gone through the list but no fit, return -1
  return -1;
}

int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  // traverse through the whole linked list,
  // if the block big enough,
  // record down the extra size leftover
  // return the id of the block with the smallest leftover
  uint64_t leftover = UINT64_MAX;
  struct header *best_fit = NULL;
  struct header *current = free_list_ptr;
  while (current != NULL) {
    // keep track of smallest leftover so far and block that corresponds to it
    if ((current->size >= size) && (current->size < leftover)) {
      leftover = current->size;
      best_fit = current;
    }
    current = current->next;
  }
  // if no fit, then return -1
  if (best_fit == NULL) {
    return -1;
  }
  best_fit_id = best_fit->id;
  return best_fit_id;
}

int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  // similar to finding best fit
  // traverse through the whole linked list,
  // if the block big enough, record leftover
  // return the id of the block with the biggest leftover
  uint64_t leftover = 0;
  struct header *worst_fit = NULL;
  struct header *current = free_list_ptr;
  while (current->next != NULL) {
    // keep track of largest leftover + corresponding block
    if ((current->size >= size) && (current->size > leftover)) {
      leftover = current->size;
      worst_fit = current;
    }
    current = current->next;
  }
  // if no fit, then return -1
  if (worst_fit == NULL) {
    return -1;
  }
  worst_fit_id = worst_fit->id;
  return worst_fit_id;
}

// based on the linked list youtube video from the pre-req section
void freeBlocks(struct header **block) {
  struct header *current = *block;
  while (current != NULL) {
    struct header *next = current->next;
    free(current);
    current = next;
  }
  *block = NULL;
}

int main(void) {

  struct header *free_block1 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block2 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block3 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block4 = (struct header *)malloc(sizeof(struct header));
  struct header *free_block5 = (struct header *)malloc(sizeof(struct header));

  if (free_block1 == NULL || free_block2 == NULL || free_block3 == NULL || free_block4 == NULL ||
      free_block5 == NULL) {
    perror("malloc failed");
    exit(EXIT_FAILURE);
  }

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  // print out the ids
  printf("ID for first-fit algorithm: %d\n", first_fit_id);
  printf("ID for best-fit algorithm: %d\n", best_fit_id);
  printf("ID for worst-fit algorithm: %d\n", worst_fit_id);

  // free memory
  freeBlocks(&free_block1);

  return 0;
}

/*
 * part 2: coalescing contiguous free blocks
 *
 * using pointer arithmetic
 * traverse through the list
 *
 * current = header block
 * previous = NULL
 *
 * while (current != NULL) {
 *
 *   if (current address + current->size == newly freed address) {
 *      then newly freed is after the existing free block
 *      current->size += newly freed->size
 *      newly freed = current
 *   } else if (newly freed address + newly freed->size == current address) {
 *     then newly freed is before the existing free block
 *     newly freed->size += current->size
 *     if (previous == NULL) {
 *       header block = current->next
 *     } else {
 *       previous->next = current->next
 *     }
 *   }
 *
 * continue traversing
 * previous = current
 * current = current->next
 * }
 *
 * if newly freed block not handled after traversing
 * append newly freed block to LL
 *
 */
