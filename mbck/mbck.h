#ifndef MBCK_H
#define MBCK_H

#include <stdlib.h>
#include <raylib.h>

typedef struct mbck_t mbck_t;

extern mbck_t *init_mbck(const char *b, int w, int h);

extern void dispose_mbck(mbck_t **m);

extern void update_mbck_scale(mbck_t *m, int width, int height);

extern void mbck_physics_process(mbck_t *m, float delta);

#endif
