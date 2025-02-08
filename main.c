#include <stdlib.h>
#include <stdint.h>

#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

#include "menu.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(void) {
  const char *game_title = "Quarto";
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, game_title);
  SetExitKey(0);
  SetTargetFPS(60);
  GameScreen currentScreen = MENU;
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
  camera.fovy = 20.0f; // Angle de vue en Y
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
  Ray ray = {};
  RayCollision collision = {};
  float select[2] = {
    -1.0f, -1.0f
  };
  uint16_t placed = 0;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    BeginMode3D(camera);
    BeginShaderMode(shader);
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
              (BoundingBox) { (Vector3) { x - 2.0f, 0.5f, z - 2.0f },
                              (Vector3) { x - 1.0f, 0.5f, z - 1.0f }}
              );
        }
        ++z;
      }
      ++x;
    }
    if (collision.hit) {
      select[0] = x - 1;
      select[1] = z - 1;
      placed ^= 1 << (int) ((x - 1) * 4 + z - 1);
    }
    for (float x = 0; x < 4.0f; ++x) {
      for (float z = 0; z < 4.0f; ++z) {
        DrawCube(
            (Vector3) {x - 1.5f, 0, z - 1.5f},
            1.0f,
            1.0f,
            1.0f,
            select[0] == x && select[1] == z ? RED : LIGHTGRAY
            );
        DrawCubeWires(
            (Vector3) {x - 1.5f, 0, z - 1.5f},
            1.0f,
            1.0f,
            1.0f,
            BLACK
            );
        if (((placed >> (int) (x * 4 + z)) & 1) != 0) {
          DrawCube(
              (Vector3) {x - 1.375f, 1.0f, z - 1.375f},
              0.5f,
              0.5f,
              0.5f,
              GOLD
              );
        }
      }
    }
    EndShaderMode();
    DrawRay(ray, BLUE);
    EndMode3D();
    EndDrawing();
  }
  UnloadShader(shader);
  CloseWindow();
  return EXIT_SUCCESS;
}
