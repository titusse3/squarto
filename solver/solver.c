#include "solver.h"

#include <stdint.h>

// min__max : Renvoie la valeur min-max associée au plateau quarto selon
//    l'heuristique heur. Qui sera appliquer au maximum à la profondeur depth.
static int min__max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max) {
  if (depth == 0) {
    return heur(quarto);
  }
  if (is_max) {
    int val = INT_MIN;
  }
}

move_t min_max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth) {
}

// move_t nega_max(const quarto_t *quarto, int (*heur)(quarto_t *));
// move_t alpha_beta(const quarto_t *quarto, int (*heur)(quarto_t *));
// move_t neg_alpha_beta(const quarto_t *quarto, int (*heur)(quarto_t *));
// move_t sss_star(const quarto_t *quarto, int (*heur)(quarto_t *));
// move_t magick(const quarto_t *quarto, int (*heur)(quarto_t *));
