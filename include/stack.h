#ifndef GRID_H
#define GRID_H

#include "card.h"

#define STACK_DELTA (25)

#define FIRST_PILE_X (80)
#define FIRST_PILE_Y (200)
#define PILE_DISTANCE (95)

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

void Stack_initCardDisplay(Stack *stack, Card *card, int i);
bool Stack_initDisplay(Stack *stack);
bool Stack_isInbound(Stack *stack, int atX, int atY);

void Stack_render(Stack *stack, SDL_Renderer *renderer);


#endif