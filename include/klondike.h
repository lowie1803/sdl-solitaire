// This library handles all Klondike mathematical logic
// Restrict movement and enforce that all moves are legal on board
#ifndef KLONDIKE_H
#define KLONDIKE_H

#include <stdio.h>
#include <stdbool.h>

#include "game.h"

bool Klondike_isTableau(Game *game, Stack *pile);
bool Klondike_isFoundation(Game *game, Stack *pile);
bool Klondike_isDeck(Game *game, Stack *pile);
bool Klondike_isContainedIn(Game *game, Stack *pile);
bool Klondike_isContainsCard(Stack *pile, Card *card);

// bool Klondike_isRed(Card *card);
bool Klondike_isDifferentColor(Card *card1, Card *card2);

// if top of des == null, return botofsrc.rank == King
// else: topofdes.rank == botofsrc+1 and isDiffColor(topofdes, botofsrc);
bool Klondike_isTableauMatched(Card *topOfDes, Card *botOfSrc);
// if top of des == null, return botofsrc.rank == Ace
// else: topofdes.rank == botofsrc-1
bool Klondike_isFoundationMatched(Card *topOfDes, Card *botOfSrc);

// To tableau: 
// - Des is empty, or the final card in des must be faced up
// - Check using isTableauMatched
// From tableau:
// - SrcCard, until the top card must be faced up
bool Klondike_isMovableTableauToTableau(Game *game, Card *srcCard, Stack *srcPile, Stack *desPile);
// From non_tableau:
// - Must be faced up, and on top of the pile.
bool Klondike_isMovableNonTableauToTableau(Game *game, Card *srcCard, Stack *srcPile, Stack *desPile);

// Is foundation matched(desPile.top, srcCard)
bool Klondike_isMovableToFoundation(Game *game, Card *srcCard, Stack *srcPile, Stack *desPile);

bool Klondike_isMovable(Game *game, Card *srcCard, Stack *srcPile, Stack *desPile);

bool Klondike_isVictory(Game *game);

#endif // KLONDIKE_H
