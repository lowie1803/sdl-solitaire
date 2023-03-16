#include "game.h"

// TODO:
// Refactor to have module "Card"
// Refactor to handle click, drag
// Handle game logic
// - Implement game logic
// - Note that game logic is integrated in some of this code.
// But most of them only handle moving around currently.

// The first thing to do is to randomly init 52 distinct cards, with 28
// cards on the tableau, and the rest in the deck.

// Next is to handle what can be stacked on each other in each fanned pile,
// and what can only be put on empty fanned pile.

// Next is to handle what can be put on foundation piles.

// Last thing to do is to handle victory, and new game.

// More features:
// - Undo.
// - Save & Load.
// - Load a customized game (from text data).

// This library handles game logic.
Card* Game_locateCard(Game* game, int atX, int atY) {
    Stack* selectedPile = Game_locateStack(game, atX, atY);
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

Stack* Game_locateStack(Game* game, int atX, int atY) {
    if (Stack_isInbound(&(game->deck.facedown), atX, atY)) {
        return &(game->deck.facedown);
    }
    if (Stack_isInbound(&(game->deck.faceup), atX, atY)) {
        return &(game->deck.faceup);
    }
    for (int pid = 0; pid < 7; pid++) {
        bool isInbound = Stack_isInbound(&(game->fannedPiles[pid]), atX, atY);
        if (isInbound) {
            return &(game->fannedPiles[pid]);
        }
    }

    for (int pid = 0; pid < 4; pid++) {
        bool isInbound = Stack_isInbound(&(game->foundationPiles[pid]), atX, atY);
        if (isInbound) {
            return &(game->foundationPiles[pid]);
        }
    }
    return NULL;
}

bool Game_moveCardBetweenStack(Card* card, Stack* pileFrom, Stack* pileTo) {
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

bool Game_initialize(Game* game) {
    game->selectedCard = NULL;
    game->selectedStack = NULL;
    // init deck on tableau
    game->deck.x1_coordinate = FIRST_PILE_X;
    game->deck.y1_coordinate = FIRST_PILE_Y - CARD_HEIGHT - STACK_DELTA;
    for (int cid = 0; cid < 15; cid++) {
        Card temp;
        temp._suit = rand() % 4 + 1;
        temp._rank = rand() % 13 + 1;
        temp.isFaceDown = true;
        Deck_pushCard(&(game->deck), &temp);
    }
    if (!Deck_initDisplay(&(game->deck)))
    {
        fprintf(stderr, "Deck fail to initialize !\n");
        return false;
    }

    // init fanned piles on tableau
    for (int pid = 0; pid < 7; pid++) {
        game->fannedPiles[pid].is_fanned = true;
        game->fannedPiles[pid].x1_coordinate = FIRST_PILE_X + pid * PILE_DISTANCE;
        game->fannedPiles[pid].y1_coordinate = FIRST_PILE_Y;
        for (int cid = 0; cid <= pid; cid++) {
            Card temp;
            temp._suit = rand() % 4 + 1;
            temp._rank = rand() % 13 + 1;
            if (cid < pid) {
                temp.isFaceDown = true;
            } else {
                temp.isFaceDown = false;
            }
            Stack_pushCard(&(game->fannedPiles[pid]), &temp);
        }

        if (!Stack_initDisplay(&(game->fannedPiles[pid])))
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

        if (!Stack_initDisplay(&(game->foundationPiles[pid])))
        {
            fprintf(stderr, "Stack fail to initialize !\n");
            fprintf(stderr, "%d\n", pid);
            return false;
        }
    }
    return true;
}

void Game_selectInteraction(Game* game, int atX, int atY) {
    if (game->selectedCard != NULL) {
        Card_deselect(game->selectedCard);
    }
    Stack* atStack = Game_locateStack(game, atX, atY);
    if (atStack == NULL) {
        return;
    }
    if (atStack == &(game->deck.facedown)) {
        Deck_flip(&(game->deck));
        return;
    }
    if (atStack == &(game->deck.faceup)) {
        if (game->deck.faceup.cards_count == 0) {
            return;
        }
        Card *card = &(game->deck.faceup._cards[game->deck.faceup.cards_count - 1]);
        game->selectedCard = card;
        game->selectedStack = atStack;
        Card_select(card);
        return;
    }
    // locate newly selected card on tableau:
    game->selectedCard = Game_locateCard(game, atX, atY);
    game->selectedStack = atStack;
    if (game->selectedCard != NULL) {
        if (game->selectedCard->isFaceDown) {
            Card* c = game->selectedCard;
            Stack* s = game->selectedStack;

            // Only let flip up when card on top of stack
            if (c == &s->_cards[s->cards_count - 1]) {
                Card_flipUp(c);
            }
            game->selectedCard = NULL;
            game->selectedStack = NULL;
        } else {
            Card_select(game->selectedCard);
        }
    }
}

void Game_moveInteraction(Game* game, int atX, int atY) {
    Stack *targetStack = Game_locateStack(game, atX, atY);
    if (targetStack == &(game->deck.facedown) || targetStack == &(game->deck.faceup)) {
        fprintf(stderr, "Not allowed!");
        return;
    }
    if (game->selectedCard != NULL && targetStack != NULL) {
        Game_moveCardBetweenStack(
            game->selectedCard,
            game->selectedStack,
            targetStack
        );
        Card_deselect(game->selectedCard);
        game->selectedStack = NULL;
        game->selectedCard = NULL;
    } else {
        fprintf(stderr, "No card selected!\n");
    }

}

bool Game_start(SDL_Renderer *renderer, int w, int h) {
    Game g = {0};
    bool initial_success = Game_initialize(&g);
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
                    Game_selectInteraction(&g, mbe.x, mbe.y);
                } else if (mbe.button == SDL_BUTTON_RIGHT) {
                    Game_moveInteraction(&g, mbe.x, mbe.y);
                }
            }
        }

        // Set background color
        Utils_setBackgroundColor(renderer, COLOR_DARK_GRAY);

        // Render piles
        Deck_render(&(g.deck), renderer);
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
