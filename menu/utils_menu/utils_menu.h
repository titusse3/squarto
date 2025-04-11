#ifndef UTILS_MENU_H
#define UTILS_MENU_H

#include <stdlib.h>
#include <raylib.h>

#include "menu.h"

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

#endif
