#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <raylib.h>
#include <raymath.h>

#include "rlights.h"
#include "menu.h"
#include "mbck.h"
#include "raygui.h"
#include "quarto.h"

#define BASE_SCREEN_WIDTH 1280
#define BASE_SCREEN_HEIGHT 720
#define GAME_NAME "Quarto"
#define QUIT_MSG \
        "Do you really want to quit the game ?\nYou will miss a lot of fun..."

static void draw_game(state_t *st, game_info_t *game, menu_content_t *info,
    piece_t *pieces, position_t *positions, game_state_t *gs);

static void draw_pieces(state_t *st, game_state_t *gs, Camera3D *camera);
static void draw_board_game(state_t *st, quarto_t *quarto);
static void select_case(state_t *st, Camera3D *camera);

static void display_background(Texture2D background, Texture2D foreground,
    float scrollingBack, float scrollingFore, float scale_bg, float scale_fg);

static void pieces_selectors(state_t *st, game_state_t *gs);

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
  game.win_info.explosion = LoadTexture("resources/image/explosion.png");
  game.win_info.frameRec = (Rectangle) {
    0, 0, game.win_info.explosion.width / 4, game.win_info.explosion.height / 4
  };
  game.win_info.f = LoadFont("resources/fonts/alagard.png");
  game.win_info.has_end = false;
  int currentFrame = 0;
  int currentLine = 0;
  int framesCounter = 0;
  //
  while (true) {
    //
    if (game.win_info.has_end) {
      framesCounter++;
      if (framesCounter > 2) {
        currentFrame++;
        if (currentFrame >= 4) {
          currentFrame = 0;
          currentLine++;
          if (currentLine >= 4) {
            currentLine = 0;
          }
        }
        framesCounter = 0;
      }
      game.win_info.frameRec.x = game.win_info.frameRec.width
          * currentFrame;
      game.win_info.frameRec.y = game.win_info.frameRec.height
          * currentLine;
    }
    //
    game_info.screen_w = GetScreenWidth();
    game_info.screen_h = GetScreenHeight();
    if (WindowShouldClose()) {
      game_info.exit_wind = true;
    }
    if (IsWindowResized()) {
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
  UnloadTexture(game.win_info.explosion);
  UnloadFont(game.win_info.f);
  UnloadSound(game.sound);
  CloseAudioDevice();
  CloseWindow();
  return EXIT_SUCCESS;
}

void draw_game(state_t *st, game_info_t *game, menu_content_t *info,
    piece_t *pieces, position_t *positions, game_state_t *gs) {
  if (st->mk_screen) {
    st->screens = malloc(sizeof *st->screens);
    if (st->screens == nullptr) {
      display_exit_menu(game, game->screen_h / 16, "Error during allocation",
          MeasureText("Error during allocation", game->screen_h / 16));
      return;
    }
    st->screens[0]
      = LoadRenderTexture(game->screen_h / 2, game->screen_h / 1.5);
    st->mk_screen = false;
  }
  if (gs->q == nullptr) {
    srand(time(nullptr));
    size_t p = rand() % NB_PIECES;
    gs->q = quarto_init(pieces[p]);
    if (gs->q == nullptr) {
      display_exit_menu(game, game->screen_h / 16, "Error during allocation",
          MeasureText("Error during allocation", game->screen_h / 16));
      return;
    }
    gs->used = 1 << (uint16_t) p;
  }
  Camera3D camera = {
    .position = (Vector3) {
      7.5f, 20.0f, 7.5f
    },
    .target = (Vector3) {
      0.0f, 0.0f, 0.0f
    },
    .up = (Vector3) {
      0.0f, 1.0f, 0.0f
    },
    .fovy = 30.0f,
    .projection = CAMERA_PERSPECTIVE
  };
  SetShaderValue(
      st->shader,
      st->shader.locs[SHADER_LOC_VECTOR_VIEW],
      &camera.position,
      SHADER_UNIFORM_VEC3
      );
  st->lights[0].position = camera.position;
  UpdateLightValues(st->shader, st->lights[0]);
  if (!quarto_is_game_over(gs->q)) {
    select_case(st, &camera);
    if (st->c_select[0] != UNDEF_COORD && gs->p_select[0] != UNDEF_COORD
        && quarto_play(gs->q,
        pieces[(size_t) (gs->p_select[0] * 4 + gs->p_select[1])],
        positions[15 - (size_t) (st->c_select[0] * 4 + st->c_select[1])])
        == NO_ERROR) {
      gs->used |= 0b1 << (uint16_t) (gs->p_select[0] * 4 + gs->p_select[1]);
      gs->p_select[0] = UNDEF_COORD;
      gs->p_select[1] = UNDEF_COORD;
    }
  }
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  draw_board_game(st, gs->q);
  EndShaderMode();
  EndMode3D();
  draw_pieces(st, gs, &camera);
  DrawTextureRec(
      st->screens->texture,
      (Rectangle) {0.0f,
                   0.0f,
                   (float) st->screens->texture.width,
                   (float) -st->screens->texture.height},
      (Vector2) {0.0f, 0.0f}, WHITE);
  if (quarto_winner(gs->q) == PLAYER1) {
    display_end_animation(game, &info->win_info, "You w in !", true);
  } else if (quarto_winner(gs->q) == PLAYER2) {
    display_end_animation(game, &info->win_info, "You Loose !", false);
  }
}

void select_case(state_t *st, Camera3D *camera) {
  Ray ray = {};
  RayCollision collision = {};
  float x = 0;
  float z = 0;
  while (x < 4.0f && !collision.hit) {
    z = 0;
    while (z < 4.0f && !collision.hit) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ray = GetScreenToWorldRay(GetMousePosition(), *camera);
        collision = GetRayCollisionBox(
            ray,
            (BoundingBox) { (Vector3) { x * 1.5f - 1.25f,
                                        1.5f,
                                        z * 1.5f - 1.25f },
                            (Vector3) { x * 1.5f - 3.25f,
                                        1.5f,
                                        z * 1.5f - 3.25f }}
            );
      }
      ++z;
    }
    ++x;
  }
  if (collision.hit) {
    st->c_select[0] = x - 1;
    st->c_select[1] = z - 1;
  }
}

void draw_board_game(state_t *st, quarto_t *quarto) {
  uint16_t sum = quarto_summary(quarto);
  uint64_t board = quarto_board(quarto);
  for (size_t x = 0; x < 4; ++x) {
    for (size_t z = 0; z < 4; ++z) {
      DrawCube(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          st->c_select[0] == x && st->c_select[1] == z ? GREEN : LIGHTGRAY
          );
      DrawCubeWires(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          BLACK
          );
      if (((sum >> (uint16_t) (x * 4 + z)) & 0b1) != 0) {
        unsigned char p = (board >> (4 * (x * 4 + z))) & 0b1111;
        DrawModelEx(
            st->pieces[p & 0b0011],
            (Vector3) {x * 1.5f - 2.25f, 0.75f, z * 1.5f - 2.25f},
            (Vector3) {0.0f, 0.0f, 0.0f},
            0.0f,
            (Vector3) {0.55f,
                       (p & 0b0100) != 0 ? 0.35f : 0.2f,
                       0.55f},
            (p & 0b1000) != 0 ? BLUE : RED
            );
      }
    }
  }
}

void pieces_selectors(state_t *st, game_state_t *gs) {
  float rect_heigth = st->screens->texture.height / 4.0f;
  float rect_width = st->screens->texture.width / 4.0f;
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      Rectangle r = {
        .x = j * rect_width, .y = i * rect_heigth,
        .width = rect_width, .height = rect_heigth
      };
      bool is_selected = (gs->p_select[0] == 3 - i && gs->p_select[1] == 3 - j);
      if (CheckCollisionPointRec(GetMousePosition(), r) && !is_selected
          && (gs->used >> ((3 - i) * 4 + 3 - j) & 0b1) == 0) {
        DrawRectangleRec(r, Fade(LIGHTGRAY, 0.8f));
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
          gs->p_select[0] = 3 - i;
          gs->p_select[1] = 3 - j;
        }
      } else if (is_selected) {
        DrawRectangleRec(r, Fade(PURPLE, 0.8f));
      } else if ((quarto_current_piece(gs->q) & 0b1111)
          == (3 - i) * 4 + 3 - j) {
        DrawRectangleRec(r, Fade(SKYBLUE, 0.8f));
      }
    }
  }
}

void draw_pieces(state_t *st, game_state_t *gs, Camera3D *camera) {
  camera->position = (Vector3) {
    21.0f, 5.25f, 4.0f
  };
  camera->target = (Vector3) {
    0.0f, 5.25f, 4.0f
  };
  SetShaderValue(
      st->shader,
      st->shader.locs[SHADER_LOC_VECTOR_VIEW],
      &camera->position,
      SHADER_UNIFORM_VEC3
      );
  st->lights[1].position = camera->position;
  UpdateLightValues(st->shader, st->lights[1]);
  BeginTextureMode(*st->screens);
  pieces_selectors(st, gs);
  ClearBackground(BLANK);
  BeginMode3D(*camera);
  BeginShaderMode(st->shader);
  for (size_t x = 0; x < 4; ++x) {
    for (size_t z = 0; z < 4; ++z) {
      DrawModelEx(
          st->pieces[(x * 4 + z) & 0b0011],
          (Vector3) {0.0f, x * 2.85f + 0.25f, z * 2.0f + 1.0f},
          (Vector3) {0.0f, 0.0f, -1.0f},
          32.0f,
          (Vector3) {0.55f,
                     ((x * 4 + z) & 0b0100) != 0 ? 0.2f : 0.1f,
                     0.55f},
          (gs->used >> (x * 4 + z) & 0b1) != 0
          ? DARKGRAY : ((x * 4 + z) & 0b1000) != 0 ? BLUE : RED
          );
    }
  }
  EndShaderMode();
  EndMode3D();
  EndTextureMode();
}
