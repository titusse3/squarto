#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <raylib.h>
#include <raymath.h>

#include "rlights.h"
#include "menu.h"
#include "raygui.h"

#define BASE_SCREEN_WIDTH 1280
#define BASE_SCREEN_HEIGHT 720
#define GAME_NAME "Quarto"

static void draw_game(state_t *st, game_info_t *game, uint16_t *placed,
    uint16_t *used);

static void display_background(Texture2D background, Texture2D foreground,
    float scrollingBack, float scrollingFore, float scale_bg, float scale_fg);

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
    .sound_play = NONE
  };
  //
  state_t st = {
    .shader = LoadShader(
        "resources/shaders/glsl330/lighting.vs",
        "resources/shaders/glsl330/lighting.fs"
        ),
    .pieces = {
      LoadModel("resources/model/PLAIN_SQUARE.obj"),
      LoadModel("resources/model/PLAIN_ROUND.obj"),
      LoadModel("resources/model/HOLE_SQUARE.obj"),
      LoadModel("resources/model/HOLE_ROUND.obj")
    },
    .c_select = {
      UNDEF_COORD,
      UNDEF_COORD
    },
    .mk_screen = true,
    .screens = nullptr
  };
  //
  st.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(
      st.shader,
      "viewPos"
      );
  for (size_t k = 0; k < NB_PIECES; ++k) {
    st.pieces[k].materials[0].shader = st.shader;
  }
  //
  InitAudioDevice();
  Music music = LoadMusicStream("resources/music/italian_hymn.mp3");
  PlayMusicStream(music);
  SetMusicVolume(music, 0.1f);
  game.sound = LoadSound("resources/sound/select.wav");
  SetSoundVolume(game.sound, 0.6f);
  //
  Texture2D background = LoadTexture("resources/image/blue-back.png");
  Texture2D foreground = LoadTexture("resources/image/blue-stars.png");
  //
  uint16_t placed = 0;
  uint16_t used = 0b0000'0000'0000'0001;
  //
  float scrollingBack = 0.0f;
  float scrollingFore = 0.0f;
  float scale_bg = (float) game_info.screen_w / background.width;
  float scale_fg = (float) game_info.screen_w / foreground.width;
  //
  while (true) {
    //
    scale_bg = (float) game_info.screen_w / background.width;
    scale_fg = (float) game_info.screen_w / foreground.width;
    scrollingBack -= 0.7f;
    scrollingFore -= 1.0f;
    if (scrollingBack <= -background.width * scale_bg) {
      scrollingBack = 0;
    }
    if (scrollingFore <= -foreground.width * scale_fg) {
      scrollingFore = 0;
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
      }
    } else if (game.menuType == RULES) {
      ++game.content.rules_values.rules_frames;
    } else if (game.menuType == HISTORY) {
      ++game.content.history_values.history_frames;
    }
    BeginDrawing();
    display_background(background, foreground,
        scrollingBack,
        scrollingFore,
        scale_bg, scale_fg);
    //
    if (game.currentScreen == GAME) {
      draw_game(&st, &game_info, &placed, &used);
    } else {
      display_menu(&game_info, &game, &st);
    }
    if (game_info.exit_wind && display_exit_menu(&game_info)) {
      break;
    }
    EndDrawing();
  }
  CloseAudioDevice();
  CloseWindow();
  return EXIT_SUCCESS;
}

void display_background(Texture2D background, Texture2D foreground,
    float scrollingBack, float scrollingFore, float scale_bg, float scale_fg) {
  ClearBackground(GetColor(0x052c46ff));
  DrawTextureEx(background, (Vector2){ scrollingBack, 0 }, 0.0f,
      scale_bg, WHITE);
  DrawTextureEx(background,
      (Vector2){ background.width *scale_bg + scrollingBack, 0 }, 0.0f,
      scale_bg, WHITE);
  DrawTextureEx(background,
      (Vector2){ scrollingBack, background.height * scale_bg }, 0.0f,
      scale_bg, WHITE);
  DrawTextureEx(background,
      (Vector2){ background.width *scale_bg + scrollingBack,
                 background.height *scale_bg }, 0.0f,
      scale_bg, WHITE);
  DrawTextureEx(foreground, (Vector2){ scrollingFore, 0 }, 0.0f,
      scale_fg, WHITE);
  DrawTextureEx(foreground,
      (Vector2){ foreground.width *scale_fg + scrollingFore, 0 }, 0.0f,
      scale_fg, WHITE);
  DrawTextureEx(foreground,
      (Vector2){ scrollingFore, foreground.height * scale_fg }, 0.0f,
      scale_fg, WHITE);
  DrawTextureEx(foreground,
      (Vector2){ foreground.width *scale_fg + scrollingFore,
                 foreground.height *scale_fg }, 0.0f,
      scale_fg, WHITE);
}

void draw_game(state_t *st, game_info_t *game, uint16_t *placed,
    uint16_t *used) {
  if (st->mk_screen) {
    st->screens = malloc(sizeof *st->screens);
    if (st->screens == nullptr) {
      // error d'alloc
      display_exit_menu(game);
      return;
    }
    st->screens[0]
      = LoadRenderTexture(game->screen_h / 2, game->screen_h / 1.5);
    st->mk_screen = false;
  }
  >> >> >> > 03f1fb7(Add selection pieces frame)
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
  Light light = CreateLight(
      LIGHT_POINT,
      camera.position,
      Vector3Zero(),
      GRAY,
      st->shader
      );
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  Ray ray = {};
  RayCollision collision = {};
  float x = 0;
  float z = 0;
  collision.hit = false;
  while (x < 4.0f && !collision.hit) {
    z = 0;
    while (z < 4.0f && !collision.hit) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ray = GetScreenToWorldRay(GetMousePosition(), camera);
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
    *placed ^= 1 << (int) ((x - 1) * 4 + z - 1);
  }
  for (float x = 0; x < 4.0f; ++x) {
    for (float z = 0; z < 4.0f; ++z) {
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
      if (((*placed >> (int) (x * 4 + z)) & 1) != 0) {
        srand(x * 4 + z + rand());
        DrawModelEx(
            st->pieces[(size_t) ((size_t) rand() % 4)],
            (Vector3) {x * 1.5f - 2.25f, 0.75f, z * 1.5f - 2.25f},
            (Vector3) {0.0f, 0.0f, 0.0f},
            0.0f,
            (Vector3) {0.55f,
                       (rand() / (double) RAND_MAX) > 0.5f ? 0.35f : 0.2f,
                       0.55f},
            (rand() / (double) RAND_MAX) > 0.5f ? BLUE : RED
            );
      }
    }
  }
  EndShaderMode();
  EndMode3D();
  camera.position = (Vector3) {
    21.0f, 5.25f, 4.0f
  };
  camera.target = (Vector3) {
    0.0f, 5.25f, 4.0f
  };
  SetShaderValue(
      st->shader,
      st->shader.locs[SHADER_LOC_VECTOR_VIEW],
      &camera.position,
      SHADER_UNIFORM_VEC3
      );
  light = CreateLight(
      LIGHT_POINT,
      camera.position,
      Vector3Zero(),
      GRAY,
      st->shader
      );
  BeginTextureMode(*st->screens);
  ClearBackground(BLANK);
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  for (size_t x = 0; x < 4.0f; ++x) {
    for (size_t z = 0; z < 4.0f; ++z) {
      if ((*used >> (x * 4 + z) & 0b1) == 0) {
        DrawModelEx(
            st->pieces[x],
            (Vector3) {0.0f, x * 2.85f + 0.25f, z * 2.0f + 1.0f},
            (Vector3) {0.0f, 0.0f, -1.0f},
            32.0f,
            (Vector3) {0.55f,
                       z % 2 == 1.0f ? 0.2f : 0.1f,
                       0.55f},
            z >= 2.0f ? BLUE : RED
            );
      }
    }
  }
  EndShaderMode();
  EndMode3D();
  EndTextureMode();
  DrawTextureRec(
      st->screens->texture,
      (Rectangle) {0.0f,
                   0.0f,
                   (float) st->screens->texture.width,
                   (float) -st->screens->texture.height},
      (Vector2) {0.0f, 0.0f},
      WHITE
      );
}
