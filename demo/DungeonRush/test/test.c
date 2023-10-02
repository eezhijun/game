#include <stdio.h>

#include <SDL.h>
#include <SDL_mixer.h>

void sdl_test(void)
{
    int numdrivers = SDL_GetNumRenderDrivers();
    int i = 0;
    printf("Render driver count:%d.\n", numdrivers);
    for (; i < numdrivers; i++) {
        SDL_RendererInfo drinfo;
        SDL_GetRenderDriverInfo(i, &drinfo);
        printf("Driver name %d : %s\n", i, drinfo.name);
        if (drinfo.flags & SDL_RENDERER_SOFTWARE) {
            printf("the renderer is a software fallback \n");
        }
        if (drinfo.flags & SDL_RENDERER_ACCELERATED) {
            printf("the renderer is a hardware acceleration \n");
        }
        if (drinfo.flags & SDL_RENDERER_PRESENTVSYNC) {
            printf("present is synchronized with the refresh rate \n");
        }
        if (drinfo.flags & SDL_RENDERER_TARGETTEXTURE) {
            printf("the renderer supports rendering to texture \n");
        }
    }
}

void sdl_test1(void)
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    // Create a window usable with OpenGL context

    window = SDL_CreateWindow("Title", 10, 10, 10, 10,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    // Select render driver
    // - A render driver that supports HW acceleration is used when available
    // - Otherwise a render driver supporting software fallback is selected
    SDL_RendererInfo renderDriverInfo;
    uint32_t rendererFlags = SDL_RENDERER_TARGETTEXTURE;
    int32_t nbRenderDrivers = SDL_GetNumRenderDrivers(), index = 0;

    while (index < nbRenderDrivers) {
        if (SDL_GetRenderDriverInfo(index, &renderDriverInfo) == 0) {
            if (((renderDriverInfo.flags & rendererFlags) == rendererFlags) &&
                ((renderDriverInfo.flags & SDL_RENDERER_ACCELERATED) ==
                 SDL_RENDERER_ACCELERATED)) {
                // Using render driver with HW acceleration
                rendererFlags |= SDL_RENDERER_ACCELERATED;
                SDL_SetHint(SDL_HINT_RENDER_DRIVER, renderDriverInfo.name);
                break;
            }
        }
        ++index;
    }

    printf("index=%d, nbRenderDrivers=%d, rendererFlags=%d", index, nbRenderDrivers, rendererFlags);
    if (index == nbRenderDrivers) {
        // Let SDL use the first render driver supporting software fallback
        rendererFlags |= SDL_RENDERER_SOFTWARE;
        index = -1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, index, rendererFlags);
}


int main(void)
{
    // sdl_test();
    sdl_test1();
    return 0;
}
