#include "rules_menu.h"

#include <stdio.h>
#include <string.h>

#include "raygui.h"

#define RULE_ZERO "Quarto est un jeu de stratégie pour deux joueur"
#define RULE_ONE "Le plateau est une grille de 4x4"
#define RULE_TWO "Il y a 16 pièces, chacune avec 4 caractéristiques"
#define RULE_THREE                                                             \
        "L'objectif du jeu est d'aligner 4 pièces\n avec une caractéristique " \
        "commune"
#define RULE_FOUR                                                              \
        "Le second joueur choisi une pièce\n qu'il donne au premier"
#define RULE_FIVE                                                              \
        "Le premier joueur place la pièce sur le plateau\n et on recommance "  \
        "avec le second joueur"
#define RULE_SIX                                                               \
        "Le jeu se termine lorsqu'un joueur aligne 4 pièces"

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
      if (x == 2) {
        DrawModelEx(
            st->pieces[(size_t) z],
            (Vector3) {x * 1.5f - 2.25f, 0.75f, z * 1.5f - 2.25f},
            (Vector3) {0.0f, 0.0f, 0.0f},
            0.0f,
            (Vector3) {0.55f, 0.2f, 0.55},
            (long int) z % 2 == 0 ? BLUE : RED
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
                 rulesRect->y + rulesRect->height / 4},
      WHITE
      );
}

static void display_rule_frame_4(menu_content_t *menu, state_t *st,
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

static void display_rule_frame_5(menu_content_t *menu, state_t *st,
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

void display_rules(game_info_t *game, int left_padding, int offset,
    int font_size, menu_content_t *menu, state_t *st) {
  DISPLAY_BCK_WINDOW(game, left_padding, offset);
  if (st->mk_screen) {
    st->screens = malloc(sizeof *st->screens);
    if (st->screens == nullptr) {
      // error d'alloc
      display_exit_menu(game, font_size, "Error during allocation",
          MeasureText("Error during allocation", font_size));
      return;
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
      if (display_text_writing(game, menu, &rulesRect, RULE_ZERO, mfont_size,
          foffset)) {
        float t = menu->content.rules_values.rules_frames / 180.0f;
        if (t > 1.0f) {
          t = 1.0f;
        }
        display_img_player(menu, &rulesRect, t);
      }
      break;
    case 1:
      if (display_text_writing(game, menu, &rulesRect, RULE_ONE, mfont_size,
          foffset)) {
        display_rule_frame_1(menu, st, &rulesRect);
      }
      break;
    case 2:
      if (display_text_writing(game, menu, &rulesRect, RULE_TWO, mfont_size,
          foffset)) {
        display_rule_frame_2(menu, st, &rulesRect);
      }
      break;
    case 3:
      if (display_text_writing(game, menu, &rulesRect, RULE_THREE, mfont_size,
          foffset)) {
        display_rule_frame_3(menu, st, &rulesRect);
      }
      break;
    case 4:
      if (display_text_writing(game, menu, &rulesRect, RULE_FOUR, mfont_size,
          foffset)) {
        display_rule_frame_4(menu, st, &rulesRect);
      }
      break;
    case 5:
      if (display_text_writing(game, menu, &rulesRect, RULE_FIVE, mfont_size,
          foffset)) {
        display_rule_frame_5(menu, st, &rulesRect);
      }
      break;
    case 6:
      if (display_text_writing(game, menu, &rulesRect, RULE_SIX, mfont_size,
          foffset)) {
        display_rule_frame_6(menu, st, &rulesRect);
      }
      break;
    default:
      break;
  }
}
