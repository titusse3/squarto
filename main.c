#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdbit.h>

#include <raylib.h>
#include <raymath.h>

#include "rlights.h"
#include "menu.h"
#include "mbck.h"
#include "raygui.h"
#include "quarto.h"
#include "solver.h"
#include "heuristic.h"
#include "game_view.h"

#include "utils_menu.h"

#define BASE_SCREEN_WIDTH 1280
#define BASE_SCREEN_HEIGHT 720
#define GAME_NAME "Quarto"
#define QUIT_MSG \
        "Do you really want to quit the game ?\nYou will miss a lot of fun..."

static void init_animation(menu_content_t *game);

static void animation_step(menu_content_t *game);

int main(void) {
  InitWindow(BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, GAME_NAME);
  SetExitKey(0);
  SetTargetFPS(60);
  //
  game_info_t game_info = {
    .screen_w = GetScreenWidth(),
    .screen_h = GetScreenHeight(),
    .game_name = GAME_NAME,
    .exit_wind = false,
    .play_music = true
  };
  //
  menu_content_t game = {
    .currentScreen = MENU,
    .menuType = NONE,
    .sound_play = NONE,
  };
  //
  state_t st = init_state(
      (const char *[2]){ "resources/shaders/glsl330/lighting.vs",
                         "resources/shaders/glsl330/lighting.fs" },
      (const char *[NB_PIECES]){ "resources/model/PLAIN_SQUARE.obj",
                                 "resources/model/PLAIN_ROUND.obj",
                                 "resources/model/HOLE_SQUARE.obj",
                                 "resources/model/HOLE_ROUND.obj" });
  //
  InitAudioDevice();
  Music music = LoadMusicStream("resources/music/italian_hymn.mp3");
  PlayMusicStream(music);
  SetMusicVolume(music, 0.6f);
  game.sound = LoadSound("resources/sound/select.wav");
  SetSoundVolume(game.sound, 0.6f);
  //
  mbck_t *m_bck = init_mbck("resources/image/blue-back.png",
      game_info.screen_w, game_info.screen_h, true);
  mbck_t *m_fg = init_mbck("resources/image/blue-stars.png",
      game_info.screen_w, game_info.screen_h, true);
  //
  uint16_t used = 0;
  piece_t pieces[] = {
    C1_SMALL_PLAIN_SQUARE,
    C1_SMALL_PLAIN_ROUND,
    C1_SMALL_HOLE_SQUARE,
    C1_SMALL_HOLE_ROUND,
    C1_HUGE_PLAIN_SQUARE,
    C1_HUGE_PLAIN_ROUND,
    C1_HUGE_HOLE_SQUARE,
    C1_HUGE_HOLE_ROUND,
    C2_SMALL_PLAIN_SQUARE,
    C2_SMALL_PLAIN_ROUND,
    C2_SMALL_HOLE_SQUARE,
    C2_SMALL_HOLE_ROUND,
    C2_HUGE_PLAIN_SQUARE,
    C2_HUGE_PLAIN_ROUND,
    C2_HUGE_HOLE_SQUARE,
    C2_HUGE_HOLE_ROUND,
  };
  position_t positions[] = {
    P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15
  };
  //
  game_state_t gs = {
    .q = nullptr, .used = 0, .p_select = {
      UNDEF_COORD, UNDEF_COORD
    }
  };
  //
  game.anim_font = LoadFont("resources/fonts/alagard.png");
  //
  init_animation(&game);
  //
  game.anims[1].has_start = true;
  //
  while (true) {
    animation_step(&game);
    //
    game_info.screen_w = GetScreenWidth();
    game_info.screen_h = GetScreenHeight();
    if (WindowShouldClose()) {
      game_info.exit_wind = true;
    }
    if (IsWindowResized()) {
      update_mbck_scale(m_bck, game_info.screen_w, game_info.screen_h);
      if (!st.mk_screen) {
        free(st.screens);
        st.mk_screen = true;
      }
    }
    if (game_info.exit_wind) {
      if (IsKeyPressed(KEY_Y)) {
        break;
      } else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE)) {
        game_info.exit_wind = false;
      }
    }
    if (game_info.play_music) {
      UpdateMusicStream(music);
    }
    if (game.currentScreen == GAME) {
      if (IsKeyPressed(KEY_ESCAPE)) {
        game.currentScreen = MENU;
        if (!st.mk_screen) {
          free(st.screens);
          st.mk_screen = true;
        }
        quarto_dispose(&(gs.q));
        used = 0;
        st.c_select[0] = UNDEF_COORD;
        st.c_select[1] = UNDEF_COORD;
        gs.p_select[0] = UNDEF_COORD;
        gs.p_select[1] = UNDEF_COORD;
      }
      ++game.content.game_values.frames;
    } else if (game.menuType == RULES) {
      ++game.content.rules_values.rules_frames;
    } else if (game.menuType == HISTORY) {
      ++game.content.history_values.history_frames;
    }
    BeginDrawing();
    ClearBackground(GetColor(0x052c46ff));
    mbck_physics_process(m_bck, 0.7f);
    mbck_physics_process(m_fg, 1.0f);
    //
    if (game.currentScreen == GAME) {
      draw_game(&st, &game_info, &game, pieces, positions, &gs);
    } else {
      display_menu(&game_info, &game, &st);
    }
    if (game_info.exit_wind) {
      int font_size = game_info.screen_h / 26;
      int size = MeasureText("Do you really want to quit the game ?",
          font_size);
      if (display_exit_menu(&game_info, font_size, QUIT_MSG, size)) {
        break;
      }
    }
    EndDrawing();
  }
  //
  dispose_mbck(&m_bck);
  dispose_mbck(&m_fg);
  //
  for (size_t i = 0; i < sizeof game.anims / sizeof *game.anims; ++i) {
    UnloadTexture(game.anims[i].img);
  }
  UnloadFont(game.anim_font);
  UnloadSound(game.sound);
  CloseAudioDevice();
  CloseWindow();
  return EXIT_SUCCESS;
}

void init_animation(menu_content_t *game) {
  game->anims[0].img = LoadTexture("resources/image/explosion.png");
  game->anims[0].frameRec = (Rectangle) {
    0, 0, game->anims[0].img.width / 4, game->anims[0].img.height / 4
  };
  game->anims[0].frameDivisor = 1;
  //
  game->anims[1].img = LoadTexture("resources/image/idle.png");
  game->anims[1].frameRec = (Rectangle) {
    0, 0, game->anims[1].img.width / 5, game->anims[1].img.height
  };
  game->anims[1].frameDivisor = 5;
  //
  for (size_t i = 0; i < sizeof game->anims / sizeof *game->anims;
      i++) {
    game->anims[i].has_start = false;
    game->anims[i].currentFrame = 0;
    game->anims[i].currentLine = 0;
    game->anims[i].framesCounter = 0;
  }
}

void animation_step(menu_content_t *game) {
  for (size_t i = 0; i < sizeof game->anims / sizeof *game->anims; ++i) {
    if (game->anims[i].has_start) {
      game->anims[i].framesCounter++;
      if (game->anims[i].framesCounter > game->anims[i].frameDivisor) {
        game->anims[i].currentFrame++;
        if (game->anims[i].currentFrame >= 4) {
          game->anims[i].currentFrame = 0;
          game->anims[i].currentLine++;
          if (game->anims[i].currentLine >= 4) {
            game->anims[i].currentLine = 0;
            game->anims[i].has_start = false;
          }
        }
        game->anims[i].framesCounter = 0;
      }
      game->anims[i].frameRec.x = game->anims[i].frameRec.width
          * game->anims[i].currentFrame;
      game->anims[i].frameRec.y = game->anims[i].frameRec.height
          * game->anims[i].currentLine;
    }
  }
}
