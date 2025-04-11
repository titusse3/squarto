#include <stdlib.h>
#include <stdio.h>
#include <stdbit.h>
#include <limits.h>

#include "quarto.h"
#include "solver.h"
#include "heuristic.h"

int main(void) {
  quarto_t *q = quarto_init(C1_HUGE_PLAIN_ROUND, D1);
  if (q == NULL) {
    fprintf(stderr, "Failed to initialize quarto\n");
    return EXIT_FAILURE;
  }
#if false
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
  move_t move;
  if (!nega_max(q, heuristic, 3, &move)) {
    fprintf(stderr, "Failed to calculate move\n");
    quarto_dispose(&q);
    return EXIT_FAILURE;
  }
  unsigned int ps = (stdc_first_leading_one_ull(move.pos) - 1) / 4;
  printf("Best move: piece %04b, position %u\n", (int) (move.piece & 0b1111),
      ps);
#elif false
  move_t move;
  if (!alpha_beta(q, max_heuristic, 5, true, -INT_MAX, INT_MAX, &move)) {
    fprintf(stderr, "Failed to calculate move\n");
    quarto_dispose(&q);
    return EXIT_FAILURE;
  }
  unsigned int ps = (stdc_first_leading_one_ull(move.pos) - 1) / 4;
  printf("Best move: piece %04b, position %u\n", (int) (move.piece & 0b1111),
      ps);
#elif false
  move_t move;
  if (!negalpha_beta(q, heuristic, 5, -INT_MAX, INT_MAX, &move)) {
    fprintf(stderr, "Failed to calculate move\n");
    quarto_dispose(&q);
    return EXIT_FAILURE;
  }
  unsigned int ps = (stdc_first_leading_one_ull(move.pos) - 1) / 4;
  printf("Best move: piece %04b, position %u\n", (int) (move.piece & 0b1111),
      ps);
#elif true
  move_t move;
  if (!sss_star(q, max_heuristic, 2, true, &move)) {
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
