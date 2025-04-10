#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "pqueue.h"

int compar(const int *a, const int *b) {
  return (*a > *b) - (*a < *b);
}

int compar2(const int *a, const int *b) {
  return -compar(a, b);
}

bool is_even(const int *a, [[maybe_unused]] void *cntxt) {
  return *a % 2 == 0;
}

int main(void) {
  int val[] = {
    0, 1, 3, 2, 6, 9, 10, 4, 2, 5
  };
  int val1[] = {
    0, 1, 3, 2, 6, 9, 10, 4, 2, 5
  };
  int size = sizeof val / sizeof *val;
  pqueue_t *p = pqueue_empty((int (*)(const void *, const void *)) compar);
  if (p == nullptr) {
    fprintf(stderr, "pqueue_empty failed\n");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < size; ++i) {
    if (pqueue_enqueue(p, val + i) != val + i) {
      fprintf(stderr, "pqueue_enqueue failed\n");
      pqueue_dispose(&p);
      return EXIT_FAILURE;
    }
  }
  qsort(val1, (size_t) size, sizeof *val1,
      (int (*)(const void *, const void *)) compar2);
  for (int i = 0; i < size; ++i) {
    int *v = pqueue_dequeue(p);
    if (v == nullptr) {
      fprintf(stderr, "pqueue_dequeue failed\n");
      pqueue_dispose(&p);
      return EXIT_FAILURE;
    }
    if (*v != val1[i]) {
      fprintf(stderr, "pqueue_dequeue failed\n");
      fprintf(stderr, "expected %d, got %d\n", val1[i], *v);
      pqueue_dispose(&p);
      return EXIT_FAILURE;
    }
  }
  for (int i = 0; i < size; ++i) {
    if (pqueue_enqueue(p, val + i) != val + i) {
      fprintf(stderr, "pqueue_enqueue failed\n");
      pqueue_dispose(&p);
      return EXIT_FAILURE;
    }
  }
  pqueue_filter_cntxt(p, (bool (*)(const void *, void *)) is_even, nullptr);
  for (size_t i = 0; i < pqueue_size(p); ++i) {
    int *v = pqueue_dequeue(p);
    if (v == nullptr) {
      fprintf(stderr, "pqueue_dequeue failed\n");
      pqueue_dispose(&p);
      return EXIT_FAILURE;
    }
    if (*v % 2 != 0) {
      fprintf(stderr, "pqueue_filter_cntxt failed\n");
      fprintf(stderr, "expected even, got %d\n", *v);
      pqueue_dispose(&p);
      return EXIT_FAILURE;
    }
  }
  pqueue_dispose(&p);
  return EXIT_SUCCESS;
}
