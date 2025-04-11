#include "heuristic.h"

#include <limits.h>
#include <stdint.h>
#include <stdbit.h>

// mask_usable_line : Même spécification que mask_usable en ne regardant que les
//   lignes du plateau.
static uint8_t mask_usable_line(const quarto_t *q) {
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
static uint8_t mask_usable_column(const quarto_t *q) {
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
static uint8_t mask_usable_diagonal(const quarto_t *q) {
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

// mask_usable_small_square : Même spécification que mask_usable en ne regardant
//   que les petits carré du plateau.
static uint8_t mask_usable_small_square(const quarto_t *q) {
  uint8_t mask = 0b0000'0000;
  uint64_t board = quarto_board(q);
  uint16_t summary = quarto_summary(q);
  for (int i = 0; i < 3; ++i) { // col
    for (int j = 0; j < 3; ++j) { // line
      int acc = 0b1111;
      bool taken = false;
      int pred;
      int nb_error = 1;
      for (int b1 = 0; b1 < 2; ++b1) {
        for (int b2 = 0; b2 < 2; ++b2) {
          if ((summary >> (31 - (i + b1) - (j + b2) * 4) & 1) == 0) {
            if (nb_error == 0) {
              nb_error = 1;
              acc = 0;
              goto end_square;
            }
            --nb_error;
            continue;
          }
          int v = (board >> (60 - (i + b1) * 4 - (j + b2) * 16)) & 0b1111;
          if (taken && (acc = (~(pred ^ v)) & acc) == 0) {
            goto end_square;
          }
          pred = v;
          taken = true;
        }
      }
end_square:
      for (int i = 0; i < 4; ++i) {
        if ((acc >> i & 1) == 1) {
          mask |= 1 << (7 - (i * 2 + (pred >> i & 1)));
        }
      }
    }
  }
  return mask;
}

// mask_usable_huge_square : Même spécification que mask_usable en ne regardant
//   que les grands carré du plateau.
static uint8_t mask_usable_huge_square(const quarto_t *q) {
  uint8_t mask = 0b0000'0000;
  uint64_t board = quarto_board(q);
  uint16_t summary = quarto_summary(q);
  for (int i = 0; i < 2; ++i) { // col
    for (int j = 0; j < 2; ++j) { // line
      int acc = 0b1111;
      bool taken = false;
      int pred;
      int nb_error = 1;
      for (int b1 = 0; b1 < 3; b1 += 2) {
        for (int b2 = 0; b2 < 3; b2 += 2) {
          if ((summary >> (31 - (i + b1) - (j + b2) * 4) & 1) == 0) {
            if (nb_error == 0) {
              nb_error = 1;
              acc = 0;
              goto end_square;
            }
            --nb_error;
            continue;
          }
          int v = (board >> (60 - (i + b1) * 4 - (j + b2) * 16)) & 0b1111;
          if (taken && (acc = (~(pred ^ v)) & acc) == 0) {
            goto end_square;
          }
          pred = v;
          taken = true;
        }
      }
end_square:
      for (int i = 0; i < 4; ++i) {
        if ((acc >> i & 1) == 1) {
          mask |= 1 << (7 - (i * 2 + (pred >> i & 1)));
        }
      }
    }
  }
  return mask;
}

// mask_usable_rot_square : Même spécification que mask_usable en ne regardant
//   que les carré rotationné du plateau.
static uint8_t mask_usable_rot_square(const quarto_t *q) {
  int dx[] = {
    -1, 0, 1, 0
  };
  int dy[] = {
    0, 1, 0, -1
  };
  uint8_t mask = 0b0000'0000;
  uint64_t board = quarto_board(q);
  uint16_t summary = quarto_summary(q);
  for (int i = 1; i < 3; ++i) { // col
    for (int j = 1; j < 3; ++j) { // line
      int acc = 0b1111;
      bool taken = false;
      int pred;
      int nb_error = 1;
      for (int k = 0; k < 4; ++k) {
        if ((summary >> (31 - (i + dx[k]) - (j + dy[k]) * 4) & 1) == 0) {
          if (nb_error == 0) {
            nb_error = 1;
            acc = 0;
            break;
          }
          --nb_error;
          continue;
        }
        int v = (board >> (60 - (i + dx[k]) * 4 - (j + dy[k]) * 16))
            & 0b1111;
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
  }
  dx[0] = 0;
  dx[1] = 2;
  dx[2] = 1;
  dx[3] = -1;
  0[dy] = -1;
  1[dy] = 0;
  2[dy] = 2;
  3[dy] = 1;
  int acc = 0b1111;
  bool taken = false;
  int pred;
  int nb_error = 1;
  for (int k = 0; k < 4; ++k) {
    if ((summary >> (31 - (1 + dx[k]) - (1 + dy[k]) * 4) & 1) == 0) {
      if (nb_error == 0) {
        nb_error = 1;
        acc = 0;
        break;
      }
      --nb_error;
      continue;
    }
    int v = (board >> (60 - (1 + dx[k]) * 4 - (1 + dy[k]) * 16))
        & 0b1111;
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
  dx[0] = 0;
  dx[1] = 1;
  dx[2] = -1;
  dx[3] = -2;
  0[dy] = -2;
  1[dy] = 0;
  2[dy] = 1;
  3[dy] = -1;
  acc = 0b1111;
  taken = false;
  nb_error = 1;
  for (int k = 0; k < 4; ++k) {
    if ((summary >> (31 - (2 + dx[k]) - (2 + dy[k]) * 4) & 1) == 0) {
      if (nb_error == 0) {
        nb_error = 1;
        acc = 0;
        break;
      }
      --nb_error;
      continue;
    }
    int v = (board >> (60 - (2 + dx[k]) * 4 - (2 + dy[k]) * 16))
        & 0b1111;
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
static uint8_t mask_usable(const quarto_t *q) {
  uint8_t mask;
  switch (quarto_difficulty(q)) {
    case D4:
      mask |= mask_usable_rot_square(q);
    case D3:
      mask |= mask_usable_huge_square(q);
    case D2:
      mask |= mask_usable_small_square(q);
    case D1:
      mask |= mask_usable_line(q) | mask_usable_column(q)
          | mask_usable_diagonal(q);
  }
  return mask;
}

// remaining_usable : Retourne le nombre de pièces qui peuvent encore être jouée
//   sur le plateau de jeu associé à q. Tel que cette pièce ne permet pas à
//   l'adversaire de gagner au tour suivant.
static int remaining_usable(const quarto_t *q) {
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
        return -INT_MAX;
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
