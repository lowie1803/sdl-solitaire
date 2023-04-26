#include "game_interface.h"

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
    if (Klondike_isMovable(game, game->selectedCard, game->selectedStack, targetStack)) {
        Game_moveCardBetweenStack(
            game->selectedCard,
            game->selectedStack,
            targetStack
        );
        Card_deselect(game->selectedCard);
        game->selectedStack = NULL;
        game->selectedCard = NULL;
        game->gameInfo = "";
        game->gameInfoColor = COLOR_WHITE;
    } else {
        game->gameInfo = "Not allowed!";
        game->gameInfoColor = COLOR_RED;
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
            Stack_render(&(g.tableauPiles[pid]), renderer);
        }
        for (int pid = 0; pid < 4; pid++) {
            Stack_render(&(g.foundationPiles[pid]), renderer);
        }

        for (int i = 0; i < 4; i++) {
            stringRGBA(renderer, 20, 520 + i * 20,
                INSTRUCTIONS[i],
                COLOR_LIGHT_GRAY.r, COLOR_LIGHT_GRAY.g, COLOR_LIGHT_GRAY.b, COLOR_LIGHT_GRAY.a);

        }
        
        if (Klondike_isVictory(&g)) {
            g.gameInfo = "Victory!";
            g.gameInfoColor = COLOR_GREEN;
        }
        stringRGBA(renderer, 20, 20,
            "INFO: ",
            g.gameInfoColor.r, g.gameInfoColor.g, g.gameInfoColor.b, g.gameInfoColor.a);
        stringRGBA(renderer, 100, 20,
            (g.gameInfo),
            g.gameInfoColor.r, g.gameInfoColor.g, g.gameInfoColor.b, g.gameInfoColor.a);
        
        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }

    return true;
}