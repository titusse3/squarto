#include <stdlib.h>
#include "raylib.h"
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
  Camera3D camera = {
    0
  };
  camera.position = (Vector3){
    10.0f, 10.0f, 10.0f
  };   // Position de la caméra
  camera.target = (Vector3){
    0.0f, 0.0f, 0.0f
  };        // Point visé
  camera.up = (Vector3){
    0.0f, 1.0f, 0.0f
  };        // Vecteur "up"
  camera.fovy = 45.0f;                               // Angle de vue en Y
  camera.projection = CAMERA_PERSPECTIVE;                // Type de projection
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
      DrawCube((Vector3) {0.0f, 0.0f, 0.0f}, 2.0f, 2.0f, 2.0f, RED);
      EndMode3D();
    } else {
      display_menu(&game_info, &game);
    }
    if (game_info.exit_wind && display_exit_menu(&game_info)) {
      break;
    }
    EndDrawing();
  }
  CloseWindow();
  return EXIT_SUCCESS;
}
