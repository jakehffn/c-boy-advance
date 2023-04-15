#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "video/video.h"

#ifdef __GNUC__
    #define UNUSED __attribute__((__unused__))
#else
    #define UNUSED
#endif

bool quit = false;
SDL_Event event;

void pollEvents() {

    // Poll for events
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                // User clicked the close button, exit loop
                printf("Quit event received\n");
                quit = true;
                break;

            case SDL_WINDOWEVENT:

                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    glViewport(0, 0, event.window.data1, event.window.data2);
                }
                break;

            case SDL_KEYDOWN:
                // User pressed a key
                printf("Key pressed: %s\n", SDL_GetKeyName(event.key.keysym.sym));
                break;

            case SDL_MOUSEBUTTONDOWN:
                // User clicked the mouse button
                printf("Mouse button %d clicked at (%d,%d)\n", event.button.button, event.button.x, event.button.y);
                break;
                
            default:
                // Ignore other events
                break;
        }
    }
}

void mainLoop() {

    pollEvents();
    drawToCanvas();
}

int main(UNUSED int argv, UNUSED char** args) {

    if (!initVideo()) {

        printf("Failed to initialize video\n");

    } else {
        
        #ifdef __EMSCRIPTEN__

            printf("On the web!\n");

            emscripten_set_main_loop(mainLoop, 0, 1);

        #else

            while(!quit) {
                mainLoop();
            }

        #endif
    }  
    
    destroyVideo();

    return 0;
}