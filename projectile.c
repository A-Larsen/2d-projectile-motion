#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define SCREEN_WIDTH_PX 1600
#define SCREEN_HEIGHT_PX 800
#define ACC_GRAVITY_MPS 9.81f

uint64_t ftoms(uint64_t frame, uint8_t fps)
    // frame to milliseconds
{
    return ((float)frame / (float)fps) * 1000;
}

void update(SDL_KeyCode key) {
    SDL_Delay(2);
}

int main(void)
{
    SDL_Window * window;
    SDL_Renderer * renderer;

    { // SDL Initialization
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            fprintf(stderr, "could not init video\n%s", SDL_GetError());

        window = SDL_CreateWindow("Projectile",
                                             SDL_WINDOWPOS_UNDEFINED,
                                             SDL_WINDOWPOS_UNDEFINED,
                                             SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX,
                                             SDL_WINDOW_SHOWN);

        renderer = SDL_CreateRenderer(window, 0,
                                                SDL_RENDERER_SOFTWARE);
    } // SDL Initialization


    { // game loop
        bool quit = false;
        uint8_t fps = 60;
        float mspd = (1 / 60.0f) * 1000.0f;
        uint64_t frame = 0;

        while (!quit) {
            uint32_t loop_start = SDL_GetTicks();

            SDL_Event event;
            SDL_KeyCode key = 0;
            bool keydown = false;

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_KEYDOWN: {
                    if (event.key.repeat == 0) {
                        key = event.key.keysym.sym;
                        keydown = true;
                    }
                    break;
                }
                case SDL_KEYUP: keydown = false; break;
                case SDL_QUIT: quit = true; break;
                }
            }

            update(key);
            SDL_RenderPresent(renderer);
            frame++;

            uint32_t loop_end = SDL_GetTicks();
            uint32_t elapsed_time = loop_end - loop_start;

            uint32_t delay = ceilf(mspd - (float)elapsed_time);

            if (delay > 0) {
                SDL_Delay(delay);
            }

            printf("%lu\n", ftoms(frame, fps));
        }

    } // game loop


    { // quit
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    } // quit

}
