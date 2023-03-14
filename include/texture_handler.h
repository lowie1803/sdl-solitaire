#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL_image.h>
#include <SDL.h>

#define SUITS_COUNT 4
#define RANKS_COUNT 13

struct TextureHandler {
  SDL_Texture* card_fronts[SUITS_COUNT][RANKS_COUNT];
  SDL_Texture* card_back;
};
typedef struct TextureHandler TextureHandler;

bool TextureHandler_load(TextureHandler* th, const char* filename, SDL_Renderer *renderer);
SDL_Texture* TextureHandler_getFront(TextureHandler* th, int suit, int rank);
SDL_Texture* TextureHandler_getBack(TextureHandler* th);
