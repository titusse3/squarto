#include <stdlib.h>
#include <stdint.h>

#include "raylib.h"
#include <raymath.h>

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "menu.h"

#include "raygui.h"

#define BASE_SCREEN_WIDTH 1280
#define BASE_SCREEN_HEIGHT 720
#define GAME_NAME "Quarto"

static void draw_game(Camera3D *camera, float select[2], uint16_t *placed,
    Model models[4]);

int main(void) {
  InitWindow(BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, GAME_NAME);
  //SetWindowState(FLAG_WINDOW_MAXIMIZED);
  SetExitKey(0);
  SetTargetFPS(60);
  //
  game_info_t game_info = {
    .screen_w = GetScreenWidth(), .screen_h = GetScreenHeight(),
    .game_name = "Quarto", .exit_wind = false, .play_music = true
  };
  menu_content_t game = {
    .currentScreen = MENU, .menuType = NONE
  };
  InitAudioDevice();
  Music music = LoadMusicStream("resources/music/italian_hymn.mp3");
  PlayMusicStream(music);
  SetMusicVolume(music, 0.1f);
  Camera3D camera = {};
  camera.position = (Vector3) {
    7.5f, 20.0f, 7.5f
  }; // Position de la caméra
  camera.target = (Vector3) {
    0.0f, 0.0f, 0.0f
  }; // Point visé
  camera.up = (Vector3) {
    0.0f, 1.0f, 0.0f
  }; // Vecteur "up"
  camera.fovy = 30.0f; // Angle de vue en Y
  camera.projection = CAMERA_PERSPECTIVE; // Type de projection
  Shader shader = LoadShader(
      "resources/shaders/glsl330/lighting.vs",
      "resources/shaders/glsl330/lighting.fs"
      );
  shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
  shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
  SetShaderValue(
      shader,
      shader.locs[SHADER_LOC_VECTOR_VIEW],
      (float[3]) {
    camera.position.x,
    camera.position.y,
    camera.position.z,
  },
      SHADER_UNIFORM_VEC3
      );
  Light light = CreateLight(
      LIGHT_POINT,
      camera.position,
      Vector3Zero(),
      WHITE,
      shader
      );
  float select[2] = {
    -1.0f, -1.0f
  };
  uint16_t placed = 0;
  Model models[4] = {
    LoadModel("resources/model/HOLE_ROUND.obj"),
    LoadModel("resources/model/HOLE_SQUARE.obj"),
    LoadModel("resources/model/PLAIN_ROUND.obj"),
    LoadModel("resources/model/PLAIN_SQUARE.obj")
  };
  models[0].materials[0].shader = shader;
  models[1].materials[0].shader = shader;
  models[2].materials[0].shader = shader;
  models[3].materials[0].shader = shader;
  Texture2D background = LoadTexture("resources/image/blue-back.png");
  Texture2D stars = LoadTexture("resources/image/blue-stars.png");
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
      UpdateCamera(&camera, 0);
    } else {
      scrollingBack -= 0.1f;
      scrollingMid -= 0.1f;
      if (scrollingBack <= -background.width * 2) {
        scrollingBack = 0;
      }
      if (scrollingMid <= -stars.width * 2) {
        scrollingMid = 0;
      }
      if (game.menuType == RULES) {
        ++game.content.rules_values.rules_frames;
      } else if (game.menuType == HISTORY) {
        ++game.content.history_values.history_frames;
      }
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (game.currentScreen == GAME) {
      BeginMode3D(camera);
      BeginShaderMode(shader);
      draw_game(&camera, select, &placed, models);
      EndShaderMode();
      EndMode3D();
    } else {
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
      display_menu(&game_info, &game, &camera, &shader);
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

void draw_game(Camera3D *camera, float select[2], uint16_t *placed,
    Model models[4]) {
  Ray ray = {};
  RayCollision collision = {};
  float x = 0;
  float z = 0;
  collision.hit = false;
  while (x < 4.0f && !collision.hit) {
    z = 0;
    while (z < 4.0f && !collision.hit) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ray = GetScreenToWorldRay(GetMousePosition(), *camera);
        collision = GetRayCollisionBox(
            ray,
            (BoundingBox) { (Vector3) { x * 1.5f - 1.25f, 1.5f,
                                        z * 1.5f - 1.25f },
                            (Vector3) { x * 1.5f - 3.25f, 1.5f,
                                        z * 1.5f - 3.25f }}
            );
      }
      ++z;
    }
    ++x;
  }
  if (collision.hit) {
    select[0] = x - 1;
    select[1] = z - 1;
    *placed ^= 1 << (int) ((x - 1) * 4 + z - 1);
  }
  for (float x = 0; x < 4.0f; ++x) {
    for (float z = 0; z < 4.0f; ++z) {
      DrawCube(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          select[0] == x && select[1] == z ? GREEN : LIGHTGRAY
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
            models[(size_t) ((size_t) rand() % 4)],
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
  DrawRay(ray, BLUE);
}
