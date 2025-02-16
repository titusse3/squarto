#include "menu.h"

#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "rlights.h"

#define MENU_PADDING 10

#define TITLE_DIVIDER 8
#define TITLE_POS_DIV 6

#define TEXT_PADDING 20
#define SPACE_BETWEEN_BUTTONS 11

#define MAX_RULES 6

#define RULE_ZERO "Quarto est un jeu de stratégie pour deux joueur"
#define RULE_ONE "Le plateau est une grille de 4x4"
#define RULE_TWO "Il y a 16 pièces, chacune avec 4 caractéristiques"
#define RULE_THREE                                                             \
        "L'objectif du jeu est d'aligner 4 pièces\n avec une caractéristique " \
        "commune"
#define RULE_FOUR                                                              \
        "Le second joueur choisi une pièce\n qu'il donne au premier"
#define RULE_FIVE                                                              \
        "Le premier joueur place la pièce sur le plateau\n et donne une pièce" \
        "au second joueur"
#define RULE_SIX                                                               \
        "Le jeu se termine lorsqu'un joueur aligne 4 pièces"

#define HISTORY_TXT                                                            \
        "Quarto est un jeu de stratégie abstrait inventé en 1991 par le\n"     \
        "Suisse Blaise Müller. Ce jeu se distingue par son concept original:"  \
        "\nvotre adversaire choisit la pièce que vous allez jouer parmi \n"    \
        "16 pièces uniques, chacune possédant quatre attributs différents.\n"  \
        "Malgré des règles simples, Quarto offre une profondeur tactique \n"   \
        "remarquable et a rapidement séduit les amateurs de jeux de réflexion" \
        "\nà travers le monde."

#define MIN(a, b) ((a) < (b) ? (a) : (b))

Texture2D *load_rules_ressources(void) {
  Texture2D *textures = malloc(2 * sizeof(Texture2D));
  textures[0] = LoadTexture("resources/image/p1.png");
  textures[1] = LoadTexture("resources/image/p2.png");
  return textures;
}

static void display_img_player(menu_content_t *menu, Rectangle *container,
    float t) {
  float scale = 0.4f;
  int img_width = menu->content.rules_values.rules_textures[0].width;
  int img_height = menu->content.rules_values.rules_textures[0].height;
  int scaled_img_width = (int) (img_width * scale);
  int total_width = 2 * scaled_img_width + SPACE_BETWEEN_BUTTONS;
  int start_x = container->x + (container->width - total_width) / 2;
  int initial_y = container->y + container->height / 5;
  int final_y = container->y + container->height / 2.5f;
  int current_y = initial_y + (int) ((final_y - initial_y) * t);
  Vector2 pos1 = {
    start_x, (float) current_y
  };
  Vector2 pos2 = {
    start_x + scaled_img_width + SPACE_BETWEEN_BUTTONS, (float) current_y
  };
  DrawTextureEx(menu->content.rules_values.rules_textures[0], pos1, 0.0f,
      scale, WHITE);
  DrawTextureEx(menu->content.rules_values.rules_textures[1], pos2, 0.0f,
      scale, WHITE);
}

static bool display_text_writing(game_info_t *game, menu_content_t *menu,
    Rectangle *container, const char *text) {
  int font_size = container->height / 20;
  int l = MeasureText(text, font_size);
  int number_to_display = menu->content.rules_values.rules_frames / 3;
  DrawText(TextSubtext(text, 0, number_to_display),
      container->x + (container->width - l) / 2,
      container->y + container->height / 5, font_size, LIGHTGRAY);
  return number_to_display >= strlen(text);
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
  Light light = CreateLight(
      LIGHT_POINT,
      camera.position,
      Vector3Zero(),
      GRAY,
      st->shader
      );
  float n = 0.75f - (MIN(menu->content.rules_values.rules_frames,
      150)) / 200.0f;
  BeginTextureMode(st->subscreen);
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
  DrawTexturePro(
      st->subscreen.texture,
      (Rectangle) {0.0f,
                   0.0f,
                   (float) st->subscreen.texture.width,
                   (float) -st->subscreen.texture.height},
      (Rectangle) {rulesRect->x + 50.0f,
                   rulesRect->y + rulesRect->height / 4,
                   rulesRect->width - 100.0f,
                   (rulesRect->width - 100.0f) * st->subscreen.texture.height
                   / st->subscreen.texture.width},
      (Vector2) {0.0f, 0.0f},
      0.0f,
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
  Light light = CreateLight(
      LIGHT_POINT,
      camera.position,
      Vector3Zero(),
      GRAY,
      st->shader
      );
  BeginTextureMode(st->subscreen);
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
          z >= 2.0f > 0.5f ? BLUE : RED
          );
    }
  }
  EndShaderMode();
  EndMode3D();
  EndTextureMode();
  DrawTexturePro(
      st->subscreen.texture,
      (Rectangle) {0.0f,
                   0.0f,
                   (float) st->subscreen.texture.width,
                   (float) -st->subscreen.texture.height},
      (Rectangle) {rulesRect->x + 50.0f,
                   rulesRect->y + rulesRect->height / 4,
                   rulesRect->width - 100.0f,
                   (rulesRect->width - 100.0f) * st->subscreen.texture.height
                   / st->subscreen.texture.width},
      (Vector2) {0.0f, 0.0f},
      0.0f,
      WHITE
      );
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
  BeginTextureMode(st->subscreen);
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
          st->c_select[0] == x && st->c_select[1] == z ? GREEN : LIGHTGRAY
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
  DrawTexturePro(
      st->subscreen.texture,
      (Rectangle) {0.0f,
                   0.0f,
                   (float) st->subscreen.texture.width,
                   (float) -st->subscreen.texture.height},
      (Rectangle) {rulesRect->x + 50.0f,
                   rulesRect->y + rulesRect->height / 4,
                   rulesRect->width - 100.0f,
                   (rulesRect->width - 100.0f) * st->subscreen.texture.height
                   / st->subscreen.texture.width},
      (Vector2) {0.0f, 0.0f},
      0.0f,
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
  Light light = CreateLight(
      LIGHT_POINT,
      camera.position,
      Vector3Zero(),
      GRAY,
      st->shader
      );
  BeginTextureMode(st->subscreen);
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
          st->c_select[0] == x && st->c_select[1] == z ? GREEN : LIGHTGRAY
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
  DrawTexturePro(
      st->subscreen.texture,
      (Rectangle) {0.0f,
                   0.0f,
                   (float) st->subscreen.texture.width,
                   (float) -st->subscreen.texture.height},
      (Rectangle) {rulesRect->x + 50.0f,
                   rulesRect->y + rulesRect->height / 4,
                   rulesRect->width - 100.0f,
                   (rulesRect->width - 100.0f) * st->subscreen.texture.height
                   / st->subscreen.texture.width},
      (Vector2) {0.0f, 0.0f},
      0.0f,
      WHITE
      );
}

static void display_rules(game_info_t *game, int left_padding, int offset,
    int font_size, menu_content_t *menu, state_t *st) {
  float rectWidth = game->screen_w - left_padding - offset;
  float rectHeight = game->screen_h - 2 * offset;
  Rectangle rulesRect = {
    left_padding,
    (game->screen_h - rectHeight) / 2,
    rectWidth,
    rectHeight
  };
  DrawRectangleRec(rulesRect, (Color) { 50, 50, 50, 200 });
  DrawRectangleLinesEx(rulesRect, 2, WHITE);
  //
  {
    Rectangle pink = {
      left_padding, (game->screen_h - rectHeight) / 2,
      MeasureText("Règle", font_size) + 2 * offset, font_size + offset / 4
    };
    DrawRectangleRec(pink, PINK);
    DrawText("Règle",
        pink.x + pink.width / 2 - MeasureText("Règle", font_size) / 2,
        rulesRect.y + pink.height / 2 - font_size / 2, font_size, WHITE);
    //
    float triWidth = rulesRect.width * 0.15f;
    float triHeight = rulesRect.height * 0.15f;
    Vector2 vertex1 = {
      rulesRect.x + rulesRect.width,
      rulesRect.y + rulesRect.height - triHeight
    };
    Vector2 vertex2 = {
      rulesRect.x + rulesRect.width - triWidth,
      rulesRect.y + rulesRect.height
    };
    Vector2 vertex3 = {
      rulesRect.x + rulesRect.width,
      rulesRect.y + rulesRect.height
    };
    DrawTriangle(vertex1, vertex2, vertex3, BLUE);
    char s[4];
    snprintf(s, 4, "%d/%d", menu->content.rules_values.rules_num + 1,
        MAX_RULES + 1);
    int tri_fsize = (int) (triHeight * 0.25f);
    float center_x = (vertex1.x + vertex2.x + vertex3.x) / 3.0f;
    float center_y = (vertex1.y + vertex2.y + vertex3.y) / 3.0f;
    DrawText(s, center_x - MeasureText(s, tri_fsize) / 2,
        center_y - tri_fsize / 3,
        tri_fsize, WHITE);
  }
  //
  display_rule_bottom_btn(game, menu, &rulesRect);
  switch (menu->content.rules_values.rules_num) {
    case 0:
      if (display_text_writing(game, menu, &rulesRect, RULE_ZERO)) {
        float t = menu->content.rules_values.rules_frames / 180.0f;
        if (t > 1.0f) {
          t = 1.0f;
          // menu->content.rules_values.rules_num = 1;
        }
        display_img_player(menu, &rulesRect, t);
      }
      break;
    case 1:
      if (display_text_writing(game, menu, &rulesRect, RULE_ONE)) {
        display_rule_frame_1(menu, st, &rulesRect);
      }
      break;
    case 2:
      if (display_text_writing(game, menu, &rulesRect, RULE_TWO)) {
        display_rule_frame_2(menu, st, &rulesRect);
      }
      break;
    case 3:
      if (display_text_writing(game, menu, &rulesRect, RULE_THREE)) {
        display_rule_frame_3(menu, st, &rulesRect);
      }
      break;
    case 4:
      if (display_text_writing(game, menu, &rulesRect, RULE_FOUR)) {
        // remettre les deux joueurs dès le début et montrer que le j1
        // donne une pièce au j2 par une animation
      }
      break;
    case 5:
      if (display_text_writing(game, menu, &rulesRect, RULE_FIVE)) {
        // montrer le joueur 1 qui place la pièce sur le plateau et après qu'il
        //    donne une pièce au joueur 2
      }
      break;
    case 6:
      if (display_text_writing(game, menu, &rulesRect, RULE_SIX)) {
        display_rule_frame_6(menu, st, &rulesRect);
      }
      break;
    default:
      break;
  }
}

static void display_menu_button(int titleWidth, game_info_t *game,
    int button_f, int button_w, menu_content_t *menu) {
  float fontSize = 55.0f;
  float spacing = 1.0f;
  Font font = GetFontDefault();
  const char *btn_title[4] = {
    "Play", "Rules", "Histoire", "Quit"
  };
  btn_t btn_type[4] = {
    PLAY, RULES, HISTORY, QUIT
  };
  for (size_t i = 0; i < 4; ++i) {
    Vector2 text_size = MeasureTextEx(font, btn_title[i], fontSize, spacing);
    Rectangle rect = {
      titleWidth + titleWidth / TITLE_POS_DIV - button_w,
      game->screen_h / 2 + (i + 1) * game->screen_h / SPACE_BETWEEN_BUTTONS,
      button_w,
      button_f
    };
    bool is_hover = CheckCollisionPointRec(GetMousePosition(), rect);
    Color text_color = WHITE;
    if (is_hover) {
      if (menu->sound_play != btn_type[i] && !IsSoundPlaying(menu->sound)) {
        PlaySound(menu->sound);
        menu->sound_play = btn_type[i];
      }
      text_color = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? GRAY : MAROON;
      if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (btn_type[i] == PLAY) {
          menu->currentScreen = GAME;
          menu->menuType = NONE;
        } else if (btn_type[i] == RULES) {
          menu->menuType = RULES;
          menu->content.rules_values.rules_num = 0;
          menu->content.rules_values.rules_frames = 0;
          menu->content.rules_values.rules_textures = load_rules_ressources();
        } else if (btn_type[i] == HISTORY) {
          menu->menuType = HISTORY;
          menu->content.history_values.history_frames = 0;
          menu->content.history_values.history_texture
            = LoadTexture("resources/image/histoire.png");
        } else if (btn_type[i] == QUIT) {
          game->exit_wind = true;
        }
        menu->sound_play = NONE;
      }
    }
    float x = rect.x + (rect.width - text_size.x);
    float y = rect.y + (rect.height - text_size.y);
    DrawTextEx(font, btn_title[i], (Vector2){ x, y }, fontSize, spacing,
        text_color);
  }
}

static void display_history(game_info_t *game, int left_padding, int offset,
    int font_size, menu_content_t *menu) {
  if (IsKeyPressed(KEY_SPACE)) {
    menu->content.history_values.history_frames += 60;
  }
  float rectWidth = game->screen_w - left_padding - offset;
  float rectHeight = game->screen_h - 2 * offset;
  Rectangle rulesRect = {
    left_padding,
    (game->screen_h - rectHeight) / 2,
    rectWidth,
    rectHeight
  };
  DrawRectangleRec(rulesRect, (Color) { 50, 50, 50, 200 });
  DrawRectangleLinesEx(rulesRect, 2, WHITE);
  //
  const char *t = "Histoire";
  const int title_size = MeasureText("Histoire", font_size);
  Rectangle pink = {
    left_padding, (game->screen_h - rectHeight) / 2,
    title_size + 2 * offset, font_size + offset / 4
  };
  DrawRectangleRec(pink, BROWN);
  DrawText("Histoire",
      pink.x + pink.width / 2 - title_size / 2,
      rulesRect.y + pink.height / 2 - font_size / 2, font_size, WHITE);
  //
  int img_width = menu->content.history_values.history_texture.width;
  int img_height = menu->content.history_values.history_texture.height;
  float img_x = rulesRect.x + (rulesRect.width - img_width) / 2.0f;
  float img_y = pink.y + pink.height + TEXT_PADDING;
  DrawTexture(menu->content.history_values.history_texture, (int) img_x,
      (int) img_y, WHITE);
  //
  int font_size_txt = rulesRect.height / 30;
  int l = MeasureText(HISTORY_TXT, font_size_txt);
  int number_to_display = menu->content.history_values.history_frames / 3;
  DrawText(TextSubtext(HISTORY_TXT, 0, number_to_display),
      pink.x + (rulesRect.width - l) / 2,
      img_y + img_height + 30, font_size_txt, LIGHTGRAY);
}

void display_menu(game_info_t *game, menu_content_t *menu, state_t *st) {
  const int title_size = game->screen_h / TITLE_DIVIDER;
  int titleWidth = MeasureText(game->game_name, title_size);
  DrawText(game->game_name, titleWidth / TITLE_POS_DIV,
      titleWidth / TITLE_POS_DIV,
      title_size, WHITE);
  int button_f = game->screen_h / (TITLE_DIVIDER * 2.2);
  //
  GuiSetStyle(DEFAULT, TEXT_SIZE, button_f);
  int button_w = MeasureText("Histoire", button_f);
  display_menu_button(titleWidth, game, button_f, button_w, menu);
  switch (menu->menuType) {
    case RULES:
      if (IsKeyPressed(KEY_ESCAPE)) {
        menu->menuType = NONE;
      }
      display_rules(game, 2 * titleWidth / TITLE_POS_DIV + titleWidth,
          titleWidth / TITLE_POS_DIV, button_f * 0.9f, menu, st);
      break;
    case HISTORY:
      if (IsKeyPressed(KEY_ESCAPE)) {
        menu->menuType = NONE;
      }
      display_history(game, 2 * titleWidth / TITLE_POS_DIV + titleWidth,
          titleWidth / TITLE_POS_DIV, button_f * 0.9f, menu);
      break;
  }
  //
  Rectangle buttonBounds = {
    game->screen_w - 50, game->screen_h - 40, 40, 30
  };
  if (GuiButton(buttonBounds, (game->play_music ? "#132#" : "#131#"))) {
    game->play_music = !game->play_music;
  }
}

bool display_exit_menu(game_info_t *game_info) {
  int rect_w = game_info->screen_w * 0.5f;
  int rect_h = game_info->screen_h * 0.5f;
  DrawRectangle(0, 0, game_info->screen_w, game_info->screen_h,
      Fade(BLACK, 0.5f));
  DrawRectangle((game_info->screen_w - rect_w) / 2,
      (game_info->screen_h - rect_h) / 2, rect_w, rect_h, BLACK);
  DrawRectangle((game_info->screen_w - rect_w) / 2,
      (game_info->screen_h - rect_h) / 2, rect_w, rect_h / 5, RED);
  int fontSize = rect_h / 8;
  int textWidth = MeasureText("Quit", fontSize);
  int rectX = (game_info->screen_w - rect_w) / 2;
  int rectY = (game_info->screen_h - rect_h) / 2;
  int textX = rectX + (rect_w - textWidth) / 2;
  int textY = rectY + ((rect_h / 5) - fontSize) / 2;
  DrawText("Quit", textX, textY, fontSize, WHITE);
  int messageFontSize = rect_h / 11;
  int messageTextWidth
    = MeasureText("do you really want to quit the game ?", messageFontSize);
  int messageX = rectX + (rect_w - messageTextWidth) / 2;
  int messageY = rectY + (rect_h - messageFontSize) / 2.5f;
  DrawText(
      "Do you really want to quit the game ?\nYou will miss a lot of fun...",
      messageX, messageY,
      messageFontSize, WHITE);
  int buttonHeight = rect_h / 10;
  int spacing = rect_w / 20;  // espace réduit entre les boutons
  int buttonWidth = rect_w / 4;
  int totalButtonsWidth = 2 * buttonWidth + spacing;
  int buttonX_no = rectX + (rect_w - totalButtonsWidth) / 2;
  int buttonX_yes = buttonX_no + buttonWidth + spacing;
  int buttonY = (game_info->screen_h - rect_h) / 2 + rect_h - buttonHeight
      - 70;
  if (GuiButton((Rectangle){ buttonX_no, buttonY, buttonWidth,
                             buttonHeight + 20 }, "No")) {
    game_info->exit_wind = false;
    return false;
  }
  if (GuiButton((Rectangle){ buttonX_yes, buttonY, buttonWidth,
                             buttonHeight + 20 }, "Yes")) {
    return true;
  }
  return false;
}
