#include <SDL.h>

#include <cstdio>
#include <iostream>
#include <thread>

#include "../include/core.h"
#include "../include/renderEngine.h"

renderEngine* render = nullptr;

Uint32 frameStart;
int currentTickTime;

// Entrypoint
int main(int argc, char* args[])
{
    // Engines
    render = new renderEngine();

    // Start
    render->Initialise("Rank Adjuster", 1280, 720);

    // Tick loop
    while (render->Running()) {
        // Start tick time
        frameStart = SDL_GetTicks();

        render->Update();
        render->Render();

        // Check for delays
        currentTickTime = SDL_GetTicks() - frameStart;
        if (D_TICKRATE_TIME > currentTickTime) {
            SDL_Delay(D_TICKRATE_TIME - currentTickTime);
        } else {
            std::cout << "Tickrate lagging: ";
            std::cout << (currentTickTime - D_TICKRATE_TIME);
            std::cout << "ms behind!" << std::endl;
        }
    }
    // Clean
    render->Clean();
    return 0;
}