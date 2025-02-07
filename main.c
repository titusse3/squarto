#include <stdlib.h>
#include "raylib.h"
#include "menu.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_NAME "Quarto"

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_NAME);
  SetWindowState(FLAG_WINDOW_MAXIMIZED);
  int screen_w = GetScreenWidth();
  int screen_h = GetScreenHeight();
  SetExitKey(0);
  GameScreen currentScreen = MENU;
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
  // Variable pour le cube de test en mode 3D
  Vector3 cubePosition = {
    0.0f, 0.0f, 0.0f
  };
  while (!WindowShouldClose()) {
    if (currentScreen == GAME) {
      if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
      }
      UpdateCamera(&camera, 0);
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (currentScreen == GAME) {
      Color leftColor = (Color){
        18, 25, 25, 255
      };  // couleur de gauche et droite
      Color midColor = (Color){
        39, 50, 53, 255
      };  // couleur centrale
      //
      BeginMode3D(camera);
      DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED); // Cube plein
      DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON); // Contour du
      // cube
      DrawGrid(10, 1.0f); // Grille de référence
      EndMode3D();
      DrawText("Appuyez sur ESC pour revenir au menu", 10, 10, 20, WHITE);
    } else {
      display_menu(screen_w, screen_h, GAME_NAME, &currentScreen);
    }
    EndDrawing();
  }
  // Fermeture de la fenêtre et nettoyage
  CloseWindow();
  return EXIT_SUCCESS;
}
