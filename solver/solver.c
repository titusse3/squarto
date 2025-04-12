#include "solver.h"

#include <stdint.h>
#include <limits.h>
#include <stdio.h>

#include "ktree.h"
#include "pqueue.h"
#include "holdall.h"

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
        if (is_max ? val2 >= *val : val2 <= *val) {
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
        if (-val2 >= *val) {
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
          if (val2 >= alpha) {
            alpha = val2;
            (*move).piece = pieces[i];
            (*move).pos = positions[j];
          }
        } else {
          if (val2 <= beta) {
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
        if (-val2 >= v) {
          v = -val2;
          (*move).piece = pieces[i];
          (*move).pos = positions[j];
        }
        if (v >= alpha) {
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
  quarto_t *quarto;
  move_t move;
} node_t;

typedef struct {
  int val;
  size_t node;
  bool resolve;
  bool is_max;
} sss_t;

// make_tree: Construit l'arbre de jeu associé à la racine root de l'arbre k. Si
//    root n'est pas un numéro valide de l'arbre k alors le résultat est
//    indeterminé.
//  Renvoie false en cas d'erreur d'allocation, sinon renvois true.
static bool make_tree(unsigned int depth, ktree_t *k, size_t root,
    holdall *hn) {
  node_t *s = ktree_get_ref_by_num(k, root);
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
  };
  position_t positions[] = {
    P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
  };
  for (size_t i = 0; i < sizeof pieces / sizeof *pieces; ++i) {
    for (size_t j = 0; j < sizeof positions / sizeof *positions; ++j) {
      quarto_t *copy = quarto_copy(s->quarto);
      if (copy == nullptr) {
        return false;
      }
      if (quarto_play(copy, pieces[i], positions[j]) == NO_ERROR) {
        node_t *n = malloc(sizeof *n);
        if (n == nullptr || holdall_put(hn, n) != 0) {
          quarto_dispose(&copy);
          return false;
        }
        n->quarto = copy;
        n->move.pos = positions[j];
        n->move.piece = pieces[i];
        size_t child;
        if ((child = ktree_insert(k, root, n)) == SIZE_MAX
            || !make_tree(depth - 1, k, child, hn)) {
          return false;
        }
      } else {
        quarto_dispose(&copy);
      }
    }
  }
  return true;
}

static int sss_compar(const sss_t *s1, const sss_t *s2) {
  return (s1->val > s2->val) - (s1->val < s2->val);
}

static int node_dispose(node_t *n) {
  quarto_dispose(&(n->quarto));
  free(n);
  return 0;
}

static int fake_free(void *p) {
  free(p);
  return 0;
}

typedef struct {
  ktree_t *k;
  size_t root;
} cntxt_t;

bool is_succ(const sss_t *s, cntxt_t *c) {
  return ktree_is_descendant(c->k, c->root, s->node);
}

bool sss_star(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, move_t *move) {
  if (depth == 0 && !quarto_is_game_over(quarto)) {
    return false;
  }
  holdall *hf = nullptr;
  holdall *hn = nullptr;
  ktree_t *k = nullptr;
  pqueue_t *p = nullptr;
  node_t *n = nullptr;
  sss_t *s = nullptr;
  if ((hf = holdall_empty()) == nullptr
      || (hn = holdall_empty()) == nullptr
      || (k = ktree_empty()) == nullptr
      || (p = pqueue_empty(
          (int (*)(const void *, const void *)) sss_compar)) == nullptr
      || (n = malloc(sizeof *n)) == nullptr
      || (s = malloc(sizeof *s)) == nullptr) {
    holdall_dispose(&hn);
    holdall_dispose(&hf);
    ktree_dispose(&k);
    pqueue_dispose(&p);
    free(n);
    return false;
  }
  n->quarto = (quarto_t *) quarto;
  size_t root;
  if ((root = ktree_insert(k, SIZE_MAX, n)) == SIZE_MAX) {
    holdall_dispose(&hn);
    holdall_dispose(&hf);
    ktree_dispose(&k);
    pqueue_dispose(&p);
    free(n);
    free(s);
    return false;
  }
  s->node = root;
  s->resolve = false;
  s->val = INT_MAX;
  s->is_max = is_max;
  if (pqueue_enqueue(p, s) == nullptr) {
    holdall_dispose(&hn);
    holdall_dispose(&hf);
    ktree_dispose(&k);
    pqueue_dispose(&p);
    free(n);
    free(s);
    return false;
  }
  if (holdall_put(hn, n) != 0 || holdall_put(hf, s) != 0) {
    holdall_dispose(&hn);
    holdall_dispose(&hf);
    ktree_dispose(&k);
    pqueue_dispose(&p);
    free(n);
    free(s);
    return false;
  }
  if (!make_tree(depth, k, s->node, hn)) {
    n->quarto = nullptr;
    holdall_apply(hn, (int (*)(void *)) node_dispose);
    holdall_apply(hf, (int (*)(void *)) fake_free);
    holdall_dispose(&hn);
    holdall_dispose(&hf);
    ktree_dispose(&k);
    pqueue_dispose(&p);
    return false;
  }
  bool r = true;
  sss_t *cur = pqueue_dequeue(p);
  while (!(cur->node == root && cur->resolve)) {
    if (!cur->resolve) {
      size_t child = ktree_get_first_child(k, cur->node);
      if (child == SIZE_MAX) {
        node_t *ncur = ktree_get_ref_by_num(k, cur->node);
        int h = heur(ncur->quarto);
        cur->val = h < cur->val ? h : cur->val;
        cur->resolve = true;
        if (pqueue_enqueue(p, cur) == nullptr) {
          goto error;
        }
      } else {
        cur->node = child;
        cur->is_max = true;
        if (pqueue_enqueue(p, cur) == nullptr) {
          goto error;
        }
        if (cur->is_max) {
          while ((child = ktree_get_neighbor(k, child)) != SIZE_MAX) {
            sss_t *ss = malloc(sizeof *ss);
            if (ss == nullptr) {
              goto error;
            }
            ss->node = child;
            ss->val = cur->val;
            ss->resolve = false;
            ss->is_max = false;
            if (pqueue_enqueue(p, ss) == nullptr || holdall_put(hf, ss) != 0) {
              free(ss);
              goto error;
            }
          }
        }
      }
    } else {
      if (cur->is_max) {
        size_t neighbor = ktree_get_neighbor(k, cur->node);
        if (neighbor != SIZE_MAX) {
          cur->node = neighbor;
          cur->resolve = false;
        } else {
          size_t parent = ktree_get_parent_by_num(k, cur->node);
          if (parent == root) {
            node_t *nroot = ktree_get_ref_by_num(k, root);
            node_t *ncur = ktree_get_ref_by_num(k, cur->node);
            nroot->move.pos = ncur->move.pos;
            nroot->move.piece = ncur->move.piece;
          }
          cur->node = parent;
          cur->is_max = false;
        }
      } else {
        size_t parent = ktree_get_parent_by_num(k, cur->node);
        if (parent == root) {
          node_t *nroot = ktree_get_ref_by_num(k, root);
          node_t *ncur = ktree_get_ref_by_num(k, cur->node);
          nroot->move.pos = ncur->move.pos;
          nroot->move.piece = ncur->move.piece;
        }
        cur->node = parent;
        cur->is_max = true;
        cntxt_t *c = malloc(sizeof *c);
        if (c == nullptr) {
          goto error;
        }
        c->k = k;
        c->root = parent;
        pqueue_filter_cntxt(p, (bool (*)(const void *, void *)) is_succ, c);
      }
      if (pqueue_enqueue(p, cur) == nullptr) {
        goto error;
      }
    }
    cur = pqueue_dequeue(p);
  }
  node_t *nroot = ktree_get_ref_by_num(k, root);
  move->pos = nroot->move.pos;
  move->piece = nroot->move.piece;
  goto dispose;
error:
  r = false;
dispose:
  n->quarto = nullptr;
  holdall_apply(hn, (int (*)(void *)) node_dispose);
  holdall_apply(hf, (int (*)(void *)) fake_free);
  holdall_dispose(&hn);
  holdall_dispose(&hf);
  ktree_dispose(&k);
  pqueue_dispose(&p);
  return r;
}
