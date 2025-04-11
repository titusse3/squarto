#include "utils_menu.h"

void display_img_player(menu_content_t *menu, Rectangle *container,
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

Rectangle rect_top_corner_title(const char *title, Rectangle parent,
    int font_size, Color color_rect) {
  int w_offset = 100;
  int h_offset = 10;
  const int title_size = MeasureText(title, font_size);
  Rectangle rect = {
    parent.x, parent.y, title_size + w_offset, font_size + h_offset
  };
  DrawRectangleRec(rect, color_rect);
  DrawText(title,
      rect.x + rect.width / 2 - title_size / 2,
      rect.y + rect.height / 2 - font_size / 2, font_size, WHITE);
  return rect;
}
