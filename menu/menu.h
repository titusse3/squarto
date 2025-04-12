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

// DIFFICULTY NAMES
#define S_EASY "Mise en Orbite"
#define S_MEDIUM "Orbites en Folie"
#define S_HARD "Tempête d'Astéroïdes"
#define S_VERY_HARD "Supernova Ultime"

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
  Texture2D img;
  Rectangle frameRec;
  int framesCounter;
  int currentFrame;
  int currentLine;
  int frameDivisor;
} animation_t;

typedef struct {
  enum {
    MENU,
    GAME
  } currentScreen;
  btn_t menuType;
  union {
    struct {
      char rules_num;
      int rules_frames;
      Texture2D *rules_textures;
    } rules_values;
    struct {
      int history_frames;
      Texture2D history_texture;
    } history_values;
    struct {
      difficulty_t difficulty;
      enum {
        MINIMAX = 0,
        NEGAMAX = 1,
        ALPHABETA = 2,
        NEGALPHABETA = 3,
        SSS_S = 4
      } solver;
      bool dropwdonw_open;
      int frames;
      int stage;
    } game_values;
  } content;
  btn_t sound_play;
  Sound sound;
  Font anim_font;
  animation_t anims[2];
} menu_content_t;

// display_menu: Fonction permettant d'afficher le menu principal du jeu.
extern void display_menu(game_info_t *game, menu_content_t *menu, state_t *st);

// display_exit_menu: Fonction permettant d'afficher le menu de sortie du jeu.
extern bool display_exit_menu(game_info_t *game_info, int fontSize,
    const char *msg, int msg_size);

// display_animation: Fonction permettant d'afficher l'animation pointé par
//    info.
extern void display_animation(game_info_t *game_info, animation_t *info,
    Rectangle destRec);

// display_end_animation: Fonction permettant d'afficher l'animation de fin de
//    partie.
extern void display_end_animation(game_info_t *game_info, animation_t *info,
    Font f, const char *text, bool win);

#endif
