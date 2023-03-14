#include "texture_handler.h"

bool TextureHandler_load(TextureHandler* th, const char* filename, SDL_Renderer *renderer) {
  SDL_Surface* surf = IMG_Load(filename);
  int width = surf->w;
  int height = surf->h;
  int clipPerRow = 13;
  int clipPerColumn = 6;
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
  SDL_FreeSurface(surf);
  SDL_Texture* clip[clipPerColumn][clipPerRow];
      // fprintf(stderr, "%d %d", width, height);
  for(int i = 0; i < clipPerColumn; i++)
  {
    for(int j = 0; j < clipPerRow; j++)
    {
      clip[i][j] = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        width/clipPerRow,
        height/clipPerColumn
      );
      SDL_SetTextureBlendMode(clip[i][j], SDL_BLENDMODE_BLEND);
      SDL_Rect rect = {
        j*width/clipPerRow,
        i*height/clipPerColumn,
        width/clipPerRow,
        height/clipPerColumn
      };
      SDL_SetRenderTarget(renderer, clip[i][j]);
      SDL_RenderCopy(renderer, texture, &rect, NULL);
    }
  }
  SDL_SetRenderTarget(renderer, NULL);

  for (int i = 0; i < SUITS_COUNT; i++) {
    for (int j = 0; j < RANKS_COUNT; j++) {
      th->card_fronts[i][j] = clip[i][j];
    }
  }
  th->card_back = clip[5][2];
  return true;
}

SDL_Texture* TextureHandler_getFront(TextureHandler* th, int suit, int rank) {
  return th->card_fronts[suit][rank];
}
SDL_Texture* TextureHandler_getBack(TextureHandler* th) {
  return th->card_back;
}
