#ifndef QUARTO__H
#define QUARTO__H

#include <stdlib.h>

// quarto_t : Structure et type de structure représentant une partie de Quarto.
typedef struct quarto_t quarto_t;

typedef enum {
  C1_SMALL_PLAIN_SQUARE
    =
      0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,
  C1_SMALL_PLAIN_ROUND
    =
      0b0001'0001'0001'0001'0001'0001'0001'0001'0001'0001'0001'0001'0001'0001'0001'0001,
  C1_SMALL_HOLE_SQUARE
    =
      0b0010'0010'0010'0010'0010'0010'0010'0010'0010'0010'0010'0010'0010'0010'0010'0010,
  C1_SMALL_HOLE_ROUND
    =
      0b0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011'0011,
  C1_HUGE_PLAIN_SQUARE
    =
      0b0100'0100'0100'0100'0100'0100'0100'0100'0100'0100'0100'0100'0100'0100'0100'0100,
  C1_HUGE_PLAIN_ROUND
    =
      0b0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101'0101,
  C1_HUGE_HOLE_SQUARE
    =
      0b0110'0110'0110'0110'0110'0110'0110'0110'0110'0110'0110'0110'0110'0110'0110'0110,
  C1_HUGE_HOLE_ROUND
    =
      0b0111'0111'0111'0111'0111'0111'0111'0111'0111'0111'0111'0111'0111'0111'0111'0111,
  //
  C2_SMALL_PLAIN_SQUARE
    =
      0b1000'1000'1000'1000'1000'1000'1000'1000'1000'1000'1000'1000'1000'1000'1000'1000,
  C2_SMALL_PLAIN_ROUND
    =
      0b1001'1001'1001'1001'1001'1001'1001'1001'1001'1001'1001'1001'1001'1001'1001'1001,
  C2_SMALL_HOLE_SQUARE
    =
      0b1010'1010'1010'1010'1010'1010'1010'1010'1010'1010'1010'1010'1010'1010'1010'1010,
  C2_SMALL_HOLE_ROUND
    =
      0b1011'1011'1011'1011'1011'1011'1011'1011'1011'1011'1011'1011'1011'1011'1011'1011,
  C2_HUGE_PLAIN_SQUARE
    =
      0b1100'1100'1100'1100'1100'1100'1100'1100'1100'1100'1100'1100'1100'1100'1100'1100,
  C2_HUGE_PLAIN_ROUND
    =
      0b1101'1101'1101'1101'1101'1101'1101'1101'1101'1101'1101'1101'1101'1101'1101'1101,
  C2_HUGE_HOLE_SQUARE
    =
      0b1110'1110'1110'1110'1110'1110'1110'1110'1110'1110'1110'1110'1110'1110'1110'1110,
  C2_HUGE_HOLE_ROUND
    =
      0b1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'1111'
} piece_t;

typedef enum {
  P0
    =
      0b1111'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,
  P1
    =
      0b0000'1111'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,
  P2
    =
      0b0000'0000'1111'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,
  P3
    =
      0b0000'0000'0000'1111'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,
  P4
    =
      0b0000'0000'0000'0000'1111'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,
  P5
    =
      0b0000'0000'0000'0000'0000'1111'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000,
  P6
    =
      0b0000'0000'0000'0000'0000'0000'1111'0000'0000'0000'0000'0000'0000'0000'0000'0000,
  P7
    =
      0b0000'0000'0000'0000'0000'0000'0000'1111'0000'0000'0000'0000'0000'0000'0000'0000,
  P8
    =
      0b0000'0000'0000'0000'0000'0000'0000'0000'1111'0000'0000'0000'0000'0000'0000'0000,
  P9
    =
      0b0000'0000'0000'0000'0000'0000'0000'0000'0000'1111'0000'0000'0000'0000'0000'0000,
  P10
    =
      0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1111'0000'0000'0000'0000'0000,
  P11
    =
      0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1111'0000'0000'0000'0000,
  P12
    =
      0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1111'0000'0000'0000,
  P13
    =
      0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1111'0000'0000,
  P14
    =
      0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1111'0000,
  P15
    =
      0b0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'0000'1111,
} position_t;

typedef enum {
  PLAYER1,
  PLAYER2,
  NEITHER
} player_t;

typdef enum {
  NO_ERROR,
  POSITION_OCCUPIED,
  PIECE_ALREADY_PLAYED,
  GAME_ALREADY_OVER,
} quarto_return_t;

// quarto_init : Retourne la structure associée à un jeu de Quarto.
//  Retourne un pointeur vers la structure associée à un jeu de Quarto en cas de
//    succés, nulptr dans le cas contraire.
extern quarto_t *quarto_init(void);

//  quarto_dispose : Libère la mémoire associée au jeu de Quarto associé à
//    *qptr et met *qptr à la valeur nulptr.
extern void quarto_dispose(quarto_t **qptr);

//  quarto_whos_turn : Retourne le joueur dont c'est le tour si la partie n'est
//    pas fini. Si elle l'est, retourne NEITHER.
extern player_t quarto_whos_turn(quarto_t *q);

//  quarto_play : Joue un coup dans la partie de Quarto associée à q. Ce coup
//    consiste à placer la pièce p à la position pos.
//    Retourne le code associé a ce mouvement.
extern quarto_return_t quarto_play(quarto_t *q, piece_t p, position_t pos);

//  quarto_game_turn : Retourne le tour de jeu associé à q.
extern int quarto_game_turn(quarto_t *q);

//  quarto_is_game_over : Retourne si la partie est finie.
extern bool quarto_is_game_over(quarto_t *q);

//  quarto_winner : Retourne le joueur gagnant si la partie est finie.
extern player_t quarto_winner(quarto_t *q);

#endif
