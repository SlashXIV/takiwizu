#include "model.h"

#include <SDL2/SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "annex_funcs.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

#define FONT "sprites/arial.ttf"
#define FONTSIZE 50
#define FONTSIZEHELP 20

#define BACKGROUND "sprites/background.jpg"
#define WHITE_TILE "sprites/white.png"
#define BLACK_TILE "sprites/black.png"
#define EMPTY_TILE "sprites/empty_frame.jpg"
#define IMMUTABLE_WHITE "sprites/immutable_white.png"
#define IMMUTABLE_BLACK "sprites/immutable_black.png"
#define HELP_BACKGROUND "sprites/help_background.jpg"

#define TILE_SIZE 50

/* Idée pour l'interface graphique:

-Fond sombre (noir/ bleu nuit très foncé)
-Ecriture en rouge.
-creation d'un menu avec l'option "jouer" et l'option "rules" qui afficherait
les régles du jeu. -Un temps de start (avec un compteur 3,2,1... avant que le
jeu commence après avoir choisi l'option jouer). -une grille au centre de
l'ecran. -Nom du groupe en petit en bas à droite.

-Optionnel : ajout d'une petite takuzu "cagnotte" avec ajout de points à chaque
coup joué correctement, si erreur la cagnotte se remet à 0.

*/

/* **************************************************************** */

struct Env_t {
  /* PUT YOUR VARIABLES HERE */
  SDL_Texture *background;

  SDL_Texture *white_tile;
  SDL_Texture *black_tile;
  SDL_Texture *empty_tile;
  SDL_Texture *immutable_white;
  SDL_Texture *immutable_black;
  SDL_Texture *titre;
  SDL_Texture *help;
  bool help_pressed;
  SDL_Texture *help_screen;
  game g;
};

void calculate_grid_position(int window_width, int window_height,
                             int grid_width, int grid_height, int *grid_x,
                             int *grid_y) {
  *grid_x = (window_width - (grid_width * TILE_SIZE)) / 2;
  *grid_y = (window_height - (grid_height * TILE_SIZE)) / 2;
}

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc,
          char *argv[]) {  // main equivalent

  Env *env = malloc(sizeof(struct Env_t));
  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */

  env->background = IMG_LoadTexture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  env->white_tile = IMG_LoadTexture(ren, WHITE_TILE);
  if (!env->white_tile) ERROR("IMG_LoadTexture: %s\n", WHITE_TILE);

  env->black_tile = IMG_LoadTexture(ren, BLACK_TILE);
  if (!env->black_tile) ERROR("IMG_LoadTexture: %s\n", BLACK_TILE);

  env->empty_tile = IMG_LoadTexture(ren, EMPTY_TILE);
  if (!env->empty_tile) ERROR("IMG_LoadTexture: %s\n", EMPTY_TILE);

  env->immutable_white = IMG_LoadTexture(ren, IMMUTABLE_WHITE);
  if (!env->immutable_white) ERROR("IMG_LoadTexture: %s\n", IMMUTABLE_WHITE);

  env->immutable_black = IMG_LoadTexture(ren, IMMUTABLE_BLACK);
  if (!env->immutable_black) ERROR("IMG_LoadTexture: %s\n", IMMUTABLE_BLACK);

  SDL_Color color = {255, 255, 255, 255};
  TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_BOLD);

  TTF_Font *font_help = TTF_OpenFont(FONT, FONTSIZEHELP);
  if (!font_help) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font_help, TTF_STYLE_BOLD);

  SDL_Surface *surface = TTF_RenderText_Blended(font, "Takuzu", color);
  env->titre = SDL_CreateTextureFromSurface(ren, surface);
  SDL_FreeSurface(surface);

  // press H for help text
  SDL_Surface *help_surface =
      TTF_RenderText_Blended(font_help, "Press H for help", color);
  env->help = SDL_CreateTextureFromSurface(ren, help_surface);
  SDL_FreeSurface(help_surface);
  TTF_CloseFont(font);

  env->g = game_default();

  env->help_pressed = false;

  env->help_screen = IMG_LoadTexture(ren, HELP_BACKGROUND);

  return env;
}

/* **************************************************************** */

void set_tile_pos(SDL_Rect *rect, int x, int y) {
  rect->x = x;
  rect->y = y;
}

void move_to(SDL_Rect *rect, int x, int y) {
  rect->x = x;
  rect->y = y;
}

void mv_left(SDL_Rect *rect) { rect->x -= TILE_SIZE; }
void mv_right(SDL_Rect *rect) { rect->x += TILE_SIZE; }
void mv_up(SDL_Rect *rect) { rect->y -= TILE_SIZE; }
void mv_down(SDL_Rect *rect) { rect->y += TILE_SIZE; }

void SDL_set_tile(Env *env, SDL_Renderer *ren, SDL_Rect rect,
                  SDL_Texture *tile) {
  SDL_QueryTexture(tile, NULL, NULL, &rect.w, &rect.h);
  SDL_RenderCopy(ren, tile, NULL, &rect);
}

void render(SDL_Window *win, SDL_Renderer *ren,
            Env *env) { /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */

  int w, h;
  SDL_GetWindowSize(win, &w, &h);
  SDL_RenderCopy(ren, env->background, NULL, NULL); /* étirable */

  SDL_Rect rect;

  /*____________________________________________________
          AFFICHAGE DU TITRE DU JEU
      ____________________________________________________  */

  SDL_QueryTexture(env->titre, NULL, NULL, &rect.w, &rect.h);
  rect.x = (w - rect.w) / 2;
  rect.y = 50;
  SDL_RenderCopy(ren, env->titre, NULL, &rect);

  /*____________________________________________________
          AFFICHAGE DU BOUTON H (HELP) EN BAS AU CENTRE
      ____________________________________________________  */

  SDL_QueryTexture(env->help, NULL, NULL, &rect.w, &rect.h);
  rect.x = (w - rect.w) / 2;
  rect.y = h - 50;
  SDL_RenderCopy(ren, env->help, NULL, &rect);

  /*____________________________________________________
        CENTRAGE DE LA GRILLE DE JEU DANS LA FENÊTRE
    ____________________________________________________  */

  int grid_w = DEFAULT_SIZE * TILE_SIZE;
  int grid_h = DEFAULT_SIZE * TILE_SIZE;
  int grid_x = (w - grid_w) / 2;
  int grid_y = (h - grid_h) / 2;

  /*____________________________________________________
        AFFICHAGE DU JEU DANS LA GRILLE DE L'INTERFACE
    ____________________________________________________  */

  rect.x = grid_x;
  rect.y = grid_y;

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      square current_square = game_get_square(env->g, i, j);

      if (current_square == S_ZERO)
        SDL_set_tile(env, ren, rect, env->white_tile);
      else if (current_square == S_ONE)
        SDL_set_tile(env, ren, rect, env->black_tile);
      else if (current_square == S_EMPTY)
        SDL_set_tile(env, ren, rect, env->empty_tile);
      else if (current_square == S_IMMUTABLE_ZERO)
        SDL_set_tile(env, ren, rect, env->immutable_white);
      else if (current_square == S_IMMUTABLE_ONE)
        SDL_set_tile(env, ren, rect, env->immutable_black);

      mv_right(&rect);
    }

    mv_down(&rect);
    for (int k = 0; k < game_nb_cols(env->g); k++) mv_left(&rect);
  }

  /*_____________________________________________
    RECHERCHE D'ERREURS ET AFFICHAGE DE CROIX
  _______________________________________________ */

  for (int i = 0; i < game_nb_rows(env->g); i++) {
    for (int j = 0; j < game_nb_cols(env->g); j++) {
      if (game_has_error(env->g, j, i)) {
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);  // Définir la couleur
                                                      // rouge
        int line_thickness = 20;  // Définir l'épaisseur de la ligne
        int x1 = grid_x + i * TILE_SIZE + line_thickness / 2;
        int y1 = grid_y + j * TILE_SIZE + line_thickness / 2;
        int x2 = grid_x + (i + 1) * TILE_SIZE - line_thickness / 2;
        int y2 = grid_y + (j + 1) * TILE_SIZE - line_thickness / 2;
        SDL_RenderDrawLine(ren, x1, y1, x2, y2);  // Dessiner la première ligne

        int x3 = grid_x + (i + 1) * TILE_SIZE - line_thickness / 2;
        int y3 = grid_y + j * TILE_SIZE + line_thickness / 2;
        int x4 = grid_x + i * TILE_SIZE + line_thickness / 2;
        int y4 = grid_y + (j + 1) * TILE_SIZE - line_thickness / 2;
        SDL_RenderDrawLine(ren, x3, y3, x4, y4);  // Dessiner la deuxième ligne
      }
    }
  }

  // SI H EST APPUYÉ, AFFICHER L'ÉCRAN D'AIDE

  if (env->help_pressed) {
    SDL_RenderCopy(ren, env->help_screen, NULL, NULL);
  }

  SDL_RenderPresent(ren);
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  // Quitter le jeu si l'utilisateur clique sur la croix en haut à droite de la
  // fenêtre
  if (e->type == SDL_QUIT) {
    return true;
  }

  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  int grid_w = DEFAULT_SIZE * TILE_SIZE;
  int grid_h = DEFAULT_SIZE * TILE_SIZE;
  int grid_x = (w - grid_w) / 2;
  int grid_y = (h - grid_h) / 2;

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'w'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_w) {
    SDL_Rect rect;
    SDL_GetMouseState(&rect.x, &rect.y);

    // Calculez l'indice de la colonne et de la rangée correspondantes
    int col_index = (rect.x - grid_x) / TILE_SIZE;
    int row_index = (rect.y - grid_y) / TILE_SIZE;

    // Calcul du square correspondant
    square square_pointed_at_event =
        game_get_square(env->g, row_index, col_index);

    // Vérifiez que les indices sont valides avant de les utiliser
    if (col_index >= 0 && col_index < game_nb_cols(env->g) && row_index >= 0 &&
        row_index < game_nb_rows(env->g)) {
      if (!zero_square(square_pointed_at_event) &&
          !immutable_square(square_pointed_at_event)) {
        game_play_move(env->g, row_index, col_index, S_ZERO);
      }
    }
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'b'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b) {
    SDL_Rect rect;
    SDL_GetMouseState(&rect.x, &rect.y);

    // Calculez l'indice de la colonne et de la rangée correspondantes
    int col_index = (rect.x - grid_x) / TILE_SIZE;
    int row_index = (rect.y - grid_y) / TILE_SIZE;

    square square_pointed_at_event =
        game_get_square(env->g, row_index, col_index);

    // Vérifiez que les indices sont valides avant de les utiliser
    if (col_index >= 0 && col_index < game_nb_cols(env->g) && row_index >= 0 &&
        row_index < game_nb_rows(env->g)) {
      if (!one_square(square_pointed_at_event) &&
          !immutable_square(square_pointed_at_event)) {
        game_play_move(env->g, row_index, col_index, S_ONE);
      }
    }
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'e'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_e) {
    SDL_Rect rect;
    SDL_GetMouseState(&rect.x, &rect.y);

    // Calculez l'indice de la colonne et de la rangée correspondantes
    int col_index = (rect.x - grid_x) / TILE_SIZE;
    int row_index = (rect.y - grid_y) / TILE_SIZE;
    square square_pointed_at_event =
        game_get_square(env->g, row_index, col_index);

    // Vérifiez que les indices sont valides avant de les utiliser
    if (col_index >= 0 && col_index < game_nb_cols(env->g) && row_index >= 0 &&
        row_index < game_nb_rows(env->g)) {
      if (!immutable_square(square_pointed_at_event) ||
          !empty_square(square_pointed_at_event)) {
        game_play_move(env->g, row_index, col_index, S_EMPTY);
      }
    }
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'r'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_r) {
    game_restart(env->g);
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'z'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_z) {
    game_undo(env->g);
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'y'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_y) {
    game_redo(env->g);
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'h'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_h) {
    env->help_pressed = !env->help_pressed;
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'q'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_q) {
    exit(EXIT_SUCCESS);
  }

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  SDL_DestroyTexture(env->background);

  SDL_DestroyTexture(env->white_tile);

  SDL_DestroyTexture(env->black_tile);

  SDL_DestroyTexture(env->immutable_black);

  SDL_DestroyTexture(env->immutable_white);

  free(env->g);

  free(env);
}