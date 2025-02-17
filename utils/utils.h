#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>

#include "rlights.h"

#define NB_PIECES 4

#define UNDEF_COORD -1.0f

typedef struct {
  Shader shader;
  Model pieces[NB_PIECES];
  float c_select[2];
  Light lights[2];
  size_t round;
  bool mk_screen;
  RenderTexture *screens;
} state_t;

#endif
