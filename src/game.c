#include "game.h"

// TODO:
// Load & init (Mode 2: load)
// Handle Victory

// More features:
// - Undo.
// - Save & Load.
// - Load a customized game (from text data).

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
        bool isInbound = Stack_isInbound(&(game->tableauPiles[pid]), atX, atY);
        if (isInbound) {
            return &(game->tableauPiles[pid]);
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

bool Game_initDisplay(Game *game) {
    // init deck on tableau
    game->deck.x1_coordinate = FIRST_PILE_X;
    game->deck.y1_coordinate = FIRST_PILE_Y - CARD_HEIGHT - STACK_DELTA;
    
    if (!Deck_initDisplay(&(game->deck)))
    {
        fprintf(stderr, "Deck fail to initialize !\n");
        return false;
    }

    // init fanned piles on tableau
    for (int pid = 0; pid < 7; pid++) {
        game->tableauPiles[pid].is_fanned = true;
        game->tableauPiles[pid].x1_coordinate = FIRST_PILE_X + pid * PILE_DISTANCE;
        game->tableauPiles[pid].y1_coordinate = FIRST_PILE_Y;

        if (!Stack_initDisplay(&(game->tableauPiles[pid])))
        {
            fprintf(stderr, "Stack fail to initialize !\n");
            fprintf(stderr, "%d\n", pid);
            return false;
        }
    }

    // init foundation piles
    for (int pid = 0; pid < 4; pid++) {
        game->foundationPiles[pid].x1_coordinate = game->tableauPiles[pid + 3].x1_coordinate;
        game->foundationPiles[pid].y1_coordinate = FIRST_PILE_Y - CARD_HEIGHT - STACK_DELTA;

        if (!Stack_initDisplay(&(game->foundationPiles[pid])))
        {
            fprintf(stderr, "Stack fail to initialize !\n");
            fprintf(stderr, "%d\n", pid);
            return false;
        }
    }
    return true;
}

bool Game_initialize(Game* game) {
    game->selectedCard = NULL;
    game->selectedStack = NULL;
    // shuffle_mode
    for (int i = 0; i < RANKS_COUNT; i++) {
        for (int j = 0; j < SUITS_COUNT; j++) {
            Card card;
            card._rank = i + 1;
            card._suit = j + 1;
            Card_flipDown(&card);
            Deck_pushCard(&(game->deck), &card);
        }
    }

    Deck_shuffle(&(game->deck));

    for (int pid = 0; pid < 7; pid++) {
        for (int cid = 0; cid <= pid; cid++) {
            int deckCount = game->deck.facedown.cards_count;
            Stack_pushCard(&(game->tableauPiles[pid]), &(game->deck.facedown._cards[deckCount - 1]));
            Stack_popCard(&(game->deck.facedown));
        }
        int tableauCardCount = game->tableauPiles[pid].cards_count;
        Card_flipUp(&(game->tableauPiles[pid]._cards[tableauCardCount - 1]));
    }

    game->gameInfo = "";
    game->gameInfoColor = COLOR_WHITE;

    return Game_initDisplay(game);
}


