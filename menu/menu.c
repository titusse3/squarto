#include "menu.h"

#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "rlights.h"
#include "rules_menu.h"
#include "utils_menu.h"

#define SPLAY "Play"
#define SRULES "Rules"
#define SHISTORY "History"
#define SDIFFICULTY "Select difficulty"
#define SQUIT "Quit"

// DIFFICULTY DESCRIPTIONS
#define S_EASY_DESC "L'alignement provoque la victoire."
#define S_MEDIUM_DESC "L'alignement ou le petit carré provoque la victoire."
#define S_HARD_DESC \
        "Les même conditions que précédament avec en plus les grands carrés."
#define S_VERY_HARD_DESC \
        "Toutes les conditions précédente en plus du carrés tournant."

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

static void display_menu_button(int titleWidth, game_info_t *game,
    int button_f, int button_w, menu_content_t *menu, state_t *st) {
  float fontSize = game->screen_h / (TITLE_DIVIDER * 1.7f);
  float spacing = 1.0f;
  Font font = GetFontDefault();
  const char *btn_title[] = {
    SPLAY, SRULES, SHISTORY, SQUIT
  };
  btn_t btn_type[] = {
    PLAY, RULES, HISTORY, QUIT
  };
  bool is_mouse_over = false;
  for (size_t i = 0; i < 4; ++i) {
    Vector2 text_size = MeasureTextEx(font, btn_title[i], fontSize, spacing);
    Rectangle rect = {
      titleWidth + titleWidth / TITLE_POS_DIV - button_w,
      game->screen_h / 2 + i * game->screen_h / SPACE_BETWEEN_BUTTONS,
      button_w, button_f
    };
    Color text_color = WHITE;
    if (CheckCollisionPointRec(GetMousePosition(), rect)) {
      is_mouse_over = true;
      if (menu->sound_play != btn_type[i] && !IsSoundPlaying(menu->sound)) {
        PlaySound(menu->sound);
        menu->sound_play = btn_type[i];
      }
      text_color = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? GRAY : (Color) {
        196, 82, 204, 255
      };
      if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (btn_type[i] == PLAY) {
          // menu->currentScreen = GAME;
          menu->menuType = CHOOSE_DIFFICULTY;
          if (!st->mk_screen) {
            free(st->screens);
            st->mk_screen = true;
          }
        } else if (btn_type[i] == RULES) {
          menu->menuType = RULES;
          menu->content.rules_values.rules_num = 0;
          menu->content.rules_values.rules_frames = 0;
          menu->content.rules_values.rules_textures = load_rules_ressources();
          if (!st->mk_screen) {
            free(st->screens);
            st->mk_screen = true;
          }
        } else if (btn_type[i] == HISTORY) {
          menu->menuType = HISTORY;
          menu->content.history_values.history_frames = 0;
          menu->content.history_values.history_texture
            = LoadTexture("resources/image/histoire.png");
          if (!st->mk_screen) {
            free(st->screens);
            st->mk_screen = true;
          }
        } else if (btn_type[i] == QUIT) {
          game->exit_wind = true;
        }
        menu->sound_play = NONE;
      }
    }
    float x = rect.x + (rect.width - text_size.x);
    float y = rect.y + (rect.height - text_size.y);
    if (menu->menuType == RULES && btn_type[i] == RULES
        || menu->menuType == HISTORY && btn_type[i] == HISTORY
        || menu->menuType == CHOOSE_DIFFICULTY && btn_type[i] == PLAY
        || game->exit_wind && btn_type[i] == QUIT) {
      text_color = (Color) {
        104, 58, 107, 255
      };
    }
    DrawTextEx(font, btn_title[i], (Vector2){ x, y }, fontSize, spacing,
        text_color);
  }
  if (!is_mouse_over) {
    menu->sound_play = NONE;
  }
}

static void display_history(game_info_t *game, int left_padding, int offset,
    int font_size, menu_content_t *menu) {
  if (IsKeyPressed(KEY_SPACE)) {
    menu->content.history_values.history_frames += 60;
  }
  DISPLAY_BCK_WINDOW(game, left_padding, offset);
  //
  Rectangle pink = rect_top_corner_title(SHISTORY, rulesRect, font_size,
      PURPLE);
  //
  int img_width = menu->content.history_values.history_texture.width;
  int img_height = menu->content.history_values.history_texture.height;
  float img_x = rulesRect.x + (rulesRect.width - img_width) / 2.0f;
  float img_y = pink.y + pink.height + TEXT_PADDING_MENU;
  DrawTexture(menu->content.history_values.history_texture, (int) img_x,
      (int) img_y, WHITE);
  //
  int font_size_txt = rulesRect.width / 40;
  int l = MeasureText(HISTORY_TXT, font_size_txt);
  int number_to_display = menu->content.history_values.history_frames / 3;
  int text_y = img_y + img_height + 30;
  int text_height = MeasureTextEx(GetFontDefault(), HISTORY_TXT, font_size_txt,
      1).y;
  int centered_y = text_y + (rulesRect.height - text_y - text_height) / 2;
  DrawText(TextSubtext(HISTORY_TXT, 0, number_to_display),
      pink.x + (rulesRect.width - l) / 2,
      centered_y, font_size_txt, LIGHTGRAY);
}

static void display_choosing_difficulty(game_info_t *game, int left_padding,
    int offset, int font_size, menu_content_t *menu) {
  DISPLAY_BCK_WINDOW(game, left_padding, offset);
  // DISPLAY_BCK_WINDOW donne accés à la varaible rulesRect
  Rectangle purple = rect_top_corner_title(SDIFFICULTY, rulesRect, font_size,
      PURPLE);
  const char *difficulty_options[] = {
    S_EASY, S_MEDIUM, S_HARD, S_VERY_HARD
  };
  const char *difficulty_descriptions[] = {
    S_EASY_DESC, S_MEDIUM_DESC, S_HARD_DESC, S_VERY_HARD_DESC
  };
  int num_options = 4;
  int button_height = rulesRect.height / (num_options * 1.5); // Increased
                                                              // height
  int button_width = rulesRect.width * 0.9f;
  int spacing = rulesRect.height / (num_options * 5);
  int total_height = num_options * button_height + (num_options - 1) * spacing;
  int start_y = rulesRect.y + (rulesRect.height - total_height) / 2;
  for (int i = 0; i < num_options; i++) {
    Rectangle button_rect = {
      rulesRect.x + (rulesRect.width - button_width) / 2,
      start_y + i * (button_height + spacing),
      button_width,
      button_height
    };
    bool is_mouse_over = CheckCollisionPointRec(GetMousePosition(),
        button_rect);
    Color button_color = is_mouse_over ? DARKPURPLE : BLACK;
    Color text_color = PURPLE; // Same color as the rectangle borders
    DrawRectangleRec(button_rect, button_color);
    DrawRectangleLinesEx(button_rect, 2, PURPLE);
    int title_font_size = font_size * 1.1f;
    Vector2 title_size = MeasureTextEx(GetFontDefault(),
        difficulty_options[i], title_font_size, 1);
    DrawTextEx(GetFontDefault(), difficulty_options[i],
        (Vector2){ button_rect.x + (button_rect.width - title_size.x) / 2,
                   button_rect.y + button_height / 4 - title_size.y / 2 },
        title_font_size, 1,
        (is_mouse_over ? WHITE : text_color));
    int desc_font_size = font_size * 0.5f;
    Vector2 desc_size = MeasureTextEx(GetFontDefault(),
        difficulty_descriptions[i], desc_font_size, 1);
    DrawTextEx(GetFontDefault(), difficulty_descriptions[i],
        (Vector2){ button_rect.x + (button_rect.width - desc_size.x) / 2,
                   button_rect.y + (button_height * 3 / 4) - desc_size.y / 2 },
        desc_font_size, 1, WHITE); // Centered description
    if (is_mouse_over && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
      menu->content.game_values.dropwdonw_open = false;
      menu->content.game_values.solver = 0;
      menu->content.game_values.frames = 0;
      menu->content.game_values.stage = 0;
      menu->content.game_values.difficulty = i;
      menu->menuType = NONE;
      menu->currentScreen = GAME;
    }
  }
}

void display_menu(game_info_t *game, menu_content_t *menu, state_t *st) {
  const int title_size = game->screen_h / TITLE_DIVIDER * 1.2;
  int titleWidth = MeasureText(game->game_name, title_size);
  Vector2 titlePos = {
    game->screen_w / 12,
    game->screen_h / 11
  };
  DrawTextEx(menu->anim_font, game->game_name,
      (Vector2){ titlePos.x + 4, titlePos.y + 4 }, title_size, 2, GRAY);
  DrawTextEx(menu->anim_font, game->game_name, titlePos, title_size, 2, WHITE);
  int button_f = game->screen_h / (TITLE_DIVIDER * 2.2);
  //
  GuiSetStyle(DEFAULT, TEXT_SIZE, button_f);
  int button_w = MeasureText(SHISTORY, button_f);
  display_menu_button(titleWidth, game, button_f, button_w, menu, st);
  if (IsKeyPressed(KEY_ESCAPE)) {
    menu->menuType = NONE;
  }
  switch (menu->menuType) {
    case CHOOSE_DIFFICULTY:
      display_choosing_difficulty(game,
          2 * titleWidth / TITLE_POS_DIV + titleWidth,
          titleWidth / TITLE_POS_DIV, button_f * 0.9f, menu);
      break;
    case RULES:
      display_rules(game, 2 * titleWidth / TITLE_POS_DIV + titleWidth,
          titleWidth / TITLE_POS_DIV, button_f * 0.9f, menu, st);
      break;
    case HISTORY:
      display_history(game, 2 * titleWidth / TITLE_POS_DIV + titleWidth,
          titleWidth / TITLE_POS_DIV, button_f * 0.9f, menu);
      break;
    default:
      break;
  }
  //
  float button_size = game->screen_h * 0.05f; // Make the button square
  Rectangle buttonBounds = {
    game->screen_w - button_size - button_size / 2,
    game->screen_h - button_size - button_size / 2,
    button_size, button_size
  };
  if (GuiButton(buttonBounds, (game->play_music ? "#132#" : "#131#"))) {
    game->play_music = !game->play_music;
  }
}

bool display_exit_menu(game_info_t *game_info, int fontSize, const char *msg,
    int msg_size) {
  int rect_w = game_info->screen_w * 0.5f;
  int rect_h = game_info->screen_h * 0.5f;
  int rectX = (game_info->screen_w - rect_w) / 2;
  int rectY = (game_info->screen_h - rect_h) / 2;
  DrawRectangle(0, 0, game_info->screen_w, game_info->screen_h,
      Fade(BLACK, 0.8f));
  DrawRectangle(rectX, rectY, rect_w, rect_h, BLACK);
  DrawRectangle(rectX, rectY, rect_w, rect_h / 5, BLACK);
  int quit_font_size = game_info->screen_h / 16;
  int textWidth = MeasureText(SQUIT, quit_font_size);
  DrawText(SQUIT, rectX + (rect_w - textWidth) / 2,
      rectY + ((rect_h / 5) - quit_font_size) / 2, quit_font_size, PURPLE);
  int messageX = rectX + (rect_w - msg_size) / 2;
  int messageY = rectY + (rect_h - fontSize) / 2.5f;
  DrawText(msg, messageX, messageY, fontSize, WHITE);
  DrawRectangleLinesEx((Rectangle){ rectX, rectY, rect_w, rect_h }, 3, PURPLE);
  int buttonHeight = rect_h / 10;
  int spacing = rect_w / 20;
  int buttonWidth = rect_w / 4;
  int totalButtonsWidth = 2 * buttonWidth + spacing;
  int buttonX_no = rectX + (rect_w - totalButtonsWidth) / 2;
  int buttonX_yes = buttonX_no + buttonWidth + spacing;
  int buttonY = rectY + rect_h - buttonHeight - 70;
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

void display_animation(game_info_t *game_info, animation_t *info,
    Rectangle destRec) {
  info->has_start = true;
  Vector2 origin = {
    0, 0
  };
  DrawTexturePro(info->img, info->frameRec, destRec, origin, 0.0f, WHITE);
}

void display_end_animation(game_info_t *game_info, animation_t *info, Font f,
    player_t p) {
  Rectangle destRec = {
    (game_info->screen_w - info->frameRec.width
    * (game_info->screen_w / (info->frameRec.width * 1.4f))) / 2,
    (game_info->screen_h - info->frameRec.height
    * (game_info->screen_w / (info->frameRec.height * 1.3f))) / 2,
    info->frameRec.width
    * (game_info->screen_w / (info->frameRec.width * 1.4f)),
    info->frameRec.height
    * (game_info->screen_w / (info->frameRec.width * 1.4f))
  };
  display_animation(game_info, info, destRec);
  //
  int font_size = f.baseSize * 8;
  static float waveOffset = 0.0f;
  waveOffset += 0.1f;
  const char *text[] = {
    "You w in !", "You Loose !", "It's a d raw !"
  };
  Color cs[] = {
    GREEN, RED, ORANGE
  };
  Vector2 textSize = MeasureTextEx(f, text[p], font_size, 1);
  for (int i = 0; i < strlen(text[p]); i++) {
    float offset = sinf(waveOffset + i * 0.5f) * 10.0f;
    DrawTextPro(f, (char[]){ text[p][i], '\0' },
        (Vector2){ game_info->screen_w / 2 - textSize.x / 2 + i * (textSize.x
                   / strlen(text[p])),
                   game_info->screen_h / 2 - f.baseSize * 2.5f + offset },
        (Vector2){ 0, 0 }, 0.0f, font_size, 1, cs[p]);
  }
  //
}
