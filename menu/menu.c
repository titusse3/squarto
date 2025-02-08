#include "menu.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

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
        "Suisse Blaise Müller. Ce jeu se distingue par son concept original:\n" \
        "votre adversaire choisit la pièce que vous allez jouer parmi \n" \
        "16 pièces uniques, chacune possédant quatre attributs différents.\n" \
        "Malgré des règles simples, Quarto offre une profondeur tactique \n" \
        "remarquable et a rapidement séduit les amateurs de jeux de réflexion\n" \
        "à travers le monde."

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
  int l = MeasureText(text, 30);
  int number_to_display = menu->content.rules_values.rules_frames / 3;
  DrawText(TextSubtext(text, 0, number_to_display),
      container->x + (container->width - l) / 2,
      container->y + container->height / 5, 30, LIGHTGRAY);
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
  if (GuiButton(midButton, "#131#") || IsKeyPressed(KEY_SPACE)) {
    menu->content.rules_values.rules_frames += 60;
  }
  if (GuiButton(nextButton, "#131#") || IsKeyPressed(KEY_RIGHT)) {
    if (menu->content.rules_values.rules_num != MAX_RULES) {
      ++menu->content.rules_values.rules_num;
      menu->content.rules_values.rules_frames = 0;
    }
  }
}

static void display_rules(game_info_t *game, int left_padding, int offset,
    int font_size, menu_content_t *menu) {
  float rectWidth = game->screen_w - left_padding - offset;
  float rectHeight = game->screen_h - 2 * offset;
  Rectangle rulesRect = {
    left_padding,
    (game->screen_h - rectHeight) / 2,
    rectWidth,
    rectHeight
  };
  DrawRectangleRec(rulesRect, (Color) { 50, 50, 50, 200 });
  DrawRectangleLinesEx(rulesRect, 2, BLACK);
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
        //faire un plateau en 3d de 4x4 qui descend un peu
      }
      break;
    case 2:
      if (display_text_writing(game, menu, &rulesRect, RULE_TWO)) {
        // faire en 3d les 16 pièces
        // ajouter des 4 bouttons sur la même lignes qui permettent
        // de mettre en surbrillance les caractéristiques des pièces afficher
      }
      break;
    case 3:
      if (display_text_writing(game, menu, &rulesRect, RULE_THREE)) {
        // faire un exemple d'alignement en 3d
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
        // montrer un exemple d'alignement de 4 pièces puis une animation de
        // victoire
      }
      break;
    default:
      break;
  }
}

static void display_menu_button(int titleWidth, game_info_t *game,
    int button_f, int button_w, menu_content_t *menu) {
  if (GuiButton((Rectangle){titleWidth + titleWidth / TITLE_POS_DIV - button_w,
                            game->screen_h / 2, button_w, button_f}, "Jouer")) {
    menu->currentScreen = GAME;
    menu->menuType = NONE;
  }
  if (GuiButton((Rectangle){titleWidth + titleWidth / TITLE_POS_DIV - button_w,
                            game->screen_h / 2 + game->screen_h
                            / SPACE_BETWEEN_BUTTONS,
                            button_w, button_f}, "Règles")) {
    menu->menuType = RULES;
    menu->content.rules_values.rules_num = 0;
    menu->content.rules_values.rules_frames = 0;
    menu->content.rules_values.rules_textures = load_rules_ressources();
  }
  if (GuiButton((Rectangle){titleWidth + titleWidth / TITLE_POS_DIV - button_w,
                            game->screen_h / 2 + 2 * game->screen_h
                            / SPACE_BETWEEN_BUTTONS, button_w, button_f},
      "Histoire")) {
    menu->menuType = HISTORY;
    menu->content.history_values.history_frames = 0;
    menu->content.history_values.history_texture
      = LoadTexture("resources/image/histoire.png");
  }
}

static void display_history(game_info_t *game, int left_padding, int offset,
    int font_size, menu_content_t *menu) {
  float rectWidth = game->screen_w - left_padding - offset;
  float rectHeight = game->screen_h - 2 * offset;
  Rectangle rulesRect = {
    left_padding,
    (game->screen_h - rectHeight) / 2,
    rectWidth,
    rectHeight
  };
  DrawRectangleRec(rulesRect, (Color) { 50, 50, 50, 200 });
  DrawRectangleLinesEx(rulesRect, 2, BLACK);
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
  int l = MeasureText(HISTORY_TXT, 20);
  int number_to_display = menu->content.history_values.history_frames / 3;
  DrawText(TextSubtext(HISTORY_TXT, 0, number_to_display),
      pink.x + (rulesRect.width - l) / 2,
      img_y + img_height + 30, 20, LIGHTGRAY);
}

void display_menu(game_info_t *game, menu_content_t *menu) {
  const int title_size = game->screen_h / TITLE_DIVIDER;
  int titleWidth = MeasureText(game->game_name, title_size);
  DrawText(game->game_name, titleWidth / TITLE_POS_DIV,
      titleWidth / TITLE_POS_DIV,
      title_size, BLACK);
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
          titleWidth / TITLE_POS_DIV, button_f * 0.9f, menu);
      break;
    case HISTORY:
      if (IsKeyPressed(KEY_ESCAPE)) {
        menu->menuType = NONE;
      }
      display_history(game, 2 * titleWidth / TITLE_POS_DIV + titleWidth,
          titleWidth / TITLE_POS_DIV, button_f * 0.9f, menu);
      break;
  }
}
