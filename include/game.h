/*
 * Copyright (c) 2018, 2019 Amine Ben Hassouna <amine.benhassouna@gmail.com>
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any
 * person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the
 * Software without restriction, including without
 * limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice
 * shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
 * ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
 * SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 * IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL2_framerate.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

#include "stack.h"
#include "deck.h"
#include "utils.h"
// #include "klondike.h"

struct Game {
  Deck deck;
  Stack tableauPiles[7];
  Stack foundationPiles[4];

  Card* selectedCard;
  Stack* selectedStack;

  char* gameInfo;
  SDL_Color gameInfoColor;
};
typedef struct Game Game;


Card* Game_locateCard(Game* game, int atX, int atY);
Stack* Game_locateStack(Game* game, int atX, int atY);
bool Game_moveCardBetweenStack(Card* card, Stack* pileFrom, Stack *pileTo);
bool Game_initDisplay(Game* game);
bool Game_initialize(Game* game);



#endif // GAME_H
