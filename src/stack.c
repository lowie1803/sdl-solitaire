#include "stack.h"

// This library only handles display & moving logic, NOT game logic.
bool Stack_pushCard(Stack *stack, Card *card) {
  int ind = stack->cards_count;
  stack->_cards[ind] = *card;
  stack->cards_count++;
  return true;
}

bool Stack_popCard(Stack *stack) {
  int ind = stack->cards_count;
  if (ind <= 0) return false;
  stack->cards_count--;
  return true;
}

bool Stack_initDisplay(Stack *stack)
{
  if (!stack->x1_coordinate || !stack->y1_coordinate)
  {
    fprintf(stderr, "Stack dimensions not initialized !\n");
    return false;
  }

  if (stack->cards_count < 0 || stack->cards_count > MAX_CARDS_PER_STACK) {
    fprintf(stderr, "Stack counts is not appropriate !\n");
    return false;
  }

  stack->x2_coordinate = stack->x1_coordinate + CARD_WIDTH;
  stack->y2_coordinate = stack->y1_coordinate + (stack->cards_count - 1) * STACK_DELTA * stack->is_fanned + CARD_HEIGHT;

  // Init all cards
  for (int i = 0; i < stack->cards_count; i++) {
    Stack_initCardDisplay(
      stack,
      &(stack->_cards[i]),
      i
    );
  }

  return true;
}

bool Stack_isInbound(Stack *stack, int atX, int atY) {
  return (
    atX >= stack->x1_coordinate &&
    atX <= stack->x2_coordinate &&
    atY >= stack->y1_coordinate &&
    atY <= stack->y2_coordinate
  );
}


void Stack_initCardDisplay(Stack *stack, Card *card, int offset) {
  Card_initDisplay(card);
  card->rect.x = stack->x1_coordinate;
  card->rect.y = stack->y1_coordinate + offset * STACK_DELTA * stack->is_fanned;
  card->border.x = card->rect.x - CARD_BORDER_WIDTH;
  card->border.y = card->rect.y - CARD_BORDER_WIDTH;
}

void Stack_render(Stack *stack, SDL_Renderer *renderer) 
{
  if (stack->cards_count == 0) {
    SDL_SetRenderDrawColor(
      renderer,
      COLOR_WHITE.r,
      COLOR_WHITE.g,
      COLOR_WHITE.b,
      COLOR_WHITE.a
    );
    SDL_Rect mt;
    mt.x = stack->x1_coordinate - CARD_BORDER_WIDTH;
    mt.y = stack->y1_coordinate - CARD_BORDER_WIDTH;
    mt.w = CARD_WIDTH + CARD_BORDER_WIDTH * 2;
    mt.h = CARD_HEIGHT + CARD_BORDER_WIDTH * 2;
    SDL_RenderFillRect(renderer, &mt);

    SDL_SetRenderDrawColor(
      renderer,
      COLOR_DARK_GRAY.r,
      COLOR_DARK_GRAY.g,
      COLOR_DARK_GRAY.b,
      COLOR_DARK_GRAY.a
    );
    mt.x = stack->x1_coordinate;
    mt.y = stack->y1_coordinate;
    mt.w = CARD_WIDTH;
    mt.h = CARD_HEIGHT;
    SDL_RenderFillRect(renderer, &mt);
  } else {
    for (int i = 0; i < stack->cards_count; i++) {
      Card_render(&(stack->_cards[i]), renderer);
    }
  }
}
