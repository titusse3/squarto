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

static void draw_game(state_t *st, uint16_t *placed);

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
    .menuType = NONE
  };
  //
  state_t st = {
    .shader = LoadShader(
        "resources/shaders/glsl330/lighting.vs",
        "resources/shaders/glsl330/lighting.fs"
        ),
    .pieces = {
      LoadModel("resources/model/HOLE_ROUND.obj"),
      LoadModel("resources/model/PLAIN_ROUND.obj"),
      LoadModel("resources/model/HOLE_SQUARE.obj"),
      LoadModel("resources/model/PLAIN_SQUARE.obj")
    },
    .c_select = {
      UNDEF_COORD,
      UNDEF_COORD
    },
    .subscreen = LoadRenderTexture(game_info.screen_w, game_info.screen_h)
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
  //
  Texture2D background = LoadTexture("resources/image/blue-back.png");
  Texture2D stars = LoadTexture("resources/image/blue-stars.png");
  //
  uint16_t placed = 0;
  //
  float scrollingBack = 0.0f;
  float scrollingMid = 0.0f;
  while (true) {
    game_info.screen_w = GetScreenWidth();
    game_info.screen_h = GetScreenHeight();
    if (WindowShouldClose()) {
      game_info.exit_wind = true;
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
      }
    } else if (game.menuType == RULES) {
      ++game.content.rules_values.rules_frames;
    } else if (game.menuType == HISTORY) {
      ++game.content.history_values.history_frames;
    }
    scrollingBack -= 0.1f;
    scrollingMid -= 0.1f;
    if (scrollingBack <= -background.width * 2) {
      scrollingBack = 0;
    }
    if (scrollingMid <= -stars.width * 2) {
      scrollingMid = 0;
    }
    BeginDrawing();
    // a refaire
    ClearBackground(GetColor(0x052c46ff));
    DrawTextureEx(background, (Vector2){ scrollingMid, 0 }, 0.0f,
        (float) game_info.screen_w / background.width, WHITE);
    DrawTextureEx(background,
        (Vector2){ background.width + scrollingMid, 0 }, 0.0f,
        (float) game_info.screen_w / background.width, WHITE);
    DrawTextureEx(stars, (Vector2){ scrollingBack, 0 }, 0.0f,
        (float) game_info.screen_w / stars.width, WHITE);
    DrawTextureEx(stars,
        (Vector2){ stars.width + scrollingBack, 0 }, 0.0f,
        (float) game_info.screen_w / stars.width, WHITE);
    DrawRectangle(0, 0, game_info.screen_w, game_info.screen_h,
        Fade(BLACK, 0.25f));
    if (game.currentScreen == GAME) {
      draw_game(&st, &placed);
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

void draw_game(state_t *st, uint16_t *placed) {
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
        srand(x * 4 + z);
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
}
