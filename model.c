#include "model.h"

#include <SDL.h>
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

#define FONT "arial.ttf"
#define FONTSIZE 20
#define BACKGROUND "background.png"
#define WHITE_TILE "white.png"
#define BLACK_TILE "black.png"
#define GRID "grid.png"


/* Idée pour l'interface graphique:

-Fond sombre (noir/ bleu nuit très foncé)
-Ecriture en rouge.
-creation d'un menu avec l'option "jouer" et l'option "rules" qui afficherait les régles du jeu.
-Un temps de start (avec un compteur 3,2,1... avant que le jeu commence après avoir choisi l'option jouer).
-une grille au centre de l'ecran.
-Nom du groupe en petit en bas à droite.

-Optionnel : ajout d'une petite takuzu "cagnotte" avec ajout de points à chaque coup joué correctement, si erreur la cagnotte se remet à 0.

*/

/* **************************************************************** */

struct Env_t {
  /* PUT YOUR VARIABLES HERE */
  SDL_Texture *background;

  SDL_Texture *white_tile;
  SDL_Texture *black_tile;

  game g;

  int grid_width;
  int grid_height;
};

/* **************************************************************** */

Env *init(SDL_Window *win, SDL_Renderer *ren, int argc, char *argv[]) {
  Env *env = malloc(sizeof(struct Env_t));

  /* PUT YOUR CODE HERE TO INIT TEXTURES, ... */

  env->background = load_texture(ren, BACKGROUND);
  if (!env->background) ERROR("IMG_LoadTexture: %s\n", BACKGROUND);

  env->white_tile = load_texture(ren, WHITE_TILE);
  if (!env->white_tile) ERROR("IMG_LoadTexture: %s\n", WHITE_TILE);

  env->black_tile = load_texture(ren, BLACK_TILE);
  if (!env->black_tile) ERROR("IMG_LoadTexture: %s\n", BLACK_TILE);

  return env;
}

/* **************************************************************** */

void render(SDL_Window *win, SDL_Renderer *ren,
            Env *env) { /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */
}

/* **************************************************************** */

bool process(SDL_Window *win, SDL_Renderer *ren, Env *env, SDL_Event *e) {
  if (e->type == SDL_QUIT) {
    return true;
  }

  /* PUT YOUR CODE HERE TO PROCESS EVENTS */

  return false;
}

/* **************************************************************** */

void clean(SDL_Window *win, SDL_Renderer *ren, Env *env) {
  /* PUT YOUR CODE HERE TO CLEAN MEMORY */

  free(env);
}