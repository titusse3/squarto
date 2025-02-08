#ifndef MENU_H
#define MENU__H

#include "raylib.h"
#include <stdlib.h>

typedef struct game_info_t {
  int screen_w;
  int screen_h;
  const char *game_name;
  bool exit_wind;
} game_info_t;

typedef struct {
  enum {
    MENU,
    GAME
  } currentScreen;
  enum {
    RULES,
    HISTORY,
    NONE
  } menuType;
  union {
    struct {
      int rules_num;
      int rules_frames;
      Texture2D *rules_textures;
    } rules_values;
    struct {
      int history_frames;
      Texture2D history_texture;
    } history_values;
  } content;
} menu_content_t;

extern void display_menu(game_info_t *game, menu_content_t *menu, Camera3D
    *camera, Shader *shader);

bool display_exit_menu(game_info_t *game_info);

#endif
