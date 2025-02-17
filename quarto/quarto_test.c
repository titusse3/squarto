#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "quarto.h"

#define GW(v, n) (*((unsigned int *) &(v) + (n)))

#define TEST { \
          printf("-------------------------------------------------------\n"); \
          printf("a qui le tour : %s\n", \
    (quarto_whos_turn(q) == PLAYER1 ? "PLAYER1" : "PLAYER2"));  \
          printf("isEnd : %s\n", \
    (quarto_is_game_over(q) ? "true" : "false")); \
          printf("le gagnant : %s\n", \
    (quarto_winner(q) == PLAYER1 ? "PLAYER1" : quarto_winner(q) \
    == PLAYER2 ? "PLAYER2" : "NEITHER")); \
          printf("Turn : %zu\n", quarto_game_turn(q)); \
          uint64_t a = quarto_current_piece(q); \
          printf("Piece : %032b%032b\n", GW(a, 1), GW(a, 0)); \
          printf("-------------------------------------------------------\n"); \
}

int main(void) {
  // Define the piece and position arrays (no longer constant to allow
  // shuffling)
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
  // Shuffle the pieces array using Fisher-Yates algorithm
  size_t n_piece = sizeof(piece) / sizeof(piece[0]) - 1;
  for (size_t i = n_piece - 1; i > 0; i--) {
    size_t j = rand() % (i + 1);
    piece_t tmp = piece[i];
    piece[i] = piece[j];
    piece[j] = tmp;
  }
  // Shuffle the positions array using Fisher-Yates algorithm
  size_t n_position = sizeof(position) / sizeof(position[0]);
  for (size_t i = n_position - 1; i > 0; i--) {
    size_t j = rand() % (i + 1);
    position_t tmp = position[i];
    position[i] = position[j];
    position[j] = tmp;
  }
  quarto_t *q = quarto_init(C1_SMALL_PLAIN_SQUARE);
  if (q == nullptr) {
    return EXIT_FAILURE;
  }
  TEST;
  for (size_t i = 0; i < sizeof piece / sizeof *piece; i++) {
    switch (quarto_play(q, piece[i], position[i])) {
      case NO_ERROR:
        break;
      case POSITION_OCCUPIED:
        printf("POSITION_OCCUPIED\n");
        break;
      case PIECE_ALREADY_PLAYED:
        printf("PIECE_ALREADY_PLAYED\n");
        break;
      case GAME_ALREADY_OVER:
        printf("GAME_ALREADY_OVER\n");
        break;
    }
  }
  TEST;
  printf("Summary: %016b\n", quarto_summary(q));
  uint64_t b = quarto_board(q);
  printf("Board : %032b%032b\n", GW(b, 1), GW(b, 0));
  // quarto_print_board(q);
  quarto_dispose(&q);
  return EXIT_SUCCESS;
}
