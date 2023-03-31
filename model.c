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
#define MCFONT "sprites/Minecraft.ttf"
#define FONTSIZE 50
#define FONTSIZEHELP 20

#define BACKGROUND "sprites/background.jpg"
#define WHITE_TILE "sprites/white.png"
#define BLACK_TILE "sprites/black.png"
#define EMPTY_TILE "sprites/empty_frame.jpg"
#define IMMUTABLE_WHITE "sprites/immutable_white.png"
#define IMMUTABLE_BLACK "sprites/immutable_black.png"
#define HELP_BACKGROUND "sprites/help_background.jpg"
#define GAME_1 "sprites/game_1.txt"

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
  SDL_Texture *team_name;
  game g;
  game g2;
  SDL_Texture *victory_font;
};

void calculate_grid_position(int window_width, int window_height,
                             int grid_width, int grid_height, int *grid_x,
                             int *grid_y) {
  *grid_x = (window_width - (grid_width * TILE_SIZE)) / 2;
  *grid_y = (window_height - (grid_height * TILE_SIZE)) / 2;
}

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

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

  SDL_Color color = {0, 0, 0, 255};
  SDL_Color red = {255, 0, 0, 255};
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

  //Team 6A:

    SDL_Surface *team_name =
      TTF_RenderText_Blended(font_help, "Team 6A", color);
  env->team_name = SDL_CreateTextureFromSurface(ren, team_name);
  SDL_FreeSurface(team_name);

  TTF_CloseFont(font);

  env->g = game_default();
  env->g2 = game_custom1();
  game_save(env->g2, "game_load.txt");

  env->help_pressed = false;

  env->help_screen = IMG_LoadTexture(ren, HELP_BACKGROUND);

  TTF_Font *victory_font = TTF_OpenFont(MCFONT, FONTSIZE);
  if (!victory_font) ERROR("TTF_OpenFont: %s\n", MCFONT);

  SDL_Surface *victory_surface =
      TTF_RenderText_Blended(victory_font, "Victory !!", red);
  env->victory_font = SDL_CreateTextureFromSurface(ren, victory_surface);

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

void mv_left(SDL_Rect *rect) { rect->x -= TILE_SIZE - 2; }
void mv_right(SDL_Rect *rect) { rect->x += TILE_SIZE - 2; }
void mv_up(SDL_Rect *rect) { rect->y -= TILE_SIZE - 2; }
void mv_down(SDL_Rect *rect) { rect->y += TILE_SIZE - 2; }

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
          AFFICHAGE DU NOM DE L'EQUIPE
      ____________________________________________________  */

  SDL_QueryTexture(env->team_name, NULL, NULL, &rect.w, &rect.h);
  rect.x = 470;
  rect.y = 551;
  SDL_RenderCopy(ren, env->team_name, NULL, &rect);

  /*____________________________________________________
        CENTRAGE DE LA GRILLE DE JEU DANS LA FENÊTRE
    ____________________________________________________  */

  int grid_w = DEFAULT_SIZE * TILE_SIZE;
  int grid_h = DEFAULT_SIZE * TILE_SIZE;
  int grid_x = (w - grid_w) / 2;
  int grid_y = (h - grid_h) / 2;

  /*____________________________________________________
            AFFICHAGE DU JEU DANS LA GRILLE DE L'INTERFACE
  ______________________________________________________*/

  // Positionnement initial de la première case de la grille
  rect.x = grid_x;
  rect.y = grid_y;

  // Boucle d'affichage des cases de la grille
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      // Récupération de la valeur de la case
      square current_square = game_get_square(env->g, i, j);

      // Affichage de la case correspondante
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

      // Déplacement à la case suivante
      mv_right(&rect);
    }

    // Déplacement à la ligne suivante
    mv_down(&rect);

    // Retour au début de la ligne
    for (int k = 0; k < game_nb_cols(env->g); k++) mv_left(&rect);
  }

  /*_____________________________________________
        RECHERCHE D'ERREURS ET AFFICHAGE DE CARRÉS PLEINS
    _______________________________________________ */

  for (int i = 0; i < game_nb_rows(env->g); i++) {
    for (int j = 0; j < game_nb_cols(env->g); j++) {
      if (game_has_error(env->g, j, i)) {  // Si la case a une erreur
        // Définir la couleur rouge avec opacité réduite de moitié (128/255)
        SDL_SetRenderDrawColor(ren, 255, 0, 0, 128);
        // Définir le mode de fusion pour dessiner des formes semi-transparentes
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
        // Définir le rectangle à dessiner
        int row = j * (TILE_SIZE - 2);
        SDL_Rect rect = {.x = grid_x + i * (TILE_SIZE - 2),
                         .y = grid_y + row,
                         .w = TILE_SIZE,
                         .h = TILE_SIZE};
        // Dessiner le carré semi-transparent
        SDL_RenderFillRect(ren, &rect);
        // Réinitialiser le mode de fusion pour dessiner des formes opaques à
        // nouveau
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
      }
    }
  }

  // SI H EST APPUYÉ, AFFICHER L'ÉCRAN D'AIDE

  if (env->help_pressed) {
    SDL_RenderCopy(ren, env->help_screen, NULL, NULL);
  }

  // SI LE JEU EST GAGNÉ, AFFICHER L'ÉCRAN DE VICTOIRE

  if (game_is_over(env->g)) {
    // SDL_RenderCopy(ren, env->win_screen, NULL, NULL);
    // SDL_SetTextureAlphaMod(env->victory_font, 128); // 50% transparency

    int victory_width, victory_height;
    SDL_QueryTexture(env->victory_font, NULL, NULL, &victory_width,
                     &victory_height);

    int grid_w = DEFAULT_SIZE * TILE_SIZE;
    int grid_h = DEFAULT_SIZE * TILE_SIZE;
    int grid_x = (w - grid_w) / 2;
    int grid_y = (h - grid_h) / 2;

    int victory_x = grid_x + (grid_w - victory_width) / 2;
    int victory_y = grid_y + grid_h + 10;  // 10 pixels below the grid

    SDL_Rect rect = {victory_x, victory_y, victory_width, victory_height};
    if (!env->help_pressed) {
      SDL_RenderCopyEx(ren, env->victory_font, NULL, &rect, 0, NULL,
                       SDL_FLIP_NONE);
    }
  }

  SDL_RenderPresent(ren);
}

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
    bool move_possible;

    // Calculez l'indice de la colonne et de la rangée correspondantes
    int col_index = (rect.x - grid_x) / TILE_SIZE;
    int row_index = (rect.y - grid_y) / TILE_SIZE;
    bool inside_grid = (col_index >= 0 && col_index < game_nb_cols(env->g) &&
                        row_index >= 0 && row_index < game_nb_rows(env->g));
    if (inside_grid) {
      square square_pointed_at_event =
          game_get_square(env->g, row_index, col_index);
      bool already_zero_square = (square_pointed_at_event == S_ZERO);
      bool already_immutable = (immutable_square(square_pointed_at_event));
      move_possible = inside_grid && !already_zero_square && !already_immutable;
    }

    if (!move_possible) {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erreur",
                               "le move est impossible", NULL);
    } else {
      game_play_move(env->g, row_index, col_index, S_ZERO);
    }
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'b'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_b) {
    SDL_Rect rect;
    SDL_GetMouseState(&rect.x, &rect.y);
    bool move_possible;

    int col_index = (rect.x - grid_x) / TILE_SIZE;
    int row_index = (rect.y - grid_y) / TILE_SIZE;
    bool inside_grid = (col_index >= 0 && col_index < game_nb_cols(env->g) &&
                        row_index >= 0 && row_index < game_nb_rows(env->g));

    if (inside_grid) {
      square square_pointed_at_event =
          game_get_square(env->g, row_index, col_index);
      bool already_one_square = (square_pointed_at_event == S_ONE);
      bool already_immutable = (immutable_square(square_pointed_at_event));
      move_possible = !already_one_square && !already_immutable;

      if (!move_possible) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erreur",
                                 "le move est impossible", NULL);
      } else {
        game_play_move(env->g, row_index, col_index, S_ONE);
      }
    } else {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erreur",
                               "le move est impossible", NULL);
    }
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'e'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_e) {
    SDL_Rect rect;
    SDL_GetMouseState(&rect.x, &rect.y);
    bool move_possible;

    int col_index = (rect.x - grid_x) / TILE_SIZE;
    int row_index = (rect.y - grid_y) / TILE_SIZE;
    bool inside_grid = (col_index >= 0 && col_index < game_nb_cols(env->g) &&
                        row_index >= 0 && row_index < game_nb_rows(env->g));

    if (inside_grid) {
      square square_pointed_at_event =
          game_get_square(env->g, row_index, col_index);
      bool already_two_square = (square_pointed_at_event == S_EMPTY);
      bool already_immutable = (immutable_square(square_pointed_at_event));
      move_possible = inside_grid && !already_two_square && !already_immutable;

      if (!move_possible) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erreur",
                                 "le move est impossible", NULL);
      } else {
        game_play_move(env->g, row_index, col_index, S_EMPTY);
      }
    } else {
      SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Erreur",
                               "le move est impossible", NULL);
    }
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'l'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_l) {
    env->g = game_load("game_load.txt");

    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information",
                             "Game loaded", NULL);
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

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'q' OU LA TOUCHE 'ESC'
  if (e->type == SDL_KEYDOWN &&
      (e->key.keysym.sym == SDLK_ESCAPE || e->key.keysym.sym == SDLK_q)) {
    return true;
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE 'm'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_m) {
    game_solve(env->g);
    // show message box "game saved"
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information",
                             "Game solved", NULL);
  }

  // SI L'UTILISATEUR APPUIE SUR LA TOUCHE  's'
  if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_s) {
    game_save(env->g, "game_save.txt");
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information",
                             "Game saved", NULL);
  }

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  SDL_DestroyTexture(env->background);

  SDL_DestroyTexture(env->white_tile);

  SDL_DestroyTexture(env->black_tile);

  SDL_DestroyTexture(env->immutable_black);

  SDL_DestroyTexture(env->immutable_white);

  SDL_DestroyTexture(env->help_screen);

  SDL_DestroyTexture(env->titre);

  SDL_DestroyTexture(env->help);

  SDL_DestroyTexture(env->team_name);

  free(env->g);

  free(env);
}