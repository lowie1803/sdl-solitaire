#include "stack.h"

bool Stack_pushCard(Stack *stack, Card *card) {
  int ind = stack->cards_count;
  stack->_cards[ind] = *card;
  stack->cards_count++;
  return true;
}

bool Card_initDisplay(Card *card)
{
  if (!card->_suit || !card->_rank)
  {
    fprintf(stderr, "Card not initialized!\n");
    return false;
  }

  card->rect.w = CARD_WIDTH;
  card->rect.h = CARD_HEIGHT;
  card->border.w = CARD_WIDTH;
  card->border.h = CARD_HEIGHT;
  card->rectColor.r = rand() % 256;
  card->rectColor.g = rand() % 256;
  card->rectColor.b = rand() % 256;
  return true;
}

bool Stack_initDisplay(Stack *stack)
{
  if (!stack->x_coordinate || !stack->y_coordinate)
  {
    fprintf(stderr, "Stack dimensions not initialized !\n");
    return false;
  }

  if (stack->cards_count < 0 || stack->cards_count > MAX_CARDS_PER_STACK) {
    fprintf(stderr, "Stack counts is not appropriate !\n");
    return false;
  }

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

void Stack_initCardDisplay(Stack *stack, Card *card, int offset) {
  Card_initDisplay(card);
  card->rect.x = stack->x_coordinate + offset * STACK_DELTA;
  card->rect.y = stack->y_coordinate;
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
  } else {
    for (int i = 0; i < stack->cards_count; i++) {
      Stack_renderCard(&(stack->_cards[i]), renderer);
    }
  }
}

void Stack_renderCard(Card *card, SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(
    renderer,
    card->borderColor.r,
    card->borderColor.g,
    card->borderColor.b,
    card->borderColor.a
  );
  SDL_RenderFillRect(renderer, &(card->border));

  SDL_SetRenderDrawColor(
    renderer,
    card->rectColor.r,
    card->rectColor.g,
    card->rectColor.b,
    card->rectColor.a
  );
  SDL_RenderFillRect(renderer, &(card->rect));
}
