#ifndef MENU_H
#define MENU_H

#include <raylib.h>
#include <stdlib.h>

#include "utils.h"

#define SPACE_BETWEEN_BUTTONS 11

#define NB_PIECES 4

#define MENU_PADDING 10

#define TITLE_DIVIDER 8
#define TITLE_POS_DIV 6

#define TEXT_PADDING_MENU 20

#define UNDEF_COORD -1.0f

typedef struct {
  int screen_w;
  int screen_h;
  const char *game_name;
  bool exit_wind;
  bool play_music;
  bool play_sound;
} game_info_t;

typedef enum {
  RULES,
  HISTORY,
  CHOOSE_DIFFICULTY,
  PLAY,
  QUIT,
  NONE
} btn_t;

typedef struct {
  bool has_start;
  // bool has_end;
  Texture2D img;
  Rectangle frameRec;
  int framesCounter;
  int currentFrame;
  int currentLine;
} animation_t;

typedef struct {
  enum {
    MENU,
    GAME
  } currentScreen;
  btn_t menuType;
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
    struct {
      difficulty_t difficulty;
      bool has_end;
    } game_values;
  } content;
  btn_t sound_play;
  Sound sound;
  Font win_animation_font;
  animation_t win_animation;
  animation_t idle_animation;
} menu_content_t;

extern void display_menu(game_info_t *game, menu_content_t *menu, state_t *st);

extern bool display_exit_menu(game_info_t *game_info, int fontSize,
    const char *msg, int msg_size);

extern void display_animation(game_info_t *game_info, animation_t *info);

extern void display_end_animation(game_info_t *game_info, animation_t *info,
    Font f, const char *text, bool win);

#endif
