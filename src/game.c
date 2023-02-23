#include "game.h"

// TODO:
// - Implement deck and victory stacks
// - Implement zero-delta stacks
// - Implement drawable stacks
// - Implement card-draw features.

Card* locateSelectedCard(Stack* fannedPiles, int atX, int atY) {
    Stack* selectedPile = locateSelectedStack(fannedPiles, atX, atY);
    if (selectedPile == NULL) return NULL;

    for (int cid = selectedPile->cards_count - 1; cid > -1; cid--) {
        Card *c = &(selectedPile->_cards[cid]);
        if (atY < c->rect.y || atY > c->rect.y + CARD_HEIGHT) {
            continue;
        }

        return c;
    }
    return NULL;
}

Stack* locateSelectedStack(Stack* fannedPiles, int atX, int atY) {
    for (int pid = 0; pid < 7; pid++) {
        int pileX1 = fannedPiles[pid].x1_coordinate;
        int pileX2 = fannedPiles[pid].x2_coordinate;
        int pileY1 = fannedPiles[pid].y1_coordinate;
        int pileY2 = fannedPiles[pid].y2_coordinate;
        if (atX < pileX1 || atX > pileX2) {
            continue;
        }

        if (atY < pileY1 || atY > pileY2) {
            continue;
        }

        return &fannedPiles[pid];
    }
    return NULL;
}

bool moveCardToStack(Card* card, Stack* pileFrom, Stack* pileTo) {
    if (pileFrom == pileTo) {
        fprintf(stderr, "Same pile!");
        return false;
    }

    int cardInd = 0;
    while (cardInd < pileFrom->cards_count) {
        if (&(pileFrom->_cards[cardInd]) == card) {
            break;
        }
        cardInd++;
    }

    if (cardInd == pileFrom->cards_count) {
        fprintf(stderr, "Card not found in pileFrom!");
        return false;
    }

    for (int cind = cardInd; cind < pileFrom->cards_count; cind++) {
        Stack_pushCard(pileTo, &(pileFrom->_cards[cind]));
    }

    while (cardInd < pileFrom->cards_count) {
        Stack_popCard(pileFrom);
    }

    // re-assign display
    Stack_initDisplay(pileFrom);
    Stack_initDisplay(pileTo);

    return true;
}

bool Game_start(SDL_Renderer *renderer, int w, int h) {
    Stack fannedPiles[7] = {0};
    for (int pid = 0; pid < 7; pid++) {
        fannedPiles[pid].is_fanned = true;
        fannedPiles[pid].x1_coordinate = FIRST_PILE_X + pid * PILE_DISTANCE;
        fannedPiles[pid].y1_coordinate = FIRST_PILE_Y;
        for (int cid = 0; cid <= pid; cid++) {
            Card temp;
            temp._suit = rand() % 4 + 1;
            temp._rank = rand() % 13 + 1;
            Stack_pushCard(&fannedPiles[pid], &temp);
        }

        if(!Stack_initDisplay(&fannedPiles[pid]))
        {
            fprintf(stderr, "Stack fail to initialize !\n");
            fprintf(stderr, "%d%d\n", w, h);
            fprintf(stderr, "%d\n", pid);
            return false;
        }
    }

    // Initialize framerate manager : 30 FPS
    FPSmanager fpsmanager;
    SDL_initFramerate(&fpsmanager);
    SDL_setFramerate(&fpsmanager, 30);

    // // Initialize start time (in ms)
    // long long last = Utils_time();

    // Event loop exit flag
    bool quit = false;

    Card *selectedCard = NULL;
    Stack *selectedStack = NULL;

    // Event loop
    while(!quit)
    {
        SDL_Event e;

        // Get available event
        while(SDL_PollEvent(&e))
        {
            // User requests quit
            if (e.type == SDL_QUIT)
            {
                quit = true;
                break;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_MouseButtonEvent mbe = e.button;
                if (mbe.button == SDL_BUTTON_LEFT) {
                    if (selectedCard != NULL) {
                        Card_deselect(selectedCard);
                        // selectedCard = NULL;
                    }
                    // locate newly selected card:
                    selectedCard = locateSelectedCard(fannedPiles, mbe.x, mbe.y);
                    selectedStack = locateSelectedStack(fannedPiles, mbe.x, mbe.y);
                    if (selectedCard != NULL) {
                        Card_select(selectedCard);
                    }
                } else if (mbe.button == SDL_BUTTON_RIGHT) {
                    // fprintf(stderr, "Right click\n");
                    Stack *targetStack = locateSelectedStack(fannedPiles, mbe.x, mbe.y);
                    if (selectedCard != NULL && targetStack != NULL) {
                        moveCardToStack(selectedCard, selectedStack, targetStack);
                        Card_deselect(selectedCard);
                        selectedStack = NULL;
                        selectedCard = NULL;
                    } else {
                        fprintf(stderr, "No card selected!\n");
                    }
                }
            }
        }

        // Set background color
        Utils_setBackgroundColor(renderer, COLOR_DARK_GRAY);

        // Render grid
        for (int pid = 0; pid < 7; pid++) {
            Stack_render(&fannedPiles[pid], renderer);
        }

        // Show message
        stringRGBA(renderer, 20, 20,
                   "This is the Solitaire game display model",
                   COLOR_LIGHT_GRAY.r, COLOR_LIGHT_GRAY.g, COLOR_LIGHT_GRAY.b, COLOR_LIGHT_GRAY.a);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }

    return true;
}
