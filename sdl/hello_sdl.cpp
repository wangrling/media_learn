//
// Created by wangrl on 19-3-24.
//

extern "C" {
#include <SDL2/SDL.h>
}
#include <iostream>
#include "res_path.h"

int main(int argc, char *argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    }

    SDL_Window *window = SDL_CreateWindow("Hello SDL2!", 100, 100, 640, 800, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    std::string imagePath = getResourcePath() + "hello.bmp";
    SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
    if (texture == nullptr) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        SDL_Quit();

        return 1;
    }

    // A sleep rendering loop, wait for 3 seconds and render an present the screen each time.
    for (int i = 0; i < 3; ++i) {
        // First clear the renderer.
        SDL_RenderClear(renderer);
        // Draw the texture.
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        // update the screen.
        SDL_RenderPresent(renderer);
        // Take a quick break after all that hard work
        SDL_Delay(1000);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

