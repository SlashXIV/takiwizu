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

#define FONT "sprites/Leaf.ttf"
#define MCFONT "sprites/Minecraft.ttf"
#define FONTSIZE 70
#define FONTSIZEHELP 

#define BACKGROUND "sprites/background.jpeg"
#define WHITE_TILE "sprites/white.png"
#define BLACK_TILE "sprites/black.png"
#define EMPTY_TILE "sprites/empty_frame.jpg"
#define IMMUTABLE_WHITE "sprites/immutable_white.png"
#define IMMUTABLE_BLACK "sprites/immutable_black.png"
#define HELP_BACKGROUND "sprites/help_background_img.jpg"
#define ICON "sprites/icon.png"
#define GAME_1 "sprites/game_1.txt"

#define TILE_SIZE 50

struct Env_t {
  SDL_Texture *background;
  SDL_Texture *white_tile;
  SDL_Texture *black_tile;
  SDL_Texture *empty_tile;
  SDL_Texture *immutable_white;
  SDL_Texture *immutable_black;
  SDL_Texture *titre;
  SDL_Texture *help;
  SDL_Texture *help_screen;
  SDL_Texture *team_name;
  SDL_Texture *victory_font;
<<<<<<< HEAD

=======
  SDL_Surface *icon;
>>>>>>> 26af6b060e7ce01e3b0352cfc04aaf08da4af731
  game g;
  bool help_pressed;
};

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
  
  SDL_Color red = {255, 0, 0, 255};
  SDL_Color dark_green = {27, 59, 34, 255};
  SDL_Color light_green = {0, 255, 122, 255};
  SDL_Color black = {0, 0, 0, 255};


  TTF_Font *font = TTF_OpenFont(FONT, FONTSIZE);
  if (!font) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

  TTF_Font *font_help = TTF_OpenFont(FONT, FONTSIZEHELP);
  if (!font_help) ERROR("TTF_OpenFont: %s\n", FONT);
  TTF_SetFontStyle(font_help, TTF_STYLE_BOLD);

  SDL_Surface *surface = TTF_RenderText_Blended(font, "T A K I W I Z U", light_green);
  env->titre = SDL_CreateTextureFromSurface(ren, surface);
  SDL_FreeSurface(surface);

  // press H for help text:
  SDL_Surface *help_surface =
      TTF_RenderText_Blended(font_help, "Press H for help", black);
  env->help = SDL_CreateTextureFromSurface(ren, help_surface);
  SDL_FreeSurface(help_surface);

  // Team 6A:
  SDL_Surface *team_name = TTF_RenderText_Blended(font_help, "Team 6A", dark_green);
  env->team_name = SDL_CreateTextureFromSurface(ren, team_name);
  SDL_FreeSurface(team_name);

  TTF_CloseFont(font);

  env->g = game_default();
  
  env->help_pressed = false;

  env->help_screen = IMG_LoadTexture(ren, HELP_BACKGROUND);

  TTF_Font *victory_font = TTF_OpenFont(MCFONT, FONTSIZE);
  if (!victory_font) ERROR("TTF_OpenFont: %s\n", MCFONT);

  SDL_Surface *victory_surface =
      TTF_RenderText_Blended(victory_font, "Victory !!", red);
  env->victory_font = SDL_CreateTextureFromSurface(ren, victory_surface);

  return env;
}

void render(SDL_Window *win, SDL_Renderer *ren,
            Env *env) { 

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
  rect.x = w - rect.w - 20;
  rect.y = h - rect.h - 20;
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

void show_error_box(const char *msg) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", msg, NULL);
}

void show_info_box(const char *msg) {
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info", msg, NULL);
}

bool inside_grid(int i, int j) {
  return ((i >= 0 && i < DEFAULT_SIZE) && (j >= 0 && j < DEFAULT_SIZE));
}

bool user_want_to_place_tile(SDL_Event *e) {
  return (e->type == SDL_KEYDOWN &&
          (e->key.keysym.sym == SDLK_b || e->key.keysym.sym == SDLK_w ||
           e->key.keysym.sym == SDLK_e));
}

bool user_want_to_load_game(SDL_Event *e) {
  return (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_l);
}

bool user_want_to_restart_game(SDL_Event *e) {
  return (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_r);
}

bool user_want_to_undo(SDL_Event *e) {
  return (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_z);
}

bool user_want_to_redo(SDL_Event *e) {
  return (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_y);
}

char *get_filename_input_from_terminal() {
  char *filename = malloc(sizeof(char) * 100);
  printf(
      "Entrez le nom du fichier à charger parmis ceux proposés ci-dessous: "
      "\n=> nom de la save :");
  system("ls -g *.txt | grep -v 'CMakeCache.txt'");
  scanf("%s", filename);
  return filename;
}

bool load_game_from_file(Env *env) {
  show_info_box(
      "CHARGEMENT D'UNE SAVE (l): \n"
      "- Prenez en main le terminal joint à l'application;\n"
      "- Suivez les instructions.\n");

  char *filename = get_filename_input_from_terminal();
  if (filename != NULL) {
    game g_test = game_load(filename);
    if (g_test == NULL) {
      show_error_box("Impossible de charger la sauvegarde");
      return false;
    }
    env->g = game_load(filename);
  } else {
    return false;
  }
  free(filename);
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information",
                           "Partie chargée avec succès!", NULL);
  return true;
}

bool user_requested_help(SDL_Event *e) {
  return (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_h);
}

bool user_requested_solving(SDL_Event *e) {
  return (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_s);
}

void toggle_help(Env *env) { env->help_pressed = !env->help_pressed; }

void toggle_solving(Env *env) {

  game_solve(env->g);
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information",
                           "Solution du jeu!", NULL);
}

square square_soon_placed(SDL_Event *e) {
  if (e->key.keysym.sym == SDLK_b) return S_ONE;
  if (e->key.keysym.sym == SDLK_w) return S_ZERO;
  if (e->key.keysym.sym == SDLK_e) return S_EMPTY;
  return S_EMPTY;
}

void compute_move(SDL_Window *win, SDL_Event *e, game g) {
  int w, h;
  SDL_GetWindowSize(win, &w, &h);

  int grid_w = DEFAULT_SIZE * TILE_SIZE;
  int grid_h = DEFAULT_SIZE * TILE_SIZE;
  int grid_x = (w - grid_w) / 2;
  int grid_y = (h - grid_h) / 2;

  SDL_Rect pos;
  SDL_GetMouseState(&pos.x, &pos.y);

  int col_index = (pos.x - grid_x) / TILE_SIZE;
  int row_index = (pos.y - grid_y) / TILE_SIZE;

  if (!inside_grid(row_index, col_index)) {
    show_error_box(
        "Vous placez une tuile en dehors de la grille du jeu. "
        "Réessayez sur une case disponible");
    return;
  }

  square square_pointed = game_get_square(g, row_index, col_index);
  square soon_placed = square_soon_placed(e);

  if (immutable_square(square_pointed) || square_pointed == soon_placed) {
    show_error_box(
        "Vous placez une tuile sur une case déjà occupée. "
        "Réessayez sur une case disponible");
    return;
  }
  game_play_move(g, row_index, col_index, soon_placed);
}

bool user_quit(SDL_Event *e) {
  return (e->type == SDL_QUIT || e->key.keysym.sym == SDLK_ESCAPE ||
          e->key.keysym.sym == SDLK_q);
}

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (user_quit(e)) return true;  // end of processing loop (quit the game)

  if (user_want_to_place_tile(e)) compute_move(win, e, env->g);

  if (user_want_to_load_game(e)) load_game_from_file(env);

  if (user_want_to_restart_game(e)) game_restart(env->g);

  if (user_want_to_undo(e)) game_undo(env->g);

  if (user_want_to_redo(e)) game_redo(env->g);

  if (user_requested_help(e)) toggle_help(env);

  if (user_requested_solving(e)) toggle_solving(env);

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