#include "game_view.h"

#include <time.h>
#include <limits.h>
#include <stdbit.h>
#include <stdio.h>

#include "utils_menu.h"
#include "solver.h"
#include "heuristic.h"
#include "raygui.h"

#define SNEGAMAX "Negamax"
#define SMINIMAX "Minimax"
#define SALPHABETA "Alpha-Beta"
#define SNEGALPHABETA "NegAlpha-Beta"
#define SSSS_S "SSS*"

#define SYM_C "\t+"

#define BOT_MSG1 "Je vais te battre !"
#define BOT_MSG2 "Tu es trop lent !"
#define BOT_MSG3 "Je vais te briser les os !"
#define BOT_MSG4 "C'est mon jeu !"

#define MSG_DURATION 75
#define MSG_INTERVAL 100

#define ALLOCATION_MSG "Error during allocation"
#define SOLVER_ERROR "Solver error"

#define ERROR_DISPLAY(game, msg)                                               \
        display_exit_menu(game, (game)->screen_h / 16, (msg),                  \
    MeasureText((msg), (game)->screen_h / 16));                                \
        return;                                                                \

// display_bot_animation: Fonction qui permet l'affichage des animations du bot.
static void display_bot_animation(game_info_t *game, menu_content_t *menu,
    float factor);

static void display_diffictulty_chooser(game_info_t *game,
    menu_content_t *menu, float factor);

// draw_board_game: Fonction qui permet de dessiner le plateau de jeu.
static void draw_board_game(state_t *st, quarto_t *quarto);

// pieces_selectors: Fonction qui permet de selectionner les pieces du jeu.
static void pieces_selectors(state_t *st, game_state_t *gs);

// draw_pieces: Fonction qui permet de dessiner les pieces du jeu.
static void draw_pieces(state_t *st, game_state_t *gs, Camera3D *camera);

// select_case: Fonction qui permet de selectionner une case du plateau de jeu.
static void select_case(state_t *st, Camera3D *camera);

void display_diffictulty_chooser(game_info_t *game, menu_content_t *menu,
    float factor) {
  const char *solvers[] = {
    SMINIMAX, SNEGAMAX, SALPHABETA, SNEGALPHABETA, SSSS_S
  };
  float dropdown_width = game->screen_w / 5.0f;
  float dropdown_height = game->screen_h / 20.0f;
  Rectangle dropdown_rect = {
    .x = game->screen_w - dropdown_width * 1.1f,
    .y = game->screen_h - menu->anims[1].frameRec.height * factor * 1.5f,
    .width = dropdown_width,
    .height = dropdown_height
  };
  int solv = menu->content.game_values.solver;
  bool dropdown_open = menu->content.game_values.dropwdonw_open;
  GuiSetStyle(DEFAULT, TEXT_SIZE, game->screen_h / 25);
  if (GuiDropdownBox(dropdown_rect,
      SMINIMAX ";"SNEGAMAX ";"SALPHABETA ";"SNEGALPHABETA ";"SSSS_S,
      &solv, dropdown_open)) {
    menu->content.game_values.solver = solv;
    menu->content.game_values.dropwdonw_open = !dropdown_open;
  }
  GuiSetStyle(DEFAULT, TEXT_SIZE, game->screen_h / 30);
}

void display_bot_animation(game_info_t *game, menu_content_t *menu,
    float factor) {
  Rectangle anim_rect = {
    .x = game->screen_w - game->screen_w / 4.3f,
    .y = -menu->anims[1].frameRec.height * factor / 3.0f,
    .width = menu->anims[1].frameRec.width * factor,
    .height = menu->anims[1].frameRec.height * factor
  };
  display_animation(game, &menu->anims[1], anim_rect);
  int font_size = game->screen_h / 20;
  const char *dialogue_text[] = {
    BOT_MSG1, nullptr, BOT_MSG2, nullptr, BOT_MSG3, nullptr, BOT_MSG4, nullptr
  };
  size_t number = sizeof dialogue_text / sizeof *dialogue_text;
  int frame = menu->content.game_values.frames / 3;
  int stage = menu->content.game_values.stage;
  if (stage < number) {
    if (dialogue_text[stage] != nullptr) {
      int text_width = MeasureText(dialogue_text[stage], font_size);
      Rectangle dialogue_box = {
        .x = anim_rect.x - text_width + anim_rect.width * 0.2f,
        .y = anim_rect.y + anim_rect.height / 2,
        .width = text_width + 40,
        .height = font_size + 20
      };
      if (dialogue_box.x < 0) {
        dialogue_box.x = 10;
      }
      if (dialogue_box.y + dialogue_box.height > game->screen_h) {
        dialogue_box.y = game->screen_h - dialogue_box.height - 10;
      }
      DrawRectangleRec(dialogue_box, BLACK);
      bool finished = display_text_writing(game, menu, &dialogue_box,
          dialogue_text[stage], font_size, frame);
      if (finished && frame % MSG_DURATION == 0) {
        menu->content.game_values.frames = 0;
        menu->content.game_values.stage = stage + 1;
      }
    }
    if (dialogue_text[stage] == nullptr && frame >= MSG_INTERVAL) {
      menu->content.game_values.frames = 0;
      menu->content.game_values.stage = stage + 1;
    }
  } else {
    menu->content.game_values.stage = 0;
    menu->content.game_values.frames = 0;
  }
}

void draw_board_game(state_t *st, quarto_t *quarto) {
  uint16_t sum = quarto_summary(quarto);
  uint64_t board = quarto_board(quarto);
  for (size_t x = 0; x < 4; ++x) {
    for (size_t z = 0; z < 4; ++z) {
      DrawCube(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          st->c_select[0] == x && st->c_select[1] == z ? PURPLE : LIGHTGRAY
          );
      DrawCubeWires(
          (Vector3) {x * 1.5f - 2.25f, 0.0f, z * 1.5f - 2.25f},
          1.5f,
          1.5f,
          1.5f,
          BLACK
          );
      if (((sum >> (uint16_t) (x * 4 + z)) & 0b1) != 0) {
        unsigned char p = (board >> (4 * (x * 4 + z))) & 0b1111;
        DrawModelEx(
            st->pieces[p & 0b0011],
            (Vector3) {x * 1.5f - 2.25f, 0.75f, z * 1.5f - 2.25f},
            (Vector3) {0.0f, 0.0f, 0.0f},
            0.0f,
            (Vector3) {0.55f,
                       (p & 0b0100) != 0 ? 0.35f : 0.2f,
                       0.55f},
            (p & 0b1000) != 0 ? BLUE : RED
            );
      }
    }
  }
}

void pieces_selectors(state_t *st, game_state_t *gs) {
  float rect_heigth = st->screens->texture.height / 4.0f;
  float rect_width = st->screens->texture.width / 4.0f;
  for (size_t i = 0; i < 4; ++i) {
    for (size_t j = 0; j < 4; ++j) {
      Rectangle r = {
        .x = j * rect_width, .y = i * rect_heigth,
        .width = rect_width, .height = rect_heigth
      };
      bool is_selected = (gs->p_select[0] == 3 - i && gs->p_select[1] == 3 - j);
      if (CheckCollisionPointRec(GetMousePosition(), r) && !is_selected
          && (gs->used >> ((3 - i) * 4 + 3 - j) & 0b1) == 0) {
        DrawRectangleRec(r, Fade(LIGHTGRAY, 0.8f));
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
          gs->p_select[0] = 3 - i;
          gs->p_select[1] = 3 - j;
        }
      } else if (is_selected) {
        DrawRectangleRec(r, Fade(PURPLE, 0.8f));
      } else if ((quarto_current_piece(gs->q) & 0b1111)
          == (3 - i) * 4 + 3 - j) {
        DrawRectangleRec(r, Fade(SKYBLUE, 0.8f));
      }
    }
  }
}

void draw_pieces(state_t *st, game_state_t *gs, Camera3D *camera) {
  camera->position = (Vector3) {
    21.0f, 5.25f, 4.0f
  };
  camera->target = (Vector3) {
    0.0f, 5.25f, 4.0f
  };
  SetShaderValue(
      st->shader,
      st->shader.locs[SHADER_LOC_VECTOR_VIEW],
      &camera->position,
      SHADER_UNIFORM_VEC3
      );
  st->lights[1].position = camera->position;
  UpdateLightValues(st->shader, st->lights[1]);
  BeginTextureMode(*st->screens);
  pieces_selectors(st, gs);
  ClearBackground(BLANK);
  BeginMode3D(*camera);
  BeginShaderMode(st->shader);
  for (size_t x = 0; x < 4; ++x) {
    for (size_t z = 0; z < 4; ++z) {
      DrawModelEx(
          st->pieces[(x * 4 + z) & 0b0011],
          (Vector3) {0.0f, x * 2.85f + 0.25f, z * 2.0f + 1.0f},
          (Vector3) {0.0f, 0.0f, -1.0f},
          32.0f,
          (Vector3) {0.55f,
                     ((x * 4 + z) & 0b0100) != 0 ? 0.2f : 0.1f,
                     0.55f},
          (gs->used >> (x * 4 + z) & 0b1) != 0
          ? DARKGRAY : ((x * 4 + z) & 0b1000) != 0 ? BLUE : RED
          );
    }
  }
  EndShaderMode();
  EndMode3D();
  EndTextureMode();
}

void select_case(state_t *st, Camera3D *camera) {
  Ray ray = {};
  RayCollision collision = {};
  float x = 0;
  float z = 0;
  while (x < 4.0f && !collision.hit) {
    z = 0;
    while (z < 4.0f && !collision.hit) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        ray = GetScreenToWorldRay(GetMousePosition(), *camera);
        collision = GetRayCollisionBox(
            ray,
            (BoundingBox) { (Vector3) { x * 1.5f - 1.25f,
                                        1.5f,
                                        z * 1.5f - 1.25f },
                            (Vector3) { x * 1.5f - 3.25f,
                                        1.5f,
                                        z * 1.5f - 3.25f }}
            );
      }
      ++z;
    }
    ++x;
  }
  if (collision.hit) {
    st->c_select[0] = x - 1;
    st->c_select[1] = z - 1;
  }
}

void draw_game(state_t *st, game_info_t *game, menu_content_t *info,
    piece_t *pieces, position_t *positions, game_state_t *gs) {
  if (st->mk_screen) {
    st->screens = malloc(sizeof *st->screens);
    if (st->screens == nullptr) {
      ERROR_DISPLAY(game, ALLOCATION_MSG);
    }
    st->screens[0] = LoadRenderTexture(game->screen_h / 2,
        game->screen_h / 1.5);
    st->mk_screen = false;
  }
  if (gs->q == nullptr) {
    srand(time(nullptr));
    size_t p = rand() % NB_PIECES;
    gs->q = quarto_init(pieces[p], info->content.game_values.difficulty);
    if (gs->q == nullptr) {
      ERROR_DISPLAY(game, ALLOCATION_MSG);
    }
    gs->used = 1 << (uint16_t) p;
  }
  Camera3D camera = {
    .position = (Vector3) {
      7.5f, 25.0f, 7.5f
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
  if (!quarto_is_game_over(gs->q)) {
    select_case(st, &camera);
    switch (quarto_whos_turn(gs->q)) {
      case PLAYER1:
        quarto_return_t qrt;
        if (st->c_select[0] != UNDEF_COORD && gs->p_select[0] != UNDEF_COORD) {
          if ((qrt = quarto_play(gs->q,
              pieces[
                (size_t) (gs->p_select[0] * 4 + gs->p_select[1])],
              positions[
                15 - (size_t) (st->c_select[0] * 4 + st->c_select[1])])
              == NO_ERROR)) {
            gs->used |= 0b1 <<
                (uint16_t) (gs->p_select[0] * 4 + gs->p_select[1]);
          } else {
            fprintf(stderr, "DEBUG: There where an error\n");
          }
          gs->p_select[0] = UNDEF_COORD;
          gs->p_select[1] = UNDEF_COORD;
          fprintf(stderr, "DEBUG: Quarto return: %d\n", qrt);
        }
        break;
      case PLAYER2:
        move_t move;
        switch (info->content.game_values.solver) {
          case MINIMAX:
            if (!min_max(gs->q, max_heuristic, 2, true, &move)) {
              ERROR_DISPLAY(game, SOLVER_ERROR);
            }
            fprintf(stderr, "DEBUG: MINIMAX - piece: %04b\n",
                (int) (move.piece) & 0b1111);
            break;
          case NEGAMAX:
            if (!nega_max(gs->q, heuristic, 2, &move)) {
              ERROR_DISPLAY(game, SOLVER_ERROR);
            }
            break;
          case ALPHABETA:
            if (!alpha_beta(gs->q, max_heuristic, 4, -INT_MAX, INT_MAX, true,
                &move)) {
              ERROR_DISPLAY(game, SOLVER_ERROR);
            }
            break;
          case NEGALPHABETA:
            if (!negalpha_beta(gs->q, heuristic, 4, -INT_MAX, INT_MAX, &move)) {
              ERROR_DISPLAY(game, SOLVER_ERROR);
            }
            break;
          case SSS_S:
            if (!sss_star(gs->q, max_heuristic, 1, true, &move)) {
              ERROR_DISPLAY(game, SOLVER_ERROR);
            }
            break;
        }
        gs->p_select[0] = UNDEF_COORD;
        gs->p_select[1] = UNDEF_COORD;
        quarto_play(gs->q, move.piece, move.pos);
        uint16_t ps = (stdc_first_leading_one_ull(move.pos) - 1) / 4;
        gs->used |= 1 << ps;
        break;
    }
  }
  BeginMode3D(camera);
  BeginShaderMode(st->shader);
  draw_board_game(st, gs->q);
  EndShaderMode();
  EndMode3D();
  draw_pieces(st, gs, &camera);
  DrawTextureRec(
      st->screens->texture,
      (Rectangle) {0.0f,
                   0.0f,
                   (float) st->screens->texture.width,
                   (float) -st->screens->texture.height},
      (Vector2) {0.0f, 0.0f}, WHITE);
  float factor = (float) game->screen_w / 290;
  display_bot_animation(game, info, factor);
  display_diffictulty_chooser(game, info, factor);
  player_t p = quarto_winner(gs->q);
  if (p != NEITHER) {
    bool win = p == PLAYER1;
    const char *t = win ? "You w in !" : "You Loose !";
    display_end_animation(game, &info->anims[0], info->anim_font, t, win);
  }
}
