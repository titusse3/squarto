#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>
#include <raymath.h>

#include "rlights.h"
#include "quarto.h"

#define NB_PIECES 4

#define UNDEF_COORD -1.0f

typedef struct {
  Shader shader;
  Model pieces[NB_PIECES];
  float c_select[2];
  Light lights[2];
  bool mk_screen;
  RenderTexture *screens;
} state_t;

extern state_t init_state(const char *ss[2], const char *mds[NB_PIECES]);

typedef struct {
  quarto_t *q;
  uint16_t used;
  float p_select[2];
} game_state_t;

#endif
