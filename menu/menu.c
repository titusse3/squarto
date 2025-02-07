#include "menu.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define TITLE_SIZE 150
#define BUTTON_SIZE 75
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50

#define MENU_PADDING 10

void display_menu(int screen_w, int screen_h, const char *game_title,
    GameScreen *currentScreen) {
  int titleWidth = MeasureText(game_title, TITLE_SIZE);
  DrawText(game_title, 100, 100,
      TITLE_SIZE, BLACK);
  GuiSetStyle(DEFAULT, TEXT_SIZE, BUTTON_SIZE);
  if (GuiButton((Rectangle){titleWidth,
                            (screen_h) / 3 + BUTTON_HEIGHT,
                            BUTTON_WIDTH, BUTTON_HEIGHT},
      "Jouer")) {
    *currentScreen = GAME;
  }
  if (GuiButton((Rectangle){ titleWidth,
                             (screen_h) / 3 + 2 * BUTTON_HEIGHT + MENU_PADDING,
                             BUTTON_WIDTH, BUTTON_HEIGHT},
      "Règles")) {
    // Action pour le bouton Settings
  }
  if (GuiButton((Rectangle){ titleWidth,
                             (screen_h) / 3 + 3 * BUTTON_HEIGHT + 2
                             * MENU_PADDING,
                             BUTTON_WIDTH, BUTTON_HEIGHT},
      "Histoire")) {
  }
}
