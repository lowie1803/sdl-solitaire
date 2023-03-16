#include <stdio.h>
#include <stdbool.h>

#include <SDL.h>
#include "utils.h"

enum suit { HEART = 1, DIAMOND = 2, SPADE = 3, CLUB = 4 };
enum rank {
  ACE = 1, TWO = 2, THREE = 3, FOUR = 4, FIVE = 5, SIX = 6,
  SEVEN = 7, EIGHT = 8, NINE = 9, TEN = 10, JACK = 11, QUEEN = 12,
  KING = 13
};

#define MAX_CARDS_PER_STACK (52)
#define CARD_HEIGHT (91)
#define CARD_WIDTH (65)
#define CARD_BORDER_WIDTH (3)

struct Card
{
  enum suit _suit;
  enum rank _rank;
  bool isSelected;
  bool isFaceDown;
  // Facedown game logic: left-click to faceup, only when top of stack.

  SDL_Rect rect;
  SDL_Color rectColor;
  
  SDL_Rect border;
  SDL_Color borderColor;
};
typedef struct Card Card;

bool Card_initDisplay(Card *card);

void Card_select(Card *card);
void Card_deselect(Card *card);

void Card_flipUp(Card *card);
void Card_flipDown(Card *card);
void Card_render(Card *card, SDL_Renderer *renderer);
