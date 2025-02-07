#include "quarto.h"

struct quarto_t {
  // 16 bit de summary
  // 1 bit pour parti finie
  // 1 bit pour le tour
  // 4 bit pour la pièce selectionner
  // 5 bit pour le tour auquelle le jeu est
  int summary;
  int64_t board;
};

quarto_t *quarto_init(void) {
  quarto_t *g = malloc(sizeof(*g));
  if (g == NULL) {
    return NULL;
  }
  g->summary = 0;
  g->board = 0;
  return game;
}

void quarto_dispose(quarto_t **qptr) {
  if (*qptr == nulptr) {
    return;
  }
  free(*qptr);
  *qptr = nulptr;
}
