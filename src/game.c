#include "game.h"

// TODO:
// - move a card from one stack to other
// - move a chain of card from one stack to other

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
        int pileX = fannedPiles[pid].x_coordinate;
        int pileY = fannedPiles[pid].y_coordinate;
        int pileCount = fannedPiles[pid].cards_count;
        if (atX < pileX || atX > pileX + CARD_WIDTH) {
            continue;
        }

        if (atY < pileY || atY > pileY + CARD_HEIGHT + STACK_DELTA * pileCount) {
            continue;
        }

        return &fannedPiles[pid];
    }
    return NULL;
}

bool moveCardToStack(Card* card, Stack* pile) {
    return true;
}

bool Game_start(SDL_Renderer *renderer, int w, int h) {
    Stack fannedPiles[7] = {0};
    for (int pid = 0; pid < 7; pid++) {
        fannedPiles[pid].x_coordinate = FIRST_PILE_X + pid * PILE_DISTANCE;
        fannedPiles[pid].y_coordinate = FIRST_PILE_Y;
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
                if (selectedCard != NULL) Card_deselect(selectedCard);
                if (mbe.button == SDL_BUTTON_LEFT) {
                    // locate newly selected card:
                    selectedCard = locateSelectedCard(fannedPiles, mbe.x, mbe.y);
                    if (selectedCard != NULL) Card_select(selectedCard);
                } else if (mbe.button == SDL_BUTTON_RIGHT) {
                    fprintf(stderr, "Right click\n");
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
