#include "stack.h"

// This library only handles display logic, NOT game logic.
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
  card->borderColor.r = COLOR_GRAY.r;
  card->borderColor.g = COLOR_GRAY.g;
  card->borderColor.b = COLOR_GRAY.b;
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

  SDL_Texture* tex = NULL;
  if (card->isFaceDown) {
    tex = TextureHandler_getBack(&TEX_HANDLER);
  } else {
    tex = TextureHandler_getFront(&TEX_HANDLER, card->_suit, card->_rank);
  }
  SDL_RenderCopy(renderer, tex, NULL, &(card->rect));
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

void Card_flipUp(Card *card) {
  if (card == NULL) {
    fprintf(stderr, "Selected NULL value\n");
    return;
  }
  card->isFaceDown = false;
}

void Card_flipDown(Card *card) {
  if (card == NULL) {
    fprintf(stderr, "Selected NULL value\n");
    return;
  }
  card->isFaceDown = true;
}
