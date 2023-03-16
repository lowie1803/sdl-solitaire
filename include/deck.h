#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include "stack.h"

struct Deck {
  Stack faceup;
  Stack facedown;

  int x1_coordinate;
  int y1_coordinate;
};
typedef struct Deck Deck;

bool Deck_initDisplay(Deck *deck);
bool Deck_pushCard(Deck *deck, Card *card);
bool Deck_popCard(Deck *deck);
bool Deck_flip(Deck *deck);
void Deck_render(Deck *deck, SDL_Renderer *renderer);

