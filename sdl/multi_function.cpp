//
// Created by wangrl on 19-3-25.
//

#include <iosfwd>
#include <iostream>
#include <SDL2/SDL_quit.h>
#include <SDL2/SDL.h>
#include "easy_cleanup.h"
#include "res_path.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/**
 * Log an SDL error with some error message to the output stream of our choice.
 * @param os    The output stream to write the message to.
 * @param msg   The error message to write, format will be msg error: SDL_GetError().
 */
void logSDLError(std::ostream &os, const std::string &msg) {
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture *loadTexture(const std::string &file, SDL_Renderer *renderer) {
    //Initialize to nullptr to avoid dangling pointer issues.
    SDL_Texture *texture = nullptr;
    // Load the image.
    SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
    // If the loading went ok, convert to texture and return the texture.
    if (loadedImage != nullptr) {
        texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
        SDL_FreeSurface(loadedImage);
        // Make sure converting went ok too.
        if (texture == nullptr) {
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    } else {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y) {
    // Setup teh destination rectangle to be at the position we want.
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    // Query the texture to get its width and height to use.
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);

    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

int main(int argc, char* argv[]) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Multi function", 100, 100,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

    const std::string resPath = getResourcePath();
    SDL_Texture *background = loadTexture(resPath + "background.bmp", renderer);
    SDL_Texture *image = loadTexture(resPath + "image.bmp", renderer);
    if (background == nullptr || image == nullptr){
        cleanup(background, image, renderer, window);
        SDL_Quit();
        return 1;
    }

    SDL_RenderClear(renderer);

    int bW, bH;
    SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
    renderTexture(background, renderer, 0, 0);
    renderTexture(background, renderer, bW, 0);
    renderTexture(background, renderer, 0, bH);
    renderTexture(background, renderer, bW, bH);

    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;
    renderTexture(image, renderer, x, y);

    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    cleanup(background, image, renderer, window);

    SDL_Quit();

    return 0;
}

