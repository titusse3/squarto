#include <stdlib.h>

#include "raylib.h"
#include <raymath.h>

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "menu.h"

#include "raygui.h"

#define BASE_SCREEN_WIDTH 1280
#define BASE_SCREEN_HEIGHT 720
#define GAME_NAME "Quarto"

int main(void) {
  InitWindow(BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, GAME_NAME);
  SetWindowState(FLAG_WINDOW_MAXIMIZED);
  SetExitKey(0);
  SetTargetFPS(60);
  //
  game_info_t game_info = {
    .screen_w = GetScreenWidth(), .screen_h = GetScreenHeight(),
    .game_name = "Quarto", .exit_wind = false
  };
  menu_content_t game = {
    .currentScreen = MENU, .menuType = NONE
  };
  Camera3D camera = {};
  camera.position = (Vector3) {
    10.0f, 20.0f, 10.0f
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
  Model model = LoadModel("resources/model/pp1.obj");
  while (true) {
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
    if (game.currentScreen == GAME) {
      if (IsKeyPressed(KEY_ESCAPE)) {
        game.currentScreen = MENU;
      }
      UpdateCamera(&camera, 0);
    } else if (game.menuType == RULES) {
      ++game.content.rules_values.rules_frames;
    } else if (game.menuType == HISTORY) {
      ++game.content.history_values.history_frames;
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (game.currentScreen == GAME) {
      BeginMode3D(camera);
      BeginShaderMode(shader);
      DrawModel(model, (Vector3) {0.0f, 0.0f, 0.0f}, 0.5f, RED);
      EndShaderMode();
      EndMode3D();
    } else {
      display_menu(&game_info, &game, &camera, &shader);
    }
    if (game_info.exit_wind && display_exit_menu(&game_info)) {
      break;
    }
    EndDrawing();
  }
  CloseWindow();
  return EXIT_SUCCESS;
}
