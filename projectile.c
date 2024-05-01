#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#define SCREEN_WIDTH_PX 1600
#define SCREEN_HEIGHT_PX 800
#define ACC_GRAVITY_MPS 9.81f

typedef struct _Mouse {
    int x;
    int y;
    uint32_t button;
} Mouse;

void getMouse(Mouse *mouse);
uint64_t ftoms(uint64_t frame, uint8_t fps);
void update(uint64_t frame, SDL_KeyCode key, Mouse *mouse);

int main(void)
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    { // SDL Initialization
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
            fprintf(stderr, "could not init video\n%s", SDL_GetError());

        window = SDL_CreateWindow("Projectile",
                                             SDL_WINDOWPOS_UNDEFINED,
                                             SDL_WINDOWPOS_UNDEFINED,
                                             SCREEN_WIDTH_PX, SCREEN_HEIGHT_PX,
                                             SDL_WINDOW_SHOWN);

        renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_SOFTWARE);
    } // SDL Initialization


    { // game loop
        bool quit = false;
        const uint8_t fps = 60;
        const float mspd = (1 / 60.0f) * 1000.0f;
        uint64_t frame = 0;

        while (!quit) {
            uint32_t loop_start = SDL_GetTicks();

            SDL_Event event;
            SDL_KeyCode key = 0;
            Mouse mouse;
            bool keydown = false;

            getMouse(&mouse);

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

            update(frame, key, &mouse);
            SDL_RenderPresent(renderer);
            frame++;

            uint32_t loop_end = SDL_GetTicks();
            uint32_t elapsed_time = loop_end - loop_start;

            uint32_t delay = ceilf(mspd - (float)elapsed_time);

            if (delay > 0) {
                SDL_Delay(delay);
            }

            //printf("%lu\n", ftoms(frame, fps));
            //printf("%d, %d\n", mouse.x, mouse.y);
        }

    } // game loop


    { // quit
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
    } // quit

}

void getMouse(Mouse * mouse)
{
    mouse->button = SDL_GetMouseState(&mouse->x, &mouse->y);
}

uint64_t ftoms(uint64_t frame, uint8_t fps)
    // frame to milliseconds
{
    return ((float)frame / (float)fps) * 1000;
}

void update(uint64_t frame, SDL_KeyCode key, Mouse *mouse)
{
    SDL_Delay(2);
}
