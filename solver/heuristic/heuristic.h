#ifndef HEUR__H
#define HEUR__H

#include <stdlib.h>

#include "quarto.h"
#include "solver.h"

// heuristic : Retourne l'heuristique du plateau de jeu associé à q avec une
//    valeur Negamax.
extern int heuristic(const quarto_t *q);

// min_heuristic : Retourne l'heuristique du plateau de jeu associé à q avec une
//    valeur Minimax avec une racine Max.
extern int max_heuristic(const quarto_t *q);

// min_heuristic : Retourne l'heuristique du plateau de jeu associé à q avec une
//    valeur Minimax avec une racine Min.
extern int min_heuristic(const quarto_t *q);

#endif
