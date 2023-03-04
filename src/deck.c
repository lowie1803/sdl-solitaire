#include "deck.h"

bool Deck_initDisplay(Deck *deck) {
  if (!deck->x1_coordinate || !deck->y1_coordinate)
  {
    fprintf(stderr, "Deck dimensions not initialized !\n");
    return false;
  }

  deck->facedown.x1_coordinate = deck->x1_coordinate;
  deck->facedown.y1_coordinate = deck->y1_coordinate;
  Stack_initDisplay(&(deck->facedown));
  deck->faceup.x1_coordinate = deck->x1_coordinate + PILE_DISTANCE;
  deck->faceup.y1_coordinate = deck->y1_coordinate;
  Stack_initDisplay(&(deck->faceup));

  return true;
}

bool Deck_pushCard(Deck *deck, Card *card) {
  int ind = deck->facedown.cards_count;
  deck->facedown._cards[ind] = *card;
  deck->facedown.cards_count++;
  return true;
}

bool Deck_popCard(Deck *deck) {
  int ind = deck->faceup.cards_count;
  if (ind <= 0) return false;
  deck->faceup.cards_count--;
  return true;
}

bool Deck_flip(Deck *deck) {
  int ind = deck->facedown.cards_count;
  if (ind == 0) {
    int ind2 = deck->faceup.cards_count;
    if (ind2 == 0) return false;

    while (ind2 > 0) {
      ind2--;
      Card *card = &(deck->faceup._cards[ind2]);
      Stack_popCard(&(deck->faceup));
      Card_flipDown(card);
      Stack_pushCard(&(deck->facedown), card);
    }
  } else {
    Card *card = &(deck->facedown._cards[ind - 1]);
    Card_flipUp(card);
    Stack_pushCard(&(deck->faceup), card);
    Stack_popCard(&(deck->facedown));
  }
  Deck_initDisplay(deck);
  return true;
}

void Deck_render(Deck *deck, SDL_Renderer *renderer) {
  Stack_render(&(deck->facedown), renderer);
  Stack_render(&(deck->faceup), renderer);
}
