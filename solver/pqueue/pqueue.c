#include "pqueue.h"

#include <stdio.h>

typedef struct cell_t cell_t;

struct cell_t {
  const void *ref;
  cell_t *next;
};

struct pqueue_t {
  cell_t *head;
  size_t size;
  int (*compar)(const void *, const void *);
};

pqueue_t *pqueue_empty(int (*compar)(const void *, const void *)) {
  pqueue_t *p = malloc(sizeof *p);
  if (p == nullptr) {
    return nullptr;
  }
  p->head = nullptr;
  p->size = 0;
  p->compar = compar;
  return p;
}

void pqueue_dispose(pqueue_t **pptr) {
  if (*pptr == nullptr) {
    return;
  }
  cell_t *cell = (*pptr)->head;
  while (cell != nullptr) {
    cell_t *next = cell->next;
    free(cell);
    cell = next;
  }
  free(*pptr);
  *pptr = nullptr;
}

void *pqueue_enqueue(pqueue_t *p, const void *ref) {
  if (ref == nullptr) {
    return nullptr;
  }
  cell_t *cell = malloc(sizeof *cell);
  if (cell == nullptr) {
    return nullptr;
  }
  cell->ref = ref;
  cell_t **pptr = &p->head;
  while (*pptr != nullptr && p->compar(ref, (*pptr)->ref) <= 0) {
    pptr = &(*pptr)->next;
  }
  cell->next = *pptr;
  *pptr = cell;
  ++p->size;
  return (void *) ref;
}

void *pqueue_dequeue(pqueue_t *p) {
  if (p->head == nullptr) {
    return nullptr;
  }
  cell_t *cell = p->head;
  p->head = cell->next;
  const void *ref = cell->ref;
  free(cell);
  --p->size;
  return (void *) ref;
}

size_t pqueue_size(const pqueue_t *p) {
  return p->size;
}

void pqueue_filter_cntxt(pqueue_t *p,
    bool (*filter)(const void *ref, void *cntxt), void *cntxt) {
  cell_t **pptr = &p->head;
  while (*pptr != nullptr) {
    if (!filter((*pptr)->ref, cntxt)) {
      cell_t *cell = *pptr;
      *pptr = cell->next;
      free(cell);
      --p->size;
    } else {
      pptr = &(*pptr)->next;
    }
  }
}

void *pqueue_peak(pqueue_t *p) {
  if (p->head == nullptr) {
    return nullptr;
  }
  return (void *) p->head->ref;
}
