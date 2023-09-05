#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// #include "types.h"
// #include "res.h"
// #include "render.h"
// #include "game.h"
// #include "ui.h"

#include "SDL.h"

#ifdef DR_DEMO

#ifdef DBG
#include <assert.h>
#endif

// extern int SDL_Init(Uint32 flags);
int main(int argc, char** args)
{
  srand(time(NULL));
  // Start up SDL and create window
  printf("DR DEMO\n");
//   SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
//   if (!init()) {
//     printf("Failed to initialize!\n");
//   } else {
    // Load media
    // if (!loadMedia()) {
    //   printf("Failed to load media!\n");
    // } else {
    //   mainUi();
    // }
//   }
//   cleanup();
    return 0;
}


#endif
