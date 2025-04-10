#include "solver.h"

#include <stdint.h>
#include <limits.h>
#include <stdio.h>

#include "ktree.h"
#include "pqueue.h"

// min__max : Met dans *val la valeur min-max associée au plateau quarto selon
//    l'heuristique heur et dans *move le coup qui à permis d'avoir cette
//    valeur. Qui sera appliquer au maximum à la profondeur depth.
// Retourne true si les valeur à bien pu être calculée, false dans le cas
//    contraire et les valeurs de *val et *move sont indéterminé.
static bool min__max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, int *val, move_t *move) {
  if (depth == 0 || quarto_is_game_over(quarto)) {
    *val = heur(quarto);
    return true;
  }
  piece_t pieces[] = {
    C1_SMALL_PLAIN_SQUARE,
    C1_SMALL_PLAIN_ROUND,
    C1_SMALL_HOLE_SQUARE,
    C1_SMALL_HOLE_ROUND,
    C1_HUGE_PLAIN_SQUARE,
    C1_HUGE_PLAIN_ROUND,
    C1_HUGE_HOLE_SQUARE,
    C1_HUGE_HOLE_ROUND,
    C2_SMALL_PLAIN_SQUARE,
    C2_SMALL_PLAIN_ROUND,
    C2_SMALL_HOLE_SQUARE,
    C2_SMALL_HOLE_ROUND,
    C2_HUGE_PLAIN_SQUARE,
    C2_HUGE_PLAIN_ROUND,
    C2_HUGE_HOLE_SQUARE,
    C2_HUGE_HOLE_ROUND,
  };
  position_t positions[] = {
    P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
  };
  *val = (is_max ? -INT_MAX : INT_MAX);
  for (size_t i = 0; i < sizeof pieces / sizeof *pieces; ++i) {
    for (size_t j = 0; j < sizeof positions / sizeof *positions; ++j) {
      quarto_t *copy = quarto_copy(quarto);
      if (copy == nullptr) {
        return false;
      }
      if (quarto_play(copy, pieces[i], positions[j]) == NO_ERROR) {
        int val2;
        move_t move2;
        if (!min__max(copy, heur, depth - 1, !is_max, &val2, &move2)) {
          return false;
        }
        if (is_max ? val2 > *val : val2 < *val) {
          *val = val2;
          (*move).piece = pieces[i];
          (*move).pos = positions[j];
        }
      }
      quarto_dispose(&copy);
    }
  }
  return true;
}

bool min_max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, move_t *move) {
  int val;
  return depth != 0 && !quarto_is_game_over(quarto)
    && min__max(quarto, heur, depth, is_max, &val, move);
}

// nega__max : Met dans *val la valeur néga-max associée au plateau quarto selon
//    l'heuristique heur et dans *move le coup qui à permis d'avoir cette
//    valeur. Qui sera appliquer au maximum à la profondeur depth.
// Retourne true si les valeur à bien pu être calculée, false dans le cas
//    contraire et les valeurs de *val et *move sont indéterminé.
static bool nega__max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, int *val, move_t *move) {
  if (depth == 0 || quarto_is_game_over(quarto)) {
    *val = heur(quarto);
    return true;
  }
  piece_t pieces[] = {
    C1_SMALL_PLAIN_SQUARE,
    C1_SMALL_PLAIN_ROUND,
    C1_SMALL_HOLE_SQUARE,
    C1_SMALL_HOLE_ROUND,
    C1_HUGE_PLAIN_SQUARE,
    C1_HUGE_PLAIN_ROUND,
    C1_HUGE_HOLE_SQUARE,
    C1_HUGE_HOLE_ROUND,
    C2_SMALL_PLAIN_SQUARE,
    C2_SMALL_PLAIN_ROUND,
    C2_SMALL_HOLE_SQUARE,
    C2_SMALL_HOLE_ROUND,
    C2_HUGE_PLAIN_SQUARE,
    C2_HUGE_PLAIN_ROUND,
    C2_HUGE_HOLE_SQUARE,
    C2_HUGE_HOLE_ROUND,
  };
  position_t positions[] = {
    P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
  };
  *val = -INT_MAX;
  for (size_t i = 0; i < sizeof pieces / sizeof *pieces; ++i) {
    for (size_t j = 0; j < sizeof positions / sizeof *positions; ++j) {
      quarto_t *copy = quarto_copy(quarto);
      if (copy == nullptr) {
        return false;
      }
      if (quarto_play(copy, pieces[i], positions[j]) == NO_ERROR) {
        int val2;
        move_t move2;
        if (!nega__max(copy, heur, depth - 1, &val2, &move2)) {
          return false;
        }
        if (-val2 > *val) {
          *val = -val2;
          (*move).piece = pieces[i];
          (*move).pos = positions[j];
        }
      }
      quarto_dispose(&copy);
    }
  }
  return true;
}

bool nega_max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, move_t *move) {
  int val;
  return depth != 0 && !quarto_is_game_over(quarto)
    && nega__max(quarto, heur, depth, &val, move);
}

// alpha__beta : Met dans *val la valeur alpha-beta associée au plateau quarto
//    selon l'heuristique heur et dans *move le coup qui à permis d'avoir cette
//    valeur. Qui sera appliquer au maximum à la profondeur depth.
// Retourne true si les valeur à bien pu être calculée, false dans le cas
//    contraire et les valeurs de *val et *move sont indéterminé.
static bool alpha__beta(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, int alpha, int beta, int *val,
    move_t *move) {
  if (depth == 0 || quarto_is_game_over(quarto)) {
    *val = heur(quarto);
    return true;
  }
  piece_t pieces[] = {
    C1_SMALL_PLAIN_SQUARE,
    C1_SMALL_PLAIN_ROUND,
    C1_SMALL_HOLE_SQUARE,
    C1_SMALL_HOLE_ROUND,
    C1_HUGE_PLAIN_SQUARE,
    C1_HUGE_PLAIN_ROUND,
    C1_HUGE_HOLE_SQUARE,
    C1_HUGE_HOLE_ROUND,
    C2_SMALL_PLAIN_SQUARE,
    C2_SMALL_PLAIN_ROUND,
    C2_SMALL_HOLE_SQUARE,
    C2_SMALL_HOLE_ROUND,
    C2_HUGE_PLAIN_SQUARE,
    C2_HUGE_PLAIN_ROUND,
    C2_HUGE_HOLE_SQUARE,
    C2_HUGE_HOLE_ROUND,
  };
  position_t positions[] = {
    P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
  };
  for (size_t i = 0; i < sizeof pieces / sizeof *pieces; ++i) {
    for (size_t j = 0; j < sizeof positions / sizeof *positions; ++j) {
      quarto_t *copy = quarto_copy(quarto);
      if (copy == nullptr) {
        return false;
      }
      if (alpha >= beta) {
        quarto_dispose(&copy);
        *val = is_max ? alpha : beta;
        return true;
      }
      if (quarto_play(copy, pieces[i], positions[j]) == NO_ERROR) {
        int val2;
        move_t move2;
        if (!alpha__beta(copy, heur, depth - 1, !is_max, alpha, beta, &val2,
            &move2)) {
          return false;
        }
        if (is_max) {
          if (val2 > alpha) {
            alpha = val2;
            (*move).piece = pieces[i];
            (*move).pos = positions[j];
          }
        } else {
          if (val2 < beta) {
            beta = val2;
            (*move).piece = pieces[i];
            (*move).pos = positions[j];
          }
        }
      }
      quarto_dispose(&copy);
    }
  }
  *val = is_max ? alpha : beta;
  return true;
}

bool alpha_beta(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, int alpha, int beta, move_t *move) {
  int val;
  return depth != 0 && !quarto_is_game_over(quarto)
    && alpha__beta(quarto, heur, depth, is_max, alpha, beta, &val, move);
}

// negalpha__beta : Met dans *val la valeur negalpha-beta associée au plateau
//    quarto selon l'heuristique heur et dans *move le coup qui à permis d'avoir
//    cette valeur. Qui sera appliquer au maximum à la profondeur depth.
// Retourne true si les valeur à bien pu être calculée, false dans le cas
//    contraire et les valeurs de *val et *move sont indéterminé.
static bool negalpha__beta(const quarto_t *quarto,
    int (*heur)(const quarto_t *), unsigned int depth, int alpha, int beta,
    int *val, move_t *move) {
  if (depth == 0 || quarto_is_game_over(quarto)) {
    *val = heur(quarto);
    return true;
  }
  piece_t pieces[] = {
    C1_SMALL_PLAIN_SQUARE,
    C1_SMALL_PLAIN_ROUND,
    C1_SMALL_HOLE_SQUARE,
    C1_SMALL_HOLE_ROUND,
    C1_HUGE_PLAIN_SQUARE,
    C1_HUGE_PLAIN_ROUND,
    C1_HUGE_HOLE_SQUARE,
    C1_HUGE_HOLE_ROUND,
    C2_SMALL_PLAIN_SQUARE,
    C2_SMALL_PLAIN_ROUND,
    C2_SMALL_HOLE_SQUARE,
    C2_SMALL_HOLE_ROUND,
    C2_HUGE_PLAIN_SQUARE,
    C2_HUGE_PLAIN_ROUND,
    C2_HUGE_HOLE_SQUARE,
    C2_HUGE_HOLE_ROUND,
  };
  position_t positions[] = {
    P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
  };
  int v = -INT_MAX;
  for (size_t i = 0; i < sizeof pieces / sizeof *pieces; ++i) {
    for (size_t j = 0; j < sizeof positions / sizeof *positions; ++j) {
      quarto_t *copy = quarto_copy(quarto);
      if (copy == nullptr) {
        return false;
      }
      if (alpha >= beta) {
        quarto_dispose(&copy);
        *val = v;
        return true;
      }
      if (quarto_play(copy, pieces[i], positions[j]) == NO_ERROR) {
        int val2;
        move_t move2;
        if (!negalpha__beta(copy, heur, depth - 1, -beta, -alpha, &val2,
            &move2)) {
          return false;
        }
        if (-val2 > v) {
          v = -val2;
          (*move).piece = pieces[i];
          (*move).pos = positions[j];
        }
        if (v > alpha) {
          alpha = v;
        }
      }
      quarto_dispose(&copy);
    }
  }
  *val = v;
  return true;
}

bool negalpha_beta(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, int alpha, int beta, move_t *move) {
  int val;
  return depth != 0 && !quarto_is_game_over(quarto)
    && negalpha__beta(quarto, heur, depth, alpha, beta, &val, move);
}

typedef struct {
  const quarto_t *quarto;
  move_t move;
} sss_t;

// make_tree: Construit l'arbre de jeu associé à la racine root de l'arbre k. Si
//    root n'est pas un numéro valide de l'arbre k alors le résultat est
//    indeterminé.
//  Renvoie false en cas d'erreur d'allocation, sinon renvois true.
static bool make_tree(unsigned int depth, ktree_t *k, size_t root) {
  sss_t *s = ktree_get_ref_by_num(k, root);
  if (depth == 0 || quarto_is_game_over(s->quarto)) {
    return true;
  }
  piece_t pieces[] = {
    C1_SMALL_PLAIN_SQUARE,
    C1_SMALL_PLAIN_ROUND,
    C1_SMALL_HOLE_SQUARE,
    C1_SMALL_HOLE_ROUND,
    C1_HUGE_PLAIN_SQUARE,
    C1_HUGE_PLAIN_ROUND,
    C1_HUGE_HOLE_SQUARE,
    C1_HUGE_HOLE_ROUND,
    C2_SMALL_PLAIN_SQUARE,
    C2_SMALL_PLAIN_ROUND,
    C2_SMALL_HOLE_SQUARE,
    C2_SMALL_HOLE_ROUND,
    C2_HUGE_PLAIN_SQUARE,
    C2_HUGE_PLAIN_ROUND,
    C2_HUGE_HOLE_SQUARE,
    C2_HUGE_HOLE_ROUND,
  }
  position_t positions[] = {
    P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
  };
  for (size_t i = 0; i < sizeof pieces / sizeof *pieces; ++i) {
    for (size_t j = 0; j < sizeof positions / sizeof *positions; ++j) {
      quarto_t *copy = quarto_copy(quarto);
      if (copy == nullptr) {
        return false;
      }
      if (quarto_play(copy, pieces[i], positions[j]) == NO_ERROR) {
        sss_t *s = malloc(sizeof *s);
        if (s == nullptr) {
          quarto_dispose(&copy);
          return false;
        }
        s->quarto = copy;
        s->move.pos = positions[j];
        s->move.piece = pieces[i];
        size_t child;
        if ((child = ktree_insert(k, root, s)) == SIZE_MAX) {
          quarto_dispose(&copy);
          free(s);
          return false;
        }
        if (!make_tree(depth - 1, k, child)) {
          return false;
        }
      }
    }
  }
  return true;
}

bool sss_star(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, move_t *move) {
  if (depth != 0 && !quarto_is_game_over(quarto)) {
    return false;
  }
  ktree_t *k = ktree_empty();
  if (k == nullptr) {
    return false;
  }
  sss_t *s = malloc(sizeof *s);
  if (s == nullptr) {
    ktree_dispose(&k);
    return false;
  }
  s->quarto = quarto;
  size_t root;
  pqueue_t *p = pqueue_empty();
  if (p == nullptr) {
    ktree_dispose(&k);
    free(s);
    return false;
  }
  if ((root = ktree_insert(k, SIZE_MAX, s)) == SIZE_MAX) {
    ktree_dispose(&k);
    return false;
  }
  make_tree(depth, k, root);
}
