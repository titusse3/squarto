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
//    profondeur depth et comment noeud racine Max ou Min selon is_max.
// Retourne true si le meilleur coup à bien pu être calculé, false dans le cas
//    contraire et *move renvoie une valeur indéterminé.
extern bool min_max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, move_t *move);

// nega_max : Met dans *move le meilleur coup à jouer selon l'algorihtme
//    Méga-Max à l'aide de l'heuristique heur. Qui sera appliquer au maximum à
//    la profondeur depth.
// Retourne true si le meilleur coup à bien pu être calculé, false dans le cas
//    contraire et *move renvoie une valeur indéterminé.
extern bool nega_max(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, move_t *move);

// alpha_beta : Met dans *move le meilleur coup à jouer selon l'algorihtme
//    alpha beta à l'aide de l'heuristique heur. Qui sera appliquer au maximum à
//    la profondeur depth et comment noeud racine Max ou Min selon is_max
// Retourne true si le meilleur coup à bien pu être calculé, false dans le cas
//    contraire et *move renvoie une valeur indéterminé.
extern bool alpha_beta(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, int alpha, int beta, move_t *move);

// negalpha_beta : Met dans *move le meilleur coup à jouer selon l'algorihtme
//    Negaplha beta à l'aide de l'heuristique heur. Qui sera appliquer au
//    maximum à la profondeur depth.
// Retourne true si le meilleur coup à bien pu être calculé, false dans le cas
//    contraire et *move renvoie une valeur indéterminé.
extern bool negalpha_beta(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, int alpha, int beta, move_t *move);

// sss_star : Met dans *move le meilleur coup à jouer selon l'algorihtme
//    SSS* à l'aide de l'heuristique heur. Qui sera appliquer au maximum à
//    la profondeur depth et comment noeud racine Max ou Min selon is_max
// Retourne true si le meilleur coup à bien pu être calculé, false dans le cas
//    contraire et *move renvoie une valeur indéterminé.
extern bool sss_star(const quarto_t *quarto, int (*heur)(const quarto_t *),
    unsigned int depth, bool is_max, move_t *move);

#endif
