#include <stdlib.h>
#include "raylib.h"
#include "menu.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1800

int main(void) {
  const char *game_title = "Quarto";
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, game_title);
  SetExitKey(0);
  SetTargetFPS(60);
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
  // Boucle principale
  while (!WindowShouldClose()) {
    if (currentScreen == MENU) {
    } else if (currentScreen == GAME) {
      if (IsKeyPressed(KEY_ESCAPE)) {
        currentScreen = MENU;
      }
      UpdateCamera(&camera, 0);
    }
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (currentScreen == MENU) {
      display_menu(SCREEN_WIDTH, SCREEN_HEIGHT, game_title, &currentScreen);
    } else if (currentScreen == GAME) {
      Color leftColor = (Color){
        18, 25, 25, 255
      };  // couleur de gauche et droite
      Color midColor = (Color){
        39, 50, 53, 255
      };  // couleur centrale
      DrawRectangleGradientH(0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, leftColor,
          midColor);
      // Dessiner la seconde moitié : dégradé de midColor -> leftColor
      DrawRectangleGradientH(SCREEN_WIDTH / 2, 0,
          SCREEN_WIDTH - SCREEN_WIDTH / 2,
          SCREEN_HEIGHT, midColor, leftColor);
      //
      BeginMode3D(camera);
      DrawCube(cubePosition, 2.0f, 2.0f, 2.0f, RED);// Cube plein
      DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);// Contour du
      // cube
      DrawGrid(10, 1.0f);// Grille de référence
      EndMode3D();
      DrawText("Appuyez sur ESC pour revenir au menu", 10, 10, 20, WHITE);
    }
    EndDrawing();
  }
  // Fermeture de la fenêtre et nettoyage
  CloseWindow();
  return EXIT_SUCCESS;
}
