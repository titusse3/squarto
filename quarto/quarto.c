#include "quarto.h"
#include <stdint.h>

#include <stdio.h>
#include <stdbit.h>

#define M_OVER_BIT   ((uint32_t) 0b0000'0000'0000'0000'1000'0000'0000'0000)
#define M_TURN_BIT   ((uint32_t) 0b0000'0000'0000'0000'0100'0000'0000'0000)
#define M_PIECE_MASK ((uint32_t) 0b0000'0000'0000'0000'0011'1100'0000'0000)
#define M_TURN_MASK  ((uint32_t) 0b0000'0000'0000'0000'0000'0011'1110'0000)

#define MAX_TURN 15

#define SELECT_PIECE(sum, piece) {                                             \
          (sum) &= ~M_PIECE_MASK;                                              \
          (sum) |= ((piece) << 10);                                            \
}                                                                              \

struct quarto_t {
  // 16 bits de résumé
  // 1 bits pour parti finie
  // 1 bits pour le tour
  // 4 bits pour la pièce selectionnée
  // 5 bits pour le tour au équelle le jeu est
  uint32_t summary;
  uint64_t board;
};

quarto_t *quarto_init(piece_t start) {
  quarto_t *q = malloc(sizeof *q);
  if (q == nullptr) {
    return nullptr;
  }
  q->summary = 0;
  SELECT_PIECE(q->summary, (uint32_t) (start & P15));
  q->board = 0;
  return q;
}

void quarto_dispose(quarto_t **qptr) {
  if (*qptr == nullptr) {
    return;
  }
  free(*qptr);
  *qptr = nullptr;
}

player_t quarto_whos_turn(quarto_t *q) {
  return (q->summary & M_TURN_BIT) == 0 ? PLAYER1 : PLAYER2;
}

bool quarto_is_game_over(quarto_t *q) {
  return (q->summary & M_OVER_BIT) != 0;
}

size_t quarto_game_turn(quarto_t *q) {
  return (q->summary & M_TURN_MASK) >> 5;
}

player_t quarto_winner(quarto_t *q) {
  return quarto_is_game_over(q) ? (quarto_whos_turn(q)
    == PLAYER1 ? PLAYER2 : PLAYER1) : NEITHER;
}

piece_t quarto_current_piece(quarto_t *q) {
  int v = (q->summary & M_PIECE_MASK) >> 10;
  switch (v) {
    case 0:
      return C1_SMALL_PLAIN_SQUARE;
    case 1:
      return C1_SMALL_PLAIN_ROUND;
    case 2:
      return C1_SMALL_HOLE_SQUARE;
    case 3:
      return C1_SMALL_HOLE_ROUND;
    case 4:
      return C1_HUGE_PLAIN_SQUARE;
    case 5:
      return C1_HUGE_PLAIN_ROUND;
    case 6:
      return C1_HUGE_HOLE_SQUARE;
    case 7:
      return C1_HUGE_HOLE_ROUND;
    case 8:
      return C2_SMALL_PLAIN_SQUARE;
    case 9:
      return C2_SMALL_PLAIN_ROUND;
    case 10:
      return C2_SMALL_HOLE_SQUARE;
    case 11:
      return C2_SMALL_HOLE_ROUND;
    case 12:
      return C2_HUGE_PLAIN_SQUARE;
    case 13:
      return C2_HUGE_PLAIN_ROUND;
    case 14:
      return C2_HUGE_HOLE_SQUARE;
    case 15:
      return C2_HUGE_HOLE_ROUND;
  }
  return 0;
}

static bool piece__already_played(quarto_t *q, piece_t p) {
  for (int i = 0; i < 16; i++) {
    int v = (q->board >> (60 - i * 4)) & 0b1111;
    if (v == (int) (p & P15)) {
      return true;
    }
  }
  return false;
}

static bool check__line(quarto_t *q) {
  for (int j = 0; j < 4; ++j) {
    if ((q->summary >> (31 - j * 4) & 1) == 0) {
      continue;
    }
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
  }
  return false;
}

static bool check__column(quarto_t *q) {
  for (int col = 0; col < 4; ++col) {
    if (((q->summary >> (31 - col)) & 1) == 0) {
      continue;
    }
    int acc = 0b1111;
    int pos = col;
    int pred = (q->board >> (60 - pos * 4)) & 0b1111;
    for (int row = 1; row < 4; ++row) {
      pos = row * 4 + col;
      if (((q->summary >> (31 - pos)) & 1) == 0) {
        acc = 0;
        break;
      }
      int v = (q->board >> (60 - pos * 4)) & 0b1111;
      if ((acc = (~(pred ^ v)) & acc) == 0) {
        break;
      }
      pred = v;
    }
    if (acc != 0) {
      return true;
    }
  }
  return false;
}

static bool check__diagonal(quarto_t *q) {
  int acc = 0b1111;
  int pred;
  if ((q->summary >> 31 & 1) != 0) {
    pred = (int) (q->board >> 60) & 0b1111;
    for (int i = 1; i < 4; ++i) {
      if ((q->summary >> (31 - i * 5) & 1) == 0) {
        acc = 0;
        break;
      }
      int v = (int) (q->board >> (60 - i * 20)) & 0b1111;
      if ((acc = (~(pred ^ v)) & acc) == 0) {
        break;
      }
      pred = v;
    }
    if (acc != 0) {
      return true;
    }
  }
  if ((q->summary >> 28 & 1) == 0) {
    return false;
  }
  acc = 0b1111;
  pred = (int) (q->board >> 48) & 0b1111;
  for (int i = 1; i < 4; ++i) {
    if ((q->summary >> (28 - i * 3) & 1) == 0) {
      acc = 0;
      break;
    }
    int v = (int) (q->board >> (48 - i * 12)) & 0b1111;
    if ((acc = (~(pred ^ v)) & acc) == 0) {
      break;
    }
    pred = v;
  }
  if (acc != 0) {
    return true;
  }
  return acc != 0;
}

quarto_return_t quarto_play(quarto_t *q, piece_t p, position_t pos) {
  if (quarto_is_game_over(q)) {
    return GAME_ALREADY_OVER;
  }
  unsigned int ps = (stdc_first_leading_one_ull(pos) - 1) / 4;
  if ((q->summary >> (31 - ps) & 1) != 0) {
    return POSITION_OCCUPIED;
  }
  piece_t p2 = quarto_current_piece(q);
  if (quarto_game_turn(q) != MAX_TURN
      && (p == p2 || piece__already_played(q, p))) {
    return PIECE_ALREADY_PLAYED;
  }
  // flip le bit du tour
  q->summary ^= M_TURN_BIT;
  // augmente le compteur du tour
  q->summary += 0b0000'0000'0000'0000'0000'0000'0010'0000;
  q->summary |= 1 << (31 - ps);
  // on met la pièce sur le plateau
  q->board |= p2 & pos;
  SELECT_PIECE(q->summary, (uint32_t) (p & P15));
  if (check__line(q) || check__column(q) || check__diagonal(q)) {
    q->summary |= M_OVER_BIT;
  }
  return NO_ERROR;
}

void quarto_print_board(quarto_t *q) {
  printf("%032b\n", q->summary);
  for (int row = 0; row < 4; ++row) {
    for (int col = 0; col < 4; ++col) {
      int pos = row * 4 + col;
      int piece = (q->board >> (60 - pos * 4)) & 0b1111;
      // Check if the position is on one of the diagonals.
      int is_diagonal = (row == col) || (row + col == 3);
      if ((q->summary & (1 << (31 - pos))) != 0) {
        if (is_diagonal) {
          printf("\033[31m%04b\033[0m ", piece);
        } else {
          printf("%04b ", piece);
        }
      } else {
        printf("   X ");
      }
    }
    printf("\n");
  }
}
