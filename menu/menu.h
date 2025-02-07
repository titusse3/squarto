#ifndef MENU_H
#define MENU__H

#include <stdlib.h>

typedef enum {
  MENU,
  GAME
} GameScreen;

extern void display_menu(int screen_w, int screen_h, const char *game_title,
    GameScreen *currentScreen);

#endif
