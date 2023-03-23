#include "card.h"

// This library only handles display & moving logic, NOT game logic.
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


bool Card_isRed(Card *card) {
  return (card->_suit == DIAMOND || card->_suit == HEART);
}

bool Card_isDiffColor(Card *card1, Card *card2) {
  return (Card_isRed(card1) != Card_isRed(card2));
}

bool Card_isDiffColor(Card *card1, Card *card2);

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

void Card_render(Card *card, SDL_Renderer *renderer) {
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
