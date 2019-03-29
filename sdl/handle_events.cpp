//
// Created by wangrl on 19-3-27.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "easy_cleanup.h"
#include "res_path.h"

// Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(std::ostream &os, const std::string &msg) {
    os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
 * Loads an image into a texture on the rendering device.
 * @param file The image file to load.
 * @param renderer The renderer to load the texture onto.
 * @return The load texture, or nullptr if something went wrong.
 */
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file.c_str());
    if (texture == nullptr)
        logSDLError(std::cout, "LoadTexture");
    return texture;
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, with some desired width and height.
 * @param texture The source texture we want to draw.
 * @param renderer The renderer we want to draw too.
 * @param x The x coordinate to draw too.
 * @param y The y coordinate to draw too.
 * @param w The width of the texture to draw.
 * @param h The height of the texture to draw.
 */
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, int w, int h) {
    // Setup the destination rectangle to be at the position we want.
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;

    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y) {
    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    renderTexture(texture, renderer, x, y, w, h);
}

int main(int argc, char* argv[]) {
    // Start up SDL and make sure it went ok.
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    // Setup our window and renderer, this time let's put our window in the center
    // of the screen.
    SDL_Window* window = SDL_CreateWindow("Handle events", SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

    // The texture we'll be using.
    const std::string resPath = getResourcePath();
    SDL_Texture* image = loadTexture(resPath + "image_handle_events.png", renderer);
    if (image == nullptr) {
        cleanup(image, renderer, window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Our texture size won't change, so we can get it here
    // instead of constantly allocating/deleting in the loop.
    int iW, iH;
    SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;

    // Our event structure.
    SDL_Event e;
    // For tracking if we want to quit.
    bool quit = false;

    while (!quit) {
        // Read any events that occurred, for now we'll just quit if any event occurs.
        while (SDL_PollEvent(&e)) {
            // If user closes the window.
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            // If user presses any key.
            if (e.type == SDL_KEYDOWN) {
                quit = true;
            }
            // If user clicks the mouse.
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                quit = true;
            }
        }
        // Rendering
        SDL_RenderClear(renderer);
        // Draw the image.
        renderTexture(image, renderer, x, y);
        // Update the screen.
        SDL_RenderPresent(renderer);
    }

    // Destroy the various items.
    cleanup(image, renderer, window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}