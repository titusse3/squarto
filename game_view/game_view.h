#ifndef GAME_VIEW
#define GAME_VIEW

#include <stdlib.h>
#include <raylib.h>

#include "menu.h"

extern void draw_game(state_t *st, game_info_t *game, menu_content_t *info,
    piece_t *pieces, position_t *positions, game_state_t *gs);

#endif
