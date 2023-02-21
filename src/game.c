#include "game.h"

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

    // Event loop
    while(!quit)
    {
        SDL_Event e;

        // Get available event
        while(SDL_PollEvent(&e))
        {
            // User requests quit
            if(e.type == SDL_QUIT)
            {
                quit = true;
                break;
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
