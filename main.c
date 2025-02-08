#include <stdlib.h>
#include "raylib.h"
#include "menu.h"

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
    .game_name = "Quarto", .exit_wind = true
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
  while (!WindowShouldClose()) {
    if (game_info.exit_wind) {
      if (IsKeyPressed(KEY_Y)) {
        break;
      } else if (IsKeyPressed(KEY_N)) {
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
    if (game_info.exit_wind) {
      int rect_w = game_info.screen_w * 0.65f;
      int rect_h = game_info.screen_h * 0.5f;
      DrawRectangle((game_info.screen_w - rect_w) / 2,
          (game_info.screen_h - rect_h) / 2, rect_w, rect_h, BLACK);
      DrawText("Are you sure you want to exit program? [Y/N]", 40, 180, 30,
          WHITE);
    }
    EndDrawing();
  }
  CloseWindow();
  return EXIT_SUCCESS;
}
