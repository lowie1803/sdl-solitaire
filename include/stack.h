#ifndef GRID_H
#define GRID_H

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
#define CARD_HEIGHT (60)
#define CARD_WIDTH (40)
#define STACK_DELTA (20)

// const SDL_Color EMPTY_STACK_BORDER = {255, 255, 255, 255};

struct Card
{
  enum suit _suit;
  enum rank _rank;

  SDL_Rect rect;
  SDL_Color rectColor;
  
  SDL_Rect border;
  SDL_Color borderColor;
};
typedef struct Card Card;

struct Stack
{
  Card _cards[MAX_CARDS_PER_STACK];

  int x_coordinate;
  int y_coordinate;
  int cards_count;
};
typedef struct Stack Stack;

bool Stack_pushCard(Stack *stack, Card *card);

bool Card_initDisplay(Card *card);
void Stack_initCardDisplay(Stack *stack, Card *card, int i);
bool Stack_initDisplay(Stack *stack);

void Stack_render(Stack *stack, SDL_Renderer *renderer);
void Stack_renderCard(Card *card, SDL_Renderer *renderer);

#endif
