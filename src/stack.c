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
  card->border.w = CARD_WIDTH + CARD_BORDER_WIDTH * 2;
  card->border.h = CARD_HEIGHT + CARD_BORDER_WIDTH * 2;
  card->rectColor.r = SUIT_TO_COLOR[card->_suit].r / card->_rank;
  card->rectColor.g = SUIT_TO_COLOR[card->_suit].g / card->_rank;
  card->rectColor.b = SUIT_TO_COLOR[card->_suit].b / card->_rank;
  card->borderColor.r = COLOR_WHITE.r;
  card->borderColor.g = COLOR_WHITE.g;
  card->borderColor.b = COLOR_WHITE.b;
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
  card->rect.x = stack->x_coordinate;
  card->rect.y = stack->y_coordinate + offset * STACK_DELTA;
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

void Card_select(Card *card) {
  if (card == NULL) {
    fprintf(stderr, "Selected NULL value\n");
    return;
  }
  card->isSelected = true;
  card->borderColor.r = COLOR_GREEN.r;
  card->borderColor.g = COLOR_GREEN.g;
  card->borderColor.b = COLOR_GREEN.b;
}

void Card_deselect(Card *card) {
  if (card == NULL) {
    fprintf(stderr, "Deselected NULL value\n");
    return;
  }
  card->isSelected = false;
  card->borderColor.r = COLOR_WHITE.r;
  card->borderColor.g = COLOR_WHITE.g;
  card->borderColor.b = COLOR_WHITE.b;
}