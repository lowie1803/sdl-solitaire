#include "game.h"

// TODO:
// - Implement deck and victory stacks
// - Implement zero-delta stacks
// - Implement drawable stacks
// - Implement card-draw features.

Card* locateSelectedCard(Game* game, int atX, int atY) {
    Stack* selectedPile = locateSelectedStack(game, atX, atY);
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

Stack* locateSelectedStack(Game* game, int atX, int atY) {
    for (int pid = 0; pid < 7; pid++) {
        int pileX1 = game->fannedPiles[pid].x1_coordinate;
        int pileX2 = game->fannedPiles[pid].x2_coordinate;
        int pileY1 = game->fannedPiles[pid].y1_coordinate;
        int pileY2 = game->fannedPiles[pid].y2_coordinate;
        if (atX < pileX1 || atX > pileX2) {
            continue;
        }

        if (atY < pileY1 || atY > pileY2) {
            continue;
        }

        return &(game->fannedPiles[pid]);
    }

    for (int pid = 0; pid < 4; pid++) {
        int pileX1 = game->foundationPiles[pid].x1_coordinate;
        int pileX2 = game->foundationPiles[pid].x2_coordinate;
        int pileY1 = game->foundationPiles[pid].y1_coordinate;
        int pileY2 = game->foundationPiles[pid].y2_coordinate;
        if (atX < pileX1 || atX > pileX2) {
            continue;
        }

        if (atY < pileY1 || atY > pileY2) {
            continue;
        }

        return &(game->foundationPiles[pid]);
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

bool initGame(Game* game) {
    // init fanned piles on tableau
    for (int pid = 0; pid < 7; pid++) {
        game->fannedPiles[pid].is_fanned = true;
        game->fannedPiles[pid].x1_coordinate = FIRST_PILE_X + pid * PILE_DISTANCE;
        game->fannedPiles[pid].y1_coordinate = FIRST_PILE_Y;
        for (int cid = 0; cid <= pid; cid++) {
            Card temp;
            temp._suit = rand() % 4 + 1;
            temp._rank = rand() % 13 + 1;
            Stack_pushCard(&(game->fannedPiles[pid]), &temp);
        }

        if(!Stack_initDisplay(&(game->fannedPiles[pid])))
        {
            fprintf(stderr, "Stack fail to initialize !\n");
            fprintf(stderr, "%d\n", pid);
            return false;
        }
    }

    // init foundation piles
    for (int pid = 0; pid < 4; pid++) {
        game->foundationPiles[pid].x1_coordinate = game->fannedPiles[pid + 3].x1_coordinate;
        game->foundationPiles[pid].y1_coordinate = FIRST_PILE_Y - CARD_HEIGHT - STACK_DELTA;

        for (int cid = 0; cid <= 0; cid++) {
            Card temp;
            temp._suit = pid+1;
            temp._rank = 1;
            Stack_pushCard(&(game->foundationPiles[pid]), &temp);
        }

        if(!Stack_initDisplay(&(game->foundationPiles[pid])))
        {
            fprintf(stderr, "Stack fail to initialize !\n");
            fprintf(stderr, "%d\n", pid);
            return false;
        }
    }
    return true;
}

bool Game_start(SDL_Renderer *renderer, int w, int h) {
    Game g = {0};
    bool initial_success = initGame(&g);
    if (!initial_success) {
        fprintf(stderr, "Game not init successfully\n");
        fprintf(stderr, "%d%d\n", w, h);
        return false;
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
                    selectedCard = locateSelectedCard(&g, mbe.x, mbe.y);
                    selectedStack = locateSelectedStack(&g, mbe.x, mbe.y);
                    if (selectedCard != NULL) {
                        Card_select(selectedCard);
                    }
                } else if (mbe.button == SDL_BUTTON_RIGHT) {
                    // fprintf(stderr, "Right click\n");
                    Stack *targetStack = locateSelectedStack(&g, mbe.x, mbe.y);
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

        // Render piles
        for (int pid = 0; pid < 7; pid++) {
            Stack_render(&(g.fannedPiles[pid]), renderer);
        }
        for (int pid = 0; pid < 4; pid++) {
            Stack_render(&(g.foundationPiles[pid]), renderer);
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
