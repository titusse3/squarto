#include "mbck.h"

struct mbck_t {
  Texture2D b;
  float scroll_b;
  float scale_b;
};

mbck_t *init_mbck(const char *b, int width, int height) {
  mbck_t *m = malloc(sizeof *m);
  if (m == nullptr) {
    return nullptr;
  }
  m->b = LoadTexture(b);
  m->scroll_b = 0;
  m->scale_b = (float) width / m->b.width;
  return m;
}

void dispose_mbck(mbck_t **m) {
  if (*m == nullptr) {
    return;
  }
  UnloadTexture((*m)->b);
  free(*m);
  *m = nullptr;
}

void update_mbck_scale(mbck_t *m, int width, int height) {
  m->scale_b = (float) width / m->b.width;
}

void mbck_physics_process(mbck_t *m, float delta) {
  m->scroll_b -= delta;
  if (m->scroll_b <= -m->b.width * m->scale_b) {
    m->scroll_b = 0;
  }
  DrawTextureEx(m->b, (Vector2){ m->scroll_b, 0 }, 0.0f,
      m->scale_b, WHITE);
  DrawTextureEx(m->b,
      (Vector2){ m->b.width *(m->scale_b) + m->scroll_b, 0 }, 0.0f,
      m->scale_b, WHITE);
  DrawTextureEx(m->b,
      (Vector2){ m->scroll_b, m->b.height * m->scale_b }, 0.0f,
      m->scale_b, WHITE);
  DrawTextureEx(m->b,
      (Vector2){ m->b.width *m->scale_b + m->scroll_b,
                 m->b.height *m->scale_b }, 0.0f,
      m->scale_b, WHITE);
}
