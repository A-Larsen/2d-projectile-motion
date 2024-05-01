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

enum /* color */ {COLOR_RED, COLOR_GREEN, COLOR_BLUE, COLOR_ORANGE, COLOR_GREY, COLOR_WHITE, COLOR_BLACK, COLOR_SIZE};

void getMouse(Mouse *mouse);
uint64_t ftoms(uint64_t frame, uint8_t fps);
void update(SDL_Renderer *renderer, uint64_t frame, uint64_t time,
            SDL_KeyCode key, Mouse *mouse);
void
setColor(SDL_Renderer *renderer, uint8_t color);

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
        uint64_t time = 0;

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

            setColor(renderer, COLOR_BLACK);
            SDL_RenderClear(renderer);
            update(renderer, frame, time, key, &mouse);
            SDL_RenderPresent(renderer);
            frame++;
            time = ((float)frame / (float)fps) * 1000;

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

void update(SDL_Renderer *renderer, uint64_t frame, uint64_t time,
            SDL_KeyCode key, Mouse *mouse)
{
    SDL_Rect rect = {
        .x = mouse->x,
        .y = mouse->y,
        .w = 10,
        .h = 10
    };
    setColor(renderer, COLOR_BLUE);
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(2);
}

void
setColor(SDL_Renderer *renderer, uint8_t color)
{
    const SDL_Color colors[] = {
        [COLOR_RED] = {.r = 217, .g = 100, .b = 89, .a = 255},
        [COLOR_WHITE] = {.r = 255, .g = 255, .b = 255, .a = 255},
        [COLOR_GREEN] = {.r = 88, .g = 140, .b = 126, .a = 255},
        [COLOR_BLUE] = {.r = 146, .g = 161, .b = 185, .a = 255},
        [COLOR_ORANGE] = {.r = 242, .g = 174, .b = 114, .a = 255},
        [COLOR_GREY] = {.r = 89, .g = 89, .b = 89, .a = 89},
        [COLOR_BLACK] = {.r = 0, .g = 0, .b = 0, .a = 0},
    };

    SDL_SetRenderDrawColor(renderer, colors[color].r, colors[color].g,
                           colors[color].b, colors[color].a);
}
