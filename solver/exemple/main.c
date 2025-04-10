#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

#include "quarto.h"
#include "solver.h"

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
uint8_t mask_usable(const quarto *q) {
  uint8_t mask = 0b0000'0000;
  uint64_t board = quarto_board(q);
  uint16_t summary = quarto_summary(q);
  for (size_t i = 0; i < 16; ++i) {
  }
  // ---
  int acc = 0b1111;
  int pred = (q->board >> (60 - j * 16)) & 0b1111;
  for (int i = 1; i < 4; ++i) {
    if ((q->summary >> (31 - j * 4 - i) & 1) == 0) {
      acc = 0;
      break;
    }
    int v = (q->board >> (60 - j * 16 - i * 4)) & 0b1111;
    if ((acc = ~(pred ^ v) & acc) == 0) {
      break;
    }
    pred = v;
  }
  if (acc != 0) {
    return true;
  }
  // ---
}

// remaining_usable : Retourne le nombre de pièces qui peuvent encore être jouée
//   sur le plateau de jeu associé à q. Tel que cette pièce ne permet pas à
//   l'adversaire de gagner au tour suivant.
size_t remaining_usable(const quarto_t *q) {
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
  size_t count = 0;
  uint8_t mask = mask_usable(q);
  uint16_t r = quarto_remaining_pieces(q);
  for (size_t i = 0; i < sizeof pieces / sizeof *pieces; ++i) {
    if (((r >> (15 - i)) & 1) == 1) {
      int c = 0;
      while (c < 8) {
        if (((mask >> c) & 1) == 1
            && (int) ((pieces[i] >> (c / 2)) & 1) != c % 2) {
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

int main(void) {
  quarto_t *q = quarto_init(C1_SMALL_PLAIN_SQUARE);
  if (q == NULL) {
    fprintf(stderr, "Failed to initialize quarto\n");
    return EXIT_FAILURE;
  }
  // quarto_play(q, C1_SMALL_PLAIN_ROUND, P0);
  // printf("%016b\n", quarto_remaining_pieces(q));
  printf("%d\n", remaining_usable(q));
  // move_t move;
  // if (!min_max(quarto, heuristic, 1, true, &move)) {
  //   fprintf(stderr, "Failed to calculate move\n");
  //   quarto_free(q);
  //   return EXIT_FAILURE;
  // }
  // printf("Best move: piece %d, position %d\n", move.piece, move.pos);
  quarto_dispose(&q);
  return EXIT_SUCCESS;
}
