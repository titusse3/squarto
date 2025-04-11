#include "menu.h"

#include <raymath.h>

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "rlights.h"
#include "rules_menu.h"
#include "utils_menu.h"

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
  float fontSize = 55.0f;
  float spacing = 1.0f;
  Font font = GetFontDefault();
  const char *btn_title[4] = {
    "Play", "Rules", "Histoire", "Quit"
  };
  btn_t btn_type[4] = {
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
      text_color = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? GRAY : MAROON;
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
  float rectWidth = game->screen_w - left_padding - offset;
  float rectHeight = game->screen_h - 2 * offset;
  Rectangle rulesRect = {
    left_padding, (game->screen_h - rectHeight) / 2, rectWidth, rectHeight
  };
  DrawRectangleRec(rulesRect, (Color) { 50, 50, 50, 200 });
  DrawRectangleLinesEx(rulesRect, 2, WHITE);
  //
  Rectangle pink = rect_top_corner_title("Histoire", rulesRect, font_size,
      BROWN);
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

void display_menu(game_info_t *game, menu_content_t *menu, state_t *st) {
  const int title_size = game->screen_h / TITLE_DIVIDER;
  int titleWidth = MeasureText(game->game_name, title_size);
  Vector2 titlePos = {
    titleWidth / TITLE_POS_DIV, titleWidth / TITLE_POS_DIV
  };
  DrawTextEx(menu->win_info.f, game->game_name, titlePos, title_size, 2, WHITE);
  int button_f = game->screen_h / (TITLE_DIVIDER * 2.2);
  //
  GuiSetStyle(DEFAULT, TEXT_SIZE, button_f);
  int button_w = MeasureText("Histoire", button_f);
  display_menu_button(titleWidth, game, button_f, button_w, menu, st);
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

bool display_exit_menu(game_info_t *game_info, int fontSize, const char *msg,
    int msg_size) {
  int rect_w = game_info->screen_w * 0.5f;
  int rect_h = game_info->screen_h * 0.5f;
  int rectX = (game_info->screen_w - rect_w) / 2;
  int rectY = (game_info->screen_h - rect_h) / 2;
  DrawRectangle(0, 0, game_info->screen_w, game_info->screen_h,
      Fade(BLACK, 0.5f));
  DrawRectangle(rectX, rectY, rect_w, rect_h, BLACK);
  DrawRectangle(rectX, rectY, rect_w, rect_h / 5, RED);
  int quit_font_size = game_info->screen_h / 16;
  int textWidth = MeasureText("Quit", quit_font_size);
  DrawText("Quit", rectX + (rect_w - textWidth) / 2,
      rectY + ((rect_h / 5) - quit_font_size) / 2, quit_font_size, WHITE);
  int messageX = rectX + (rect_w - msg_size) / 2;
  int messageY = rectY + (rect_h - fontSize) / 2.5f;
  DrawText(
      msg,
      messageX, messageY, fontSize, WHITE);
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

void display_end_animation(game_info_t *game_info, win_display_info *info,
    const char *text, bool win) {
  info->has_end = true;
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
  Vector2 origin = {
    0, 0
  };
  DrawTexturePro(info->explosion, info->frameRec, destRec, origin, 0.0f, WHITE);
  //
  int font_size = info->f.baseSize * 8;
  static float waveOffset = 0.0f;
  waveOffset += 0.1f;
  Vector2 textSize = MeasureTextEx(info->f, text, font_size, 1);
  for (int i = 0; i < strlen(text); i++) {
    float offset = sinf(waveOffset + i * 0.5f) * 10.0f;
    DrawTextPro(info->f, (char[]){ text[i], '\0' },
        (Vector2){ game_info->screen_w / 2 - textSize.x / 2 + i * (textSize.x
                   / strlen(text)),
                   game_info->screen_h / 2 - info->f.baseSize * 2.5f + offset },
        (Vector2){ 0, 0 }, 0.0f, font_size, 1, (win ? GREEN : RED));
  }
  //
}
