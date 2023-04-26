#include "klondike.h"

bool Klondike_isTableau(Game *game, Stack *pile) {
  // bool result = false;
  for (int i = 0; i < 7; i++) {
    if (pile == &(game->tableauPiles[i])) {
      return true;
    }
  }
  return false;
}

bool Klondike_isFoundation(Game *game, Stack *pile) {
  for (int i = 0; i < 4; i++) {
    if (pile == &(game->foundationPiles[i])) {
      return true;
    }
  }
  return false;
}

bool Klondike_isDeck(Game *game, Stack *pile) {
  return (pile == &(game->deck.faceup));
}

bool Klondike_isContainedIn(Game *game, Stack *pile) {
  return (
    Klondike_isTableau(game, pile) ||
    Klondike_isFoundation(game, pile) ||
    Klondike_isDeck(game, pile)
  );
}

bool Klondike_isContainsCard(Stack *pile, Card *card) {
  for (int i = 0; i < pile->cards_count; i++) {
    if (card == &(pile->_cards[i])) {
      return true;
    }
  }
  return false;
}

bool Klondike_isTableauMatched(Card *topOfDes, Card *botOfSrc) {
  if (topOfDes == NULL) {
    return botOfSrc->_rank == KING;
  }
  if (topOfDes->isFaceDown) return false;

  return (
    topOfDes->_rank == botOfSrc->_rank + 1
    && Card_isDiffColor(topOfDes, botOfSrc)
  );
}

bool Klondike_isFoundationMatched(Card *topOfDes, Card *botOfSrc) {
  if (topOfDes == NULL) {
    return botOfSrc->_rank == ACE;
  }
  return (
    topOfDes->_rank == botOfSrc->_rank - 1
    && topOfDes->_suit == botOfSrc->_suit
  );
}

bool Klondike_isMovableTableauToTableau(Game *game, Card *srcCard, Stack *srcPile, Stack *desPile) {
  if (
    !Klondike_isTableau(game, srcPile) ||
    !Klondike_isTableau(game, desPile) ||
    !Klondike_isContainsCard(srcPile, srcCard) ||
    srcPile == desPile
  ) {
    return false;
  }
  int desCardCount = desPile->cards_count;
  Card *topOfDes = (desCardCount > 0 ? &(desPile->_cards[desCardCount - 1]) : NULL);
  return Klondike_isTableauMatched(topOfDes, srcCard);
}
// From non_tableau:
// - Must be faced up, and on top of the pile.
// - Need to implement top of deck
bool Klondike_isMovableNonTableauToTableau(Game *game, Card *srcCard, Stack *srcPile, Stack *desPile) {
  if (
    !Klondike_isContainedIn(game, srcPile) ||
    !Klondike_isTableau(game, desPile) ||
    !Klondike_isContainsCard(srcPile, srcCard) ||
    srcPile == desPile
  ) {
    return false;
  }
  int desCardCount = desPile->cards_count;
  Card *topOfDes = (desCardCount > 0 ? &(desPile->_cards[desCardCount - 1]) : NULL);
  return Klondike_isTableauMatched(topOfDes, srcCard);
}

// Is foundation matched(desPile.top, srcCard)
bool Klondike_isMovableToFoundation(Game *game, Card *srcCard, Stack *srcPile, Stack *desPile) {
  if (
    !Klondike_isContainedIn(game, srcPile) ||
    !Klondike_isFoundation(game, desPile) ||
    !Klondike_isContainsCard(srcPile, srcCard) ||
    srcPile == desPile
  ) {
    return false;
  }
  int desCardCount = desPile->cards_count;
  Card *topOfDes = (desCardCount > 0 ? &(desPile->_cards[desCardCount - 1]) : NULL);
  return Klondike_isFoundationMatched(topOfDes, srcCard);
}

bool Klondike_isMovable(Game *game, Card *srcCard, Stack *srcPile, Stack *desPile) {
  if (Klondike_isFoundation(game, desPile)) {
    return Klondike_isMovableToFoundation(game, srcCard, srcPile, desPile);
  }

  if (Klondike_isTableau(game, desPile)) {
    if (Klondike_isTableau(game, srcPile)) {
      return Klondike_isMovableTableauToTableau(game, srcCard, srcPile, desPile);
    }
    else {
      return Klondike_isMovableNonTableauToTableau(game, srcCard, srcPile, desPile);
    }
  }
  return false;
}

bool Klondike_isVictory(Game *game) {
  for (int pid = 0; pid < 4; pid++) {
    if (game->foundationPiles[pid].cards_count != 13) {
      return false;
    }
    
    for (int cid = 1; cid < 13; cid++) {
      if (game->foundationPiles[pid]._cards[cid]._suit != game->foundationPiles[pid]._cards[cid - 1]._suit) {
        return false;
      }
    }
  }
  return true;
}
