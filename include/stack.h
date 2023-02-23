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
#define CARD_HEIGHT (90)
#define CARD_WIDTH (60)
#define CARD_BORDER_WIDTH (3)
#define STACK_DELTA (25)

#define FIRST_PILE_X (80)
#define FIRST_PILE_Y (200)
#define PILE_DISTANCE (95)

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

struct Stack
{
  Card _cards[MAX_CARDS_PER_STACK];

  int x1_coordinate;
  int y1_coordinate;
  int cards_count;
  bool is_fanned;

  // Will be initialized in Stack_initDisplay()
  int x2_coordinate;
  int y2_coordinate;
};
typedef struct Stack Stack;

bool Stack_pushCard(Stack *stack, Card *card);
bool Stack_popCard(Stack *stack);

bool Card_initDisplay(Card *card);
void Stack_initCardDisplay(Stack *stack, Card *card, int i);
bool Stack_initDisplay(Stack *stack);

void Stack_render(Stack *stack, SDL_Renderer *renderer);
void Stack_renderCard(Card *card, SDL_Renderer *renderer);

void Card_select(Card *card);
void Card_deselect(Card *card);

#endif
