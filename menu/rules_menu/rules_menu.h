#ifndef RULES_F_H
#define RULES_F_H

#include <raylib.h>
#include <stdlib.h>

#include "menu.h"
#include "utils_menu.h"

// MAX_RULES: Nombre maximum de règles à afficher.
#define MAX_RULES 6

// display_rules: Fonction prenant en charge la logique permettant d'afficher
//    le menu de rêgle du jeu.
extern void display_rules(game_info_t *game, int left_padding, int offset,
    int font_size, menu_content_t *menu, state_t *st);

#endif
