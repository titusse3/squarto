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

bool test_h(char **pp) {
  bool b[4] = {
    true, true, true, true
  };
  for (size_t i = 1; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      b[j] = (pp[i - 1][j] == pp[i][j]) && b[j];
    }
  }
  return b[0] || b[1] || b[2] || b[3];
}

int main(void) {
  // Define the piece and position arrays (no longer constant to allow
  // shuffling)
#if true
  for (unsigned int k = 0; k < 100'000'00; k++) {
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
    srand(k);
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
    char **pp;
    for (size_t i = 0; i < sizeof piece / sizeof *piece; i++) {
      switch (quarto_play(q, piece[i], position[i], &pp)) {
        case WIN_ROW:
          if (!test_h(pp)) {
            printf("ERROR seed %u\n", k);
          }
          break;
        case WIN_COL:
          if (!test_h(pp)) {
            printf("col error seed %u\n", k);
          }
          break;
        case DIAG:
          if (!test_h(pp)) {
            printf("diag error seed %u\n", k);
          }
          break;
        case POSITION_OCCUPIED:
          printf("POSITION_OCCUPIED\n");
          break;
        case PIECE_ALREADY_PLAYED:
          printf("PIECE_ALREADY_PLAYED\n");
          break;
        case GAME_ALREADY_OVER:
          break;
        case NO_WIN:
          break;
      }
    }
    quarto_dispose(&q);
  }
#else
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
  srand(99984);
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
  char **pp;
  for (size_t i = 0; i < sizeof piece / sizeof *piece; i++) {
    switch (quarto_play(q, piece[i], position[i], &pp)) {
      case WIN_ROW:
        if (!test_h(pp)) {
          printf("ERROR seed\n");
        }
        break;
      case WIN_COL:
        if (!test_h(pp)) {
          printf("col error seed\n");
        }
        break;
      case DIAG:
        printf("win ???\n");
        if (!test_h(pp)) {
          printf("diag error seed\n");
        }
        break;
      case POSITION_OCCUPIED:
        printf("POSITION_OCCUPIED\n");
        break;
      case PIECE_ALREADY_PLAYED:
        printf("PIECE_ALREADY_PLAYED\n");
        break;
      case GAME_ALREADY_OVER:
        break;
      case NO_WIN:
        break;
    }
  }
  quarto_print_board(q);
  quarto_dispose(&q);
#endif
  return EXIT_SUCCESS;
}
