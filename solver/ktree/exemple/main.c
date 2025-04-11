#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "ktree.h"

#define NB_VAL 15

int main(void) {
  ktree_t *k = ktree_empty();
  if (k == nullptr) {
    fprintf(stderr, "Error: Not enough memory to create the K-Tree.\n");
    return EXIT_FAILURE;
  }
  int vals[NB_VAL] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
  };
  size_t ind[NB_VAL] = {
    0
  };
  size_t count = 0;
  if ((ind[count] = ktree_insert(k, SIZE_MAX, vals + count)) == SIZE_MAX) {
    fprintf(stderr, "Error: Not enough memory to insert the value \"%d\" into "
        "the K-Tree.\n", *(vals + count));
    ktree_dispose(&k);
    return EXIT_FAILURE;
  }
  ++count;
  for (; count < 4; ++count) {
    if ((ind[count] = ktree_insert(k, ind[0], vals + count)) == SIZE_MAX) {
      fprintf(stderr,
          "Error: Not enough memory to insert the value \"%d\" into "
          "the K-Tree.\n", *(vals + count));
      ktree_dispose(&k);
      return EXIT_FAILURE;
    }
  }
  for (; count < 6; ++count) {
    if ((ind[count] = ktree_insert(k, ind[1], vals + count)) == SIZE_MAX) {
      fprintf(stderr,
          "Error: Not enough memory to insert the value \"%d\" into "
          "the K-Tree.\n", *(vals + count));
      ktree_dispose(&k);
      return EXIT_FAILURE;
    }
  }
  for (; count < 8; ++count) {
    if ((ind[count] = ktree_insert(k, ind[2], vals + count)) == SIZE_MAX) {
      fprintf(stderr,
          "Error: Not enough memory to insert the value \"%d\" into "
          "the K-Tree.\n", *(vals + count));
      ktree_dispose(&k);
      return EXIT_FAILURE;
    }
  }
  for (; count < 10; ++count) {
    if ((ind[count] = ktree_insert(k, ind[3], vals + count)) == SIZE_MAX) {
      fprintf(stderr,
          "Error: Not enough memory to insert the value \"%d\" into "
          "the K-Tree.\n", *(vals + count));
      ktree_dispose(&k);
      return EXIT_FAILURE;
    }
  }
  for (; count < 15; ++count) {
    if ((ind[count] = ktree_insert(k, ind[4], vals + count)) == SIZE_MAX) {
      fprintf(stderr,
          "Error: Not enough memory to insert the value \"%d\" into "
          "the K-Tree.\n", *(vals + count));
      ktree_dispose(&k);
      return EXIT_FAILURE;
    }
  }
  assert(ktree_get_root_num(k) == ind[0]);
  assert(ktree_get_first_child(k, ind[0]) == ind[3]);
  assert(ktree_get_first_child(k, ind[10]) == SIZE_MAX);
  assert(ktree_get_neighbor(k, ind[1]) == SIZE_MAX);
  assert(ktree_get_neighbor(k, ind[2]) == ind[1]);
  assert(ktree_get_neighbor(k, ind[3]) == ind[2]);
  assert(ktree_get_ref_by_num(k, ind[0]) == vals);
  for (size_t n = 1; n < 4; ++n) {
    assert(ktree_get_ref_by_num(k, ind[n]) == vals + n);
    assert(ktree_get_parent_by_num(k, ind[n]) == ind[0]);
  }
  assert(ktree_get_parent_by_num(k, ind[0]) == SIZE_MAX);
  assert(ktree_is_descendant(k, ind[0], ind[1]));
  assert(ktree_is_descendant(k, ind[0], ind[4]));
  assert(ktree_is_descendant(k, ind[0], ind[10]));
  assert(ktree_is_descendant(k, ind[1], ind[11]));
  assert(!ktree_is_descendant(k, ind[1], ind[0]));
  ktree_dispose(&k);
  return EXIT_FAILURE;
}
