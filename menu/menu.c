#include "menu.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#define MENU_PADDING 10

#define TITLE_DIVIDER 8
#define TITLE_POS_DIV 6

#define TEXT_PADDING 20
#define SPACE_BETWEEN_BUTTONS 11

void display_rules(int screen_w, int screen_h, int left_padding, int offset,
    int font_size, float *scrollOffset);

void display_menu(int screen_w, int screen_h, const char *game_title,
    GameScreen *currentScreen) {
  const int title_size = screen_h / TITLE_DIVIDER;
  int titleWidth = MeasureText(game_title, title_size);
  DrawText(game_title, titleWidth / TITLE_POS_DIV, titleWidth / TITLE_POS_DIV,
      title_size, BLACK);
  //
  int button_f = screen_h / (TITLE_DIVIDER * 2);
  GuiSetStyle(DEFAULT, TEXT_SIZE, button_f);
  int button_w = MeasureText("Histoire", button_f);
  if (GuiButton((Rectangle){titleWidth + titleWidth / TITLE_POS_DIV - button_w,
                            screen_h / 2, button_w, button_f},
      "Jouer")) {
    *currentScreen = GAME;
  }
  if (GuiButton((Rectangle){titleWidth + titleWidth / TITLE_POS_DIV - button_w,
                            screen_h / 2 + screen_h / SPACE_BETWEEN_BUTTONS,
                            button_w, button_f},
      "Règles")) {
    *currentScreen = RULES;
  }
  if (GuiButton((Rectangle){titleWidth + titleWidth / TITLE_POS_DIV - button_w,
                            screen_h / 2 + 2 * screen_h / SPACE_BETWEEN_BUTTONS,
                            button_w,
                            button_f},
      "Histoire")) {
    *currentScreen = HISTORY;
  }
  if (*currentScreen == RULES) {
    float scrollOffset = 0;
    display_rules(screen_w, screen_h,
        2 * titleWidth / TITLE_POS_DIV + titleWidth,
        titleWidth / TITLE_POS_DIV, button_f, &scrollOffset);
  }
}

// void display_rules(int screen_w, int screen_h, int left_padding, int offset,
//     int font_size) {
//   float rectWidth = screen_w - left_padding - offset;
//   float rectHeight = screen_h - 2 * offset;
//   Rectangle rulesRect = {
//     left_padding,
//     (screen_h - rectHeight) / 2,
//     rectWidth,
//     rectHeight
//   };
//   Color bgColor = (Color){
//     50, 50, 50, 200
//   };
//   DrawRectangleRec(rulesRect, bgColor);
//   DrawRectangleLinesEx(rulesRect, 2, BLACK);
//   //
//   Rectangle pink = {
//     left_padding, (screen_h - rectHeight) / 2,
//     MeasureText("Règle", font_size) + 2 * offset, font_size + offset / 4
//   };
//   DrawRectangleRec(pink, PINK);
//   DrawText("Règle",
//       pink.x + pink.width / 2 - MeasureText("Règle", font_size) / 2,
//       rulesRect.y + pink.height / 2 - font_size / 2, font_size, WHITE);
// }

// Cette fonction affiche le rectangle avec un contenu défilant
// On lui passe un pointeur sur scrollOffset pour qu'il puisse être modifié
void display_rules(int screen_w, int screen_h, int left_padding, int offset,
    int font_size, float *scrollOffset) {
  // Dimensions du rectangle noir (la zone de scroll)
  float rectWidth = screen_w - left_padding - offset;
  float rectHeight = screen_h - 2 * offset;
  Rectangle rulesRect = {
    left_padding,
    (screen_h - rectHeight) / 2,
    rectWidth,
    rectHeight
  };
  // Dessiner le rectangle de fond noir semi-transparent
  Color bgColor = (Color){
    50, 50, 50, 200
  };
  DrawRectangleRec(rulesRect, bgColor);
  DrawRectangleLinesEx(rulesRect, 2, BLACK);
  // Gestion du scroll : on modifie scrollOffset si la souris est sur le
  // rectangle
  if (CheckCollisionPointRec(GetMousePosition(), rulesRect)) {
    // La molette retourne un incrément/décrément
    float wheel = GetMouseWheelMove();
    // Multipliez par une constante pour ajuster la vitesse du scroll
    *scrollOffset += wheel * 20;
    // Optionnel : si le contenu a une hauteur fixe, vous pouvez limiter le
    // scroll
    // Exemple : pour un contenu de 20 lignes dont la hauteur totale est
    // contentHeight
    int lineSpacing = font_size + 5;
    int numLines = 20;
    int contentHeight = 20 + numLines * lineSpacing;
    // Le scroll sera négatif lorsque le contenu défile vers le haut,
    // et 0 correspond à la position de départ (haut du contenu visible)
    if (*scrollOffset > 0) {
      *scrollOffset = 0;
    }
    if (*scrollOffset < rectHeight - contentHeight) {
      *scrollOffset = rectHeight - contentHeight;
    }
  }
  // Définir une zone de découpe (scissor) pour que le dessin soit limité à
  // rulesRect
  BeginScissorMode((int) rulesRect.x, (int) rulesRect.y, (int) rulesRect.width,
      (int) rulesRect.height);
  // Dessiner le contenu défilant dans le rectangle
  // Ici, nous affichons 20 lignes de texte par exemple
  int lineSpacing = font_size + 5;
  int numLines = 30;
  for (int i = 0; i < numLines; i++) {
    char line[256];
    sprintf(line, "Ligne %d : Ceci est un exemple de texte qui défile.", i + 1);
    DrawText(line,
        rulesRect.x + 10,
        rulesRect.y + 10 + i * lineSpacing + *scrollOffset,
        font_size,
        WHITE);
  }
  EndScissorMode();
  // Dessiner le bandeau rose (par exemple, un en-tête fixe)
  Rectangle pink = {
    left_padding,
    (screen_h - rectHeight) / 2,
    MeasureText("Règle", font_size) + 2 * offset,
    font_size + offset / 4
  };
  DrawRectangleRec(pink, PINK);
  DrawText("Règle",
      pink.x + pink.width / 2 - MeasureText("Règle", font_size) / 2,
      pink.y + pink.height / 2 - font_size / 2,
      font_size,
      WHITE);
}
