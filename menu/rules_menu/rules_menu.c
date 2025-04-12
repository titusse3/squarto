#include "rules_menu.h"

#include <stdio.h>
#include <string.h>

#include "raygui.h"

// MAX_RULES: Nombre maximum de règles à afficher.
#define MAX_RULES 7

#define RULE_0 "Quarto est un jeu de stratégie pour deux joueur"
#define RULE_1 "Le plateau est une grille de 4x4"
#define RULE_2 "Il y a 16 pièces, chacune avec 4 caractéristiques"

#define RULE_3                                                                 \
        "Le second joueur choisi une pièce\n"                                  \
        "qu'il donne au premier"                                               \

#define RULE_4                                                                 \
        "Le premier joueur place la pièce sur le plateau\n"                    \
        "et on recommance avec le second joueur"                               \

#define RULE_5                                                                 \
        "Le jeu admet plusieurs niveaus de difficulté.\n"                      \
        "A chaque niveau on conserve les conditions de \n"                     \
        "victoire niveau précédente."                                          \

#define RULE_6                                                                 \
        "Dans le premier niveau (" S_EASY "), pour\n"                          \
        "gagner, il faut aligner 4 pièces avec une \n"                         \
        "caractéristique commune."                                             \

#define RULE_7                                                                 \
        "Dans le second niveau (" S_MEDIUM "), pour\n"                         \
        "gagner, il faut former petit carré de 4 pièces\n"                     \
        "avec une caractéristique commune."                                    \

#define RULE_8                                                                 \
        "Dans le troisieme niveau (" S_HARD "), pour\n"                        \
        "gagner, il faut former grand carré de 4 pièces\n"                     \
        "avec une caractéristique commune."                                    \

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static void bottom_corner_pagging(menu_content_t *menu, Rectangle parent) {
  float triWidth = parent.width * 0.15f;
  float triHeight = parent.height * 0.15f;
  Vector2 vertex1 = {
    parent.x + parent.width,
    parent.y + parent.height - triHeight
  };
  Vector2 vertex2 = {
    parent.x + parent.width - triWidth,
    parent.y + parent.height
  };
  Vector2 vertex3 = {
    parent.x + parent.width,
    parent.y + parent.height
  };
  DrawTriangle(vertex1, vertex2, vertex3, BLUE);
  char s[8];
  snprintf(s, 7, "%d/%d", menu->content.rules_values.rules_num + 1,
      MAX_RULES + 1);
  int tri_fsize = (int) (triHeight * 0.25f);
  float center_x = (vertex1.x + vertex2.x + vertex3.x) / 3.0f;
  float center_y = (vertex1.y + vertex2.y + vertex3.y) / 3.0f;
  DrawText(s, center_x - MeasureText(s, tri_fsize) / 2,
      center_y - tri_fsize / 3, tri_fsize, WHITE);
}

static void display_rule_bottom_btn(game_info_t *game, menu_content_t *menu,
    Rectangle *container) {
  float btnPadHor = container->width * 0.01f;
  float btnPadVer = container->height * 0.04f;
  float totalButtonsWidth = container->width * 0.7f;
  int buttonWidth = (int) ((totalButtonsWidth - 4 * btnPadHor) / 3.0f);
  int buttonHeight = (int) (container->height * 0.09f);
  GuiSetStyle(DEFAULT, TEXT_SIZE, buttonHeight);
  int buttonY = (int) (container->y + container->height - buttonHeight
      - btnPadVer);
  float groupWidth = 3 * buttonWidth + 4 * btnPadHor;
  float startX = container->x + (container->width - groupWidth) / 2;
  Rectangle prevButton = {
    startX + btnPadHor, buttonY, buttonWidth, buttonHeight
  };
  Rectangle midButton = {
    startX + 2 * btnPadHor + buttonWidth, buttonY, buttonWidth, buttonHeight
  };
  Rectangle nextButton = {
    startX + 3 * btnPadHor + 2 * buttonWidth, buttonY, buttonWidth, buttonHeight
  };
  if (GuiButton(prevButton, "#130#") || IsKeyPressed(KEY_LEFT)) {
    if (menu->content.rules_values.rules_num != 0) {
      --menu->content.rules_values.rules_num;
      menu->content.rules_values.rules_frames = 0;
    }
  }
  if (GuiButton(midButton, "#134#") || IsKeyPressed(KEY_SPACE)) {
    menu->content.rules_values.rules_frames += 60;
  }
  if (GuiButton(nextButton, "#131#") || IsKeyPressed(KEY_RIGHT)) {
    if (menu->content.rules_values.rules_num != MAX_RULES) {
      ++menu->content.rules_values.rules_num;
      menu->content.rules_values.rules_frames = 0;
    }
  }
}

static void display_rule_frame_1(menu_content_t *menu, state_t *st,
    Rectangle *rulesRect) {
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
  st->lights[1].position = camera.position;
  UpdateLightValues(st->shader, st->lights[1]);
  float n = 0.75f - (MIN(menu->content.rules_values.rules_frames,
      150)) / 200.0f;
  BeginTextureMode(*st->screens);
  ClearBackground(BLANK);
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  for (float x = 0; x < 4.0f; ++x) {
    for (float z = 0; z < 4.0f; ++z) {
      DrawCube(
          (Vector3) {(x - n) * 1.5f - 2.25f, 0.0f,
                     (z - n) * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          LIGHTGRAY
          );
      DrawCubeWires(
          (Vector3) {(x - n) * 1.5f - 2.25f, 0.0f,
                     (z - n) * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          BLACK
          );
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
      (Vector2) {rulesRect->x,
                 rulesRect->y + rulesRect->height / 4},
      WHITE
      );
}

static void display_rule_frame_2(menu_content_t *menu, state_t *st,
    Rectangle *rulesRect) {
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
  st->lights[1].position = camera.position;
  UpdateLightValues(st->shader, st->lights[1]);
  BeginTextureMode(*st->screens);
  ClearBackground(BLANK);
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  for (float x = 0; x < 4.0f; ++x) {
    for (float z = 0; z < 4.0f; ++z) {
      DrawModelEx(
          st->pieces[(size_t) x],
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          (Vector3) {0.0f, 0.0f, 0.0f},
          0.0f,
          (Vector3) {0.55f,
                     (long int) z % 2 == 0 > 0.5f ? 0.35f : 0.2f,
                     0.55f},
          z >= 2.0f > 0.5f ? BLUE : RED);
    }
  }
  EndShaderMode();
  EndMode3D();
  EndTextureMode();
  DrawTextureRec(st->screens->texture,
      (Rectangle) {0.0f, 0.0f, (float) st->screens->texture.width,
                   (float) -st->screens->texture.height},
      (Vector2) {rulesRect->x, rulesRect->y + rulesRect->height / 4}, WHITE);
}

static void display_rule_frame_3(menu_content_t *menu, state_t *st,
    Rectangle *rulesRect) {
  Camera3D camera = {
    .position = (Vector3) {
      10.0f, 20.0f, 10.0f
    },
    .target = (Vector3) {
      0.0f, 0.0f, 0.0f
    },
    .up = (Vector3) {
      0.0f, 1.0f, 0.0f
    }, .fovy = 30.0f, .projection = CAMERA_PERSPECTIVE
  };
  SetShaderValue(
      st->shader,
      st->shader.locs[SHADER_LOC_VECTOR_VIEW],
      &camera.position,
      SHADER_UNIFORM_VEC3
      );
  st->lights[0].position = camera.position;
  UpdateLightValues(st->shader, st->lights[0]);
  st->lights[1].position = camera.position;
  UpdateLightValues(st->shader, st->lights[1]);
  BeginTextureMode(*st->screens);
  ClearBackground(BLANK);
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  float piece = MIN(menu->content.rules_values.rules_frames - 170, 60) / 60.0f;
  DrawModelEx(
      st->pieces[0],
      (Vector3) {-4.0f * piece + 2.0f, 0.0f, 4.0f * piece - 2.0f},
      (Vector3) {0.0f, 0.0f, 0.0f},
      0.0f,
      (Vector3) {0.55f, 0.2f, 0.55},
      RED
      );
  EndShaderMode();
  EndMode3D();
  EndTextureMode();
  float scale = 0.4f;
  int img_width = menu->content.rules_values.rules_textures[0].width;
  int total_width = 2 * (img_width * scale) + SPACE_BETWEEN_BUTTONS;
  int start_x = rulesRect->x + (rulesRect->width - total_width) / 2;
  int final_y = rulesRect->y + rulesRect->height / 2.5f;
  Vector2 pos1 = {
    start_x, final_y
  };
  Vector2 pos2 = {
    start_x + (img_width * scale) + SPACE_BETWEEN_BUTTONS, final_y
  };
  DrawTextureEx(menu->content.rules_values.rules_textures[0], pos1, 0.0f,
      scale, WHITE);
  DrawTextureEx(menu->content.rules_values.rules_textures[1], pos2, 0.0f,
      scale, WHITE);
  //
  DrawTextureRec(
      st->screens->texture,
      (Rectangle) {0.0f,
                   0.0f,
                   (float) st->screens->texture.width,
                   (float) -st->screens->texture.height},
      (Vector2) {rulesRect->x,
                 rulesRect->y + rulesRect->height / 4},
      WHITE
      );
}

static void display_rule_frame_4(menu_content_t *menu, state_t *st,
    Rectangle *rulesRect) {
  Camera3D camera = {
    .position = (Vector3) {
      7.5f, 20.0f, 7.5f
    },
    .target = (Vector3) {
      0.0f, 0.0f, 0.0f
    },
    .up = (Vector3) {
      0.0f, 1.0f, 0.0f
    }, .fovy = 30.0f, .projection = CAMERA_PERSPECTIVE
  };
  SetShaderValue(
      st->shader,
      st->shader.locs[SHADER_LOC_VECTOR_VIEW],
      &camera.position,
      SHADER_UNIFORM_VEC3
      );
  st->lights[0].position = camera.position;
  UpdateLightValues(st->shader, st->lights[0]);
  st->lights[1].position = camera.position;
  UpdateLightValues(st->shader, st->lights[1]);
  BeginTextureMode(*st->screens);
  ClearBackground(BLANK);
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  for (float x = 0; x < 4.0f; ++x) {
    for (float z = 0; z < 4.0f; ++z) {
      DrawCube(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          LIGHTGRAY
          );
      DrawCubeWires(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          BLACK
          );
    }
  }
  float t = MIN(menu->content.rules_values.rules_frames - 260, 60) / 60.0f;
  Vector3 start_pos = {
    2 * 1.5f - 2.25f, 5.0f, 2 * 1.5f - 2.25f
  };
  Vector3 end_pos = {
    2 * 1.5f - 2.25f, 0.75f, 2 * 1.5f - 2.25f
  };
  Vector3 current_pos = {
    start_pos.x,
    start_pos.y + (end_pos.y - start_pos.y) * t,
    start_pos.z
  };
  DrawModelEx(
      st->pieces[0],
      current_pos,
      (Vector3) {0.0f, 0.0f, 0.0f},
      0.0f,
      (Vector3) {0.55f, 0.2f, 0.55},
      RED
      );
  EndShaderMode();
  EndMode3D();
  EndTextureMode();
  DrawTextureRec(
      st->screens->texture,
      (Rectangle) {0.0f,
                   0.0f,
                   (float) st->screens->texture.width,
                   (float) -st->screens->texture.height},
      (Vector2) {rulesRect->x,
                 rulesRect->y + rulesRect->height / 4},
      WHITE
      );
}

static void display_rule_frame_5(menu_content_t *menu, state_t *st,
    Rectangle *rulesRect) {
  const char *difficulty_options[] = {
    S_EASY, S_MEDIUM, S_HARD, S_VERY_HARD
  };
  int num_options = sizeof difficulty_options / sizeof *difficulty_options;
  float container_width = rulesRect->width * 0.6f;
  float container_height = rulesRect->height * 0.5f;
  Rectangle container = {
    rulesRect->x + (rulesRect->width - container_width) / 2,
    rulesRect->height - rulesRect->y - (rulesRect->height - container_height)
    * 0.95f,
    container_width,
    container_height
  };
  int button_height = container.height / (num_options * 1.5);
  int button_width = container.width * 0.9f;
  int spacing = container.height / (num_options * 5);
  int total_height = num_options * button_height + (num_options - 1) * spacing;
  int start_y = container.y + (container.height - total_height) / 2;
  for (int i = 0; i < num_options; i++) {
    Rectangle button_rect = {
      container.x + (container.width - button_width) / 2,
      start_y + i * (button_height + spacing),
      button_width,
      button_height
    };
    Color button_color = BLACK;
    Color text_color = PURPLE;
    DrawRectangleRec(button_rect, button_color);
    DrawRectangleLinesEx(button_rect, 2, PURPLE);
    int title_font_size = container.width * 0.05f;
    Vector2 title_size = MeasureTextEx(GetFontDefault(),
        difficulty_options[i], title_font_size, 1);
    DrawTextEx(GetFontDefault(), difficulty_options[i],
        (Vector2){ button_rect.x + (button_rect.width - title_size.x) / 2,
                   button_rect.y + (button_height - title_size.y) / 2 },
        title_font_size, 1, text_color);
  }
}

static void display_rule_frame_6(menu_content_t *menu, state_t *st,
    Rectangle *rulesRect) {
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
  st->lights[1].position = camera.position;
  UpdateLightValues(st->shader, st->lights[1]);
  BeginTextureMode(*st->screens);
  ClearBackground(BLANK);
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  for (float x = 0; x < 4.0f; ++x) {
    for (float z = 0; z < 4.0f; ++z) {
      DrawCube(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          LIGHTGRAY
          );
      DrawCubeWires(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          BLACK
          );
      if (x == 2) {
        DrawModelEx(
            st->pieces[(size_t) (z + 1) % 4],
            (Vector3) {x * 1.5f - 2.25f, 0.75f, z * 1.5f - 2.25f},
            (Vector3) {0.0f, 0.0f, 0.0f},
            0.0f,
            (Vector3) {0.55f, 0.35f, 0.55},
            (long int) z % 2 != 0 ? BLUE : RED
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
      (Vector2) {rulesRect->x,
                 rulesRect->y + rulesRect->height / 4}, WHITE);
}

static void display_rule_frame_7(menu_content_t *menu, state_t *st,
    Rectangle *rulesRect) {
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
  st->lights[1].position = camera.position;
  UpdateLightValues(st->shader, st->lights[1]);
  BeginTextureMode(*st->screens);
  ClearBackground(BLANK);
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  for (float x = 0; x < 4.0f; ++x) {
    for (float z = 0; z < 4.0f; ++z) {
      DrawCube(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          LIGHTGRAY
          );
      DrawCubeWires(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          BLACK
          );
      if (x < 3 && x > 0 && z < 3 && z > 0) {
        DrawModelEx(
            st->pieces[(size_t) ((x - 1) * 2 + (z - 1))],
            (Vector3) {x * 1.5f - 2.25f, 0.75f, z * 1.5f - 2.25f},
            (Vector3) {0.0f, 0.0f, 0.0f},
            0.0f,
            (Vector3) {0.55f, 0.35f, 0.55},
            (long int) z % 2 != 0 ? BLUE : RED
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
      (Vector2) {rulesRect->x,
                 rulesRect->y + rulesRect->height / 4}, WHITE);
}

static void display_rule_frame_8(menu_content_t *menu, state_t *st,
    Rectangle *rulesRect) {
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
  st->lights[1].position = camera.position;
  UpdateLightValues(st->shader, st->lights[1]);
  BeginTextureMode(*st->screens);
  ClearBackground(BLANK);
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  for (float x = 0; x < 4.0f; ++x) {
    for (float z = 0; z < 4.0f; ++z) {
      DrawCube(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          LIGHTGRAY
          );
      DrawCubeWires(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          BLACK
          );
      if (x < 3 && x > 0 && z < 3 && z > 0) {
        DrawModelEx(
            st->pieces[(size_t) ((x - 1) * 2 + (z - 1))],
            (Vector3) {x * 1.5f - 2.25f, 0.75f, z * 1.5f - 2.25f},
            (Vector3) {0.0f, 0.0f, 0.0f},
            0.0f,
            (Vector3) {0.55f, 0.35f, 0.55},
            (long int) z % 2 != 0 ? BLUE : RED
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
      (Vector2) {rulesRect->x,
                 rulesRect->y + rulesRect->height / 4}, WHITE);
}

void display_rules(game_info_t *game, int left_padding, int offset,
    int font_size, menu_content_t *menu, state_t *st) {
  DISPLAY_BCK_WINDOW(game, left_padding, offset);
  if (st->mk_screen) {
    st->screens = malloc(sizeof *st->screens);
    if (st->screens == nullptr) {
      // error d'alloc
      ERROR_DISPLAY(game, ALLOCATION_MSG);
    }
    *st->screens = LoadRenderTexture(rulesRect.width, rulesRect.height / 1.5f);
    st->mk_screen = false;
  }
  //
  Rectangle pink = rect_top_corner_title("Rules", rulesRect, font_size,
      PURPLE);
  bottom_corner_pagging(menu, rulesRect);
  //
  display_rule_bottom_btn(game, menu, &rulesRect);
  int foffset = menu->content.rules_values.rules_frames / 3;
  int mfont_size = rulesRect.width / 30;
  switch (menu->content.rules_values.rules_num) {
    case 0:
      if (display_text_writing(game, menu, &rulesRect, RULE_0, mfont_size,
          foffset)) {
        float t = menu->content.rules_values.rules_frames / 180.0f;
        if (t > 1.0f) {
          t = 1.0f;
        }
        display_img_player(menu, &rulesRect, t);
      }
      break;
    case 1:
      if (display_text_writing(game, menu, &rulesRect, RULE_1, mfont_size,
          foffset)) {
        display_rule_frame_1(menu, st, &rulesRect);
      }
      break;
    case 2:
      if (display_text_writing(game, menu, &rulesRect, RULE_2, mfont_size,
          foffset)) {
        display_rule_frame_2(menu, st, &rulesRect);
      }
      break;
    case 3:
      if (display_text_writing(game, menu, &rulesRect, RULE_3, mfont_size,
          foffset)) {
        display_rule_frame_3(menu, st, &rulesRect);
      }
      break;
    case 4:
      if (display_text_writing(game, menu, &rulesRect, RULE_4, mfont_size,
          foffset)) {
        display_rule_frame_4(menu, st, &rulesRect);
      }
      break;
    case 5:
      if (display_text_writing(game, menu, &rulesRect, RULE_5, mfont_size,
          foffset)) {
        display_rule_frame_5(menu, st, &rulesRect);
      }
      break;
    case 6:
      if (display_text_writing(game, menu, &rulesRect, RULE_6, mfont_size,
          foffset)) {
        display_rule_frame_6(menu, st, &rulesRect);
      }
      break;
    case 7:
      if (display_text_writing(game, menu, &rulesRect, RULE_7, mfont_size,
          foffset)) {
        display_rule_frame_7(menu, st, &rulesRect);
      }
      break;
    default:
      break;
  }
}
