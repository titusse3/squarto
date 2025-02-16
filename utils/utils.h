#ifndef UTILS_H
#define UTILS_H

#include <raylib.h>

#define NB_PIECES 4

#define UNDEF_COORD -1.0f

typedef struct {
  Shader shader;
  Model pieces[NB_PIECES];
  float c_select[2];
  bool mk_screen;
  RenderTexture *screens;
} state_t;

#endif
