#include "game.h"
#include "klondike.h"

// handle interactions
void Game_selectInteraction(Game* game, int atX, int atY);
void Game_moveInteraction(Game* game, int atX, int atY);

bool Game_start(SDL_Renderer *renderer, int w, int h);
