#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "quarto.h"
#include "solver.h"

int heuristic(const quarto_t *q) {
  if (quarto_is_game_over(q)) {
    switch quarto_winner(q) {
      case PLAYER_1:
        return MAX_INT;
      case PLAYER_2:
        return MIN_INT;
      default:
        return 0;
    }
  }
  int count = 0;

  return count;
}

int main(void) {
  quarto_t *q = quarto_init(C1_SMALL_PLAIN_SQUARE);
  if (q == NULL) {
    fprintf(stderr, "Failed to initialize quarto\n");
    return EXIT_FAILURE;
  }
  move_t move;
  if (!min_max(quarto, heuristic, 1, true, &move)) {
    fprintf(stderr, "Failed to calculate move\n");
    quarto_free(q);
    return EXIT_FAILURE;
  }
  printf("Best move: piece %d, position %d\n", move.piece, move.pos);
  quarto_free(q);
  return EXIT_SUCCESS;
}
