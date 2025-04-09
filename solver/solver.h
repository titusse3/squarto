#ifndef SOLV__H
#define SOLV__H

#include <stdlib.h>

#include "quarto.h"

typedef struct {
  position_t pos;
  piece_t piece;
} move_t;

// min_max : Met dans *move le meilleur coup à jouer selon l'algorihtme Min-Max
//    à l'aide de l'heuristique heur. Qui sera appliquer au maximum à la
//    profondeur depth et comment noeud racine Max ou Min selon is_max
// Retourne true si le meilleur coup à bien pu être calculé, false dans le cas
//    contraire et *move renvoie une valeur indéterminé.
extern bool min_max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, move_t *move);

// nega_max : Renvoie le meilleur coup à jouer selon l'algorihtme Nega-Max à
//    l'aide de l'heuristique heur. Qui sera appliquer au maximum à la
//    profondeur depth.
extern move_t nega_max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth);

// alpha_beta : Renvoie le meilleur coup à jouer selon l'algorihtme Alpha-Beta
//    à l'aide de l'heuristique heur. Qui sera appliquer au maximum à la
//    profondeur depth.
extern move_t alpha_beta(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth);

// neg_alpha_beta : Renvoie le meilleur coup à jouer selon l'algorihtme
//    Nega-Alpha-Beta à l'aide de l'heuristique heur. Qui sera appliquer au
//    maximum à la profondeur depth.
extern move_t neg_alpha_beta(const quarto_t *quarto,
    int (*heur)(const quarto_t *), unsigned int depth);

// sss_star : Renvoie le meilleur coup à jouer selon l'algorihtme SSS* à
//    l'aide de l'heuristique heur. Qui sera appliquer au maximum à la
//    profondeur depth.
extern move_t sss_star(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth);

// magick : Renvoie le meilleur coup à jouer selon un algorithme secret. Qui
//    sera appliquer au maximum à la profondeur depth.
extern move_t magick(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth);

#endif
