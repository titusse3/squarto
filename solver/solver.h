#ifndef SOLV__H
#define SOLV__H

#include <stdlib.h>

#include "quarto.h"

typedef struct {
  position_t pos;
  piece_t piece;
} move_t;

#define MAX_DEPTH 4

// min_max : Renvoie le meilleur coup à jouer selon l'algorihtme Min-Max à
//    l'aide de l'heuristique heur.
extern move_t min_max(const quarto_t *quarto, int (*heur)(quarto_t *));

// nega_max : Renvoie le meilleur coup à jouer selon l'algorihtme Nega-Max à
//    l'aide de l'heuristique heur.
extern move_t nega_max(const quarto_t *quarto, int (*heur)(quarto_t *));

// alpha_beta : Renvoie le meilleur coup à jouer selon l'algorihtme Alpha-Beta
//    à l'aide de l'heuristique heur.
extern move_t alpha_beta(const quarto_t *quarto, int (*heur)(quarto_t *));

// neg_alpha_beta : Renvoie le meilleur coup à jouer selon l'algorihtme
//    Nega-Alpha-Beta à l'aide de l'heuristique heur.
extern move_t neg_alpha_beta(const quarto_t *quarto, int (*heur)(quarto_t *));

// sss_star : Renvoie le meilleur coup à jouer selon l'algorihtme SSS* à
//    l'aide de l'heuristique heur.
extern move_t sss_star(const quarto_t *quarto, int (*heur)(quarto_t *));

// magick : Renvoie le meilleur coup à jouer selon un algorithme secret.
extern move_t magick(const quarto_t *quarto, int (*heur)(quarto_t *));

#endif
