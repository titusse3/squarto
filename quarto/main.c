#include <stdlib.h>
#include "quarto.h"

int main(void) {
  piece_t piece[] = {
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
    C1_SMALL_PLAIN_SQUARE
  };
  position_t position[] = {
    P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
  };
  srand(25);
  size_t n_piece = sizeof(piece) / sizeof(piece[0]) - 1;
  for (size_t i = n_piece - 1; i > 0; i--) {
    size_t j = (size_t) rand() % (i + 1);
    piece_t tmp = piece[i];
    piece[i] = piece[j];
    piece[j] = tmp;
  }
  size_t n_position = sizeof(position) / sizeof(position[0]);
  for (size_t i = n_position - 1; i > 0; i--) {
    size_t j = (size_t) rand() % (i + 1);
    position_t tmp = position[i];
    position[i] = position[j];
    position[j] = tmp;
  }
  quarto_t *q = quarto_init(C1_SMALL_PLAIN_SQUARE);
  if (q == nullptr) {
    return EXIT_FAILURE;
  }
  for (size_t i = 0; i < sizeof piece / sizeof *piece; i++) {
    switch (quarto_play(q, piece[i], position[i])) {
      case NO_ERROR:
        break;
      case POSITION_OCCUPIED:
        break;
      case PIECE_ALREADY_PLAYED:
        break;
      case GAME_ALREADY_OVER:
        break;
    }
  }
  quarto_dispose(&q);
  return EXIT_SUCCESS;
}
