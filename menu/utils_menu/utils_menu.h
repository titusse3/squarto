#ifndef UTILS_MENU_H
#define UTILS_MENU_H

#include <stdlib.h>
#include <raylib.h>

#include "menu.h"

#define ALLOCATION_MSG "Error during allocation"
#define SOLVER_ERROR "Solver error"

#define ERROR_DISPLAY(game, msg)                                               \
        display_exit_menu(game, (game)->screen_h / 16, (msg),                  \
    MeasureText((msg), (game)->screen_h / 16));                                \
        return;

// DISPLAY_BCK_WINDOW: Macro permettant d'afficher une fenêtre de fond pour un
//    menu.
#define DISPLAY_BCK_WINDOW(game, left_padding, offset)                         \
        float rectWidth = (game)->screen_w - left_padding - offset;            \
        float rectHeight = (game)->screen_h - 2 * offset;                      \
        Rectangle rulesRect = {                                                \
          left_padding, ((game)->screen_h - rectHeight) / 2, rectWidth,        \
          rectHeight                                                           \
        };                                                                     \
        DrawRectangleRec(rulesRect, (Color) { 50, 50, 50, 200 });              \
        DrawRectangleLinesEx(rulesRect, 2, WHITE);                             \

// display_img_player: Fonction prenant en charge l'affichage d'image.
extern void display_img_player(menu_content_t *menu, Rectangle *container,
    float t);

// rect_top_corner_title: Fonction prenant en charge l'affichage d'un rectangle
//    dans le coin supérieur gauche de la fenêtre du menu.
//    Elle affiche également le titre du menu à l'intérieur de ce rectangle.
//    Le rectangle est de couleur color_rect et le titre est centré à
//    l'intérieur.
//    Retourne le rectangle créer.
extern Rectangle rect_top_corner_title(const char *title, Rectangle parent,
    int font_size, Color color_rect);

// display_text_writing: Fonction prenant en charge l'affichage d'une animation
//    de texte qui s'écrit lettre par lettre.
extern bool display_text_writing(game_info_t *game, menu_content_t *menu,
    Rectangle *container, const char *text, int font_size, int offset);

#endif
