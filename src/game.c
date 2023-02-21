#include "game.h"

bool Game_start(SDL_Renderer *renderer, int w, int h) {
    Stack _stack = {0};
    Card sample1, sample2;
    sample1._suit = HEART;
    sample1._rank = EIGHT;
    sample2._suit = DIAMOND;
    sample2._rank = ACE;
    //TODO:
    // - Display 2 stacks of cards.
    // - Move a card on top of stack to the other one.

    // Set stack coordinates
    _stack.x_coordinate = 50;
    _stack.y_coordinate = 50;
    fprintf(stderr, "init good");
    Stack_pushCard(&_stack, &sample1);
    fprintf(stderr, "push good");
    Stack_pushCard(&_stack, &sample2);

    if(!Stack_initDisplay(&_stack))
    {
        fprintf(stderr, "Stack fail to initialize !\n");
        fprintf(stderr, "%d%d", w, h);
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
            if(e.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
        }

        // Set background color
        Utils_setBackgroundColor(renderer, COLOR_DARK_GRAY);

        // Render grid
        Stack_render(&_stack, renderer);

        // Show message
        stringRGBA(renderer, 20, 20,
                   "This is a falling brick < Press RIGTH and LEFT to move >",
                   COLOR_LIGHT_GRAY.r, COLOR_LIGHT_GRAY.g, COLOR_LIGHT_GRAY.b, COLOR_LIGHT_GRAY.a);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay
        SDL_framerateDelay(&fpsmanager);
    }

    return true;
}
