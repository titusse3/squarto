#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdbit.h>

#include "quarto.h"
#include "solver.h"

// mask_usable_line : Même spécification que mask_usable en ne regardant que les
//   lignes du plateau.
uint8_t mask_usable_line(const quarto_t *q) {
  uint8_t mask = 0b0000'0000;
  uint64_t board = quarto_board(q);
  uint16_t summary = quarto_summary(q);
  for (int j = 0; j < 4; ++j) {
    int acc = 0b1111;
    int nb_error = 1;
    bool taken = false;
    int pred;
    for (int i = 0; i < 4; ++i) {
      if ((summary >> (15 - j * 4 - i) & 1) == 0) {
        if (nb_error == 0) {
          nb_error = 1;
          acc = 0;
          break;
        }
        --nb_error;
        continue;
      }
      int v = (board >> (60 - j * 16 - i * 4)) & 0b1111;
      if (taken && (acc = ~(pred ^ v) & acc) == 0) {
        break;
      }
      pred = v;
      taken = true;
    }
    for (int i = 0; i < 4; ++i) {
      if ((acc >> i & 1) == 1) {
        mask |= 1 << (7 - (i * 2 + (pred >> i & 1)));
      }
    }
  }
  return mask;
}

// mask_usable_column : Même spécification que mask_usable en ne regardant que
// les colones du plateau.
uint8_t mask_usable_column(const quarto_t *q) {
  uint8_t mask = 0b0000'0000;
  uint64_t board = quarto_board(q);
  uint16_t summary = quarto_summary(q);
  for (int col = 0; col < 4; ++col) {
    int acc = 0b1111;
    int nb_error = 1;
    int pos = col;
    bool taken = false;
    int pred;
    for (int row = 0; row < 4; ++row) {
      pos = row * 4 + col;
      if (((summary >> (15 - pos)) & 1) == 0) {
        if (nb_error == 0) {
          nb_error = 1;
          acc = 0;
          break;
        }
        --nb_error;
        continue;
      }
      int v = (board >> (60 - pos * 4)) & 0b1111;
      if (taken && (acc = (~(pred ^ v)) & acc) == 0) {
        break;
      }
      pred = v;
      taken = true;
    }
    for (int i = 0; i < 4; ++i) {
      if ((acc >> i & 1) == 1) {
        mask |= 1 << (7 - (i * 2 + (pred >> i & 1)));
      }
    }
  }
  return mask;
}

// mask_usable_diagonal : Même spécification que mask_usable en ne regardant que
// les diagonales du plateau.
uint8_t mask_usable_diagonal(const quarto_t *q) {
  uint8_t mask = 0b0000'0000;
  uint64_t board = quarto_board(q);
  uint16_t summary = quarto_summary(q);
  int acc = 0b1111;
  int nb_error = 1;
  bool taken = false;
  int pred = 0; // initialisation pour faire plaisir à gcc
  for (int i = 0; i < 4; ++i) {
    if ((summary >> (15 - i * 5) & 1) == 0) {
      if (nb_error == 0) {
        nb_error = 1;
        acc = 0;
        break;
      }
      --nb_error;
      continue;
    }
    int v = (board >> (60 - i * 20)) & 0b1111;
    if (taken && (acc = (~(pred ^ v)) & acc) == 0) {
      break;
    }
    pred = v;
    taken = true;
  }
  for (int i = 0; i < 4; ++i) {
    if ((acc >> i & 1) == 1) {
      mask |= 1 << (7 - (i * 2 + (pred >> i & 1)));
    }
  }
  acc = 0b1111;
  taken = false;
  nb_error = 1;
  for (int i = 0; i < 4; ++i) {
    if ((summary >> (12 - i * 3) & 1) == 0) {
      if (nb_error == 0) {
        nb_error = 1;
        acc = 0;
        break;
      }
      --nb_error;
      continue;
    }
    int v = (board >> (48 - i * 12)) & 0b1111;
    if (taken && (acc = (~(pred ^ v)) & acc) == 0) {
      break;
    }
    pred = v;
    taken = true;
  }
  for (int i = 0; i < 4; ++i) {
    if ((acc >> i & 1) == 1) {
      mask |= 1 << (7 - (i * 2 + (pred >> i & 1)));
    }
  }
  return mask;
}

// mask_usable : Retourne le masque des pièces jouable sur le plateau de jeu
//   associé à q. Tel que cette pièce ne permet pas à l'adversaire de gagner au
//   tour suivant.
//   Le masque est de la forme 0b0000'0000.
//   Le bit d'indice :
//    - 0 : 1 si les carrer sont pas autorisé
//    - 1 : 1 si les ronds sont pas autorisé
//    - 2 : 1 si les pièces pleines sont pas autorisé
//    - 3 : 1 si les pièces avec un trou sont pas autorisé
//    - 4 : 1 si les petites pièces sont pas autorisé
//    - 5 : 1 si les grandes pièces sont pas autorisé
//    - 6 : 1 si les pièces de couleur C1 sont pas autorisé
//    - 7 : 1 si les pièces de couleur C2 sont pas autorisé
uint8_t mask_usable(const quarto_t *q) {
  return mask_usable_line(q) | mask_usable_column(q) | mask_usable_diagonal(q);
}

// remaining_usable : Retourne le nombre de pièces qui peuvent encore être jouée
//   sur le plateau de jeu associé à q. Tel que cette pièce ne permet pas à
//   l'adversaire de gagner au tour suivant.
int remaining_usable(const quarto_t *q) {
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
  int count = 0;
  uint8_t mask = mask_usable(q);
  uint16_t r = quarto_remaining_pieces(q);
  for (size_t i = 0; i < sizeof pieces / sizeof *pieces; ++i) {
    if (((r >> (15 - i)) & 1) == 1) {
      int c = 0;
      while (c < 8) {
        if (((mask >> c) & 1) == 1
            // ((pieces[i] >> (3 - c / 2)) & 1)
            //   permet de récuppéré le bit correspondant à la propriété qu'on
            //   veut tester dans pieces[i]
            // Ce bit doit avoir la bonne valeur selon la parité de c, car si
            // c est impair alors on test la propriété inverse du bit prédicat
            // dans piece[i].
            && (int) ((pieces[i] >> (3 - c / 2)) & 1) != c % 2) {
          break;
        }
        ++c;
      }
      if (c == 8) {
        ++count;
      }
    }
  }
  return count;
}

int heuristic(const quarto_t *q) {
  if (quarto_is_game_over(q)) {
    switch (quarto_winner(q)) {
      case PLAYER1:
        return INT_MAX;
      case PLAYER2:
        return INT_MIN;
      default:
        return 0;
    }
  }
  return remaining_usable(q);
}

#define NEGA2NORMAL(name, func, m1, m2)                                        \
        int name ## _heuristic(const quarto_t * q) {                           \
          switch (quarto_whos_turn(q)) {                                       \
            case PLAYER1:                                                      \
              return (m1) * func(q);                                           \
            case PLAYER2:                                                      \
              return (m2) * func(q);                                           \
            default:                                                           \
              return (quarto_winner(q)                                         \
                == PLAYER1 ? (m1) : (m2)) * (func) (q);                        \
          }                                                                    \
        }                                                                      \

NEGA2NORMAL(max, heuristic, 1, -1)
NEGA2NORMAL(min, heuristic, -1, 1)

int main(void) {
  quarto_t *q = quarto_init(C1_HUGE_PLAIN_ROUND);
  if (q == NULL) {
    fprintf(stderr, "Failed to initialize quarto\n");
    return EXIT_FAILURE;
  }
#if true
  move_t move;
  if (!min_max(q, max_heuristic, 3, true, &move)) {
    fprintf(stderr, "Failed to calculate move\n");
    quarto_dispose(&q);
    return EXIT_FAILURE;
  }
  unsigned int ps = (stdc_first_leading_one_ull(move.pos) - 1) / 4;
  printf("Best move: piece %04b, position %u\n", (int) (move.piece & 0b1111),
      ps);
#elif false
  quarto_play(q, C1_HUGE_HOLE_ROUND, P0);
  quarto_play(q, C1_HUGE_HOLE_SQUARE, P4);
  quarto_play(q, C2_SMALL_PLAIN_ROUND, P8);
  quarto_play(q, C2_SMALL_HOLE_ROUND, P5);
  quarto_play(q, C2_SMALL_PLAIN_SQUARE, P6);
  printf("mask:\t%08b\n", mask_usable(q));
  printf("usable:\t%d\n", remaining_usable(q));
#elif false
  quarto_play(q, C1_HUGE_HOLE_ROUND, P0);
  quarto_play(q, C1_HUGE_HOLE_SQUARE, P5);
  quarto_play(q, C2_SMALL_PLAIN_ROUND, P10);
  printf("mask:\t%08b\n", mask_usable(q));
  printf("usable:\t%d\n", remaining_usable(q));
#elif false
  quarto_play(q, C1_HUGE_HOLE_ROUND, P3);
  quarto_play(q, C1_HUGE_HOLE_SQUARE, P6);
  quarto_play(q, C2_SMALL_PLAIN_ROUND, P9);
  printf("mask:\t%08b\n", mask_usable(q));
  printf("usable:\t%d\n", remaining_usable(q));
#else
  printf("test\n");
  quarto_play(q, C1_HUGE_HOLE_ROUND, P3);
  quarto_play(q, C1_HUGE_HOLE_SQUARE, P6);
  quarto_play(q, C2_SMALL_PLAIN_ROUND, P9);
  quarto_play(q, C2_SMALL_PLAIN_SQUARE, P1);
  quarto_play(q, C2_SMALL_HOLE_SQUARE, P2);
  printf("mask:\t%08b\n", mask_usable(q));
  printf("usable:\t%d\n", remaining_usable(q));
  printf("heuristic:\t%d\n", heuristic(q));
#endif
  quarto_dispose(&q);
  return EXIT_SUCCESS;
}
