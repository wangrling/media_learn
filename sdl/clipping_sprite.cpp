//
// Created by wangrl on 19-3-27.
//

#include <ostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "easy_cleanup.h"
#include "res_path.h"

// Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer* renderer) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, file.c_str());
    if (texture == nullptr){
        logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at some destination rect
 * taking a clip of the texture if desired
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param dst The destination rectangle to render the texture too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, SDL_Rect dst, SDL_Rect *clip = nullptr) {
    SDL_RenderCopy(renderer, texture, clip, &dst);
}

void renderTexture(SDL_Texture* texture, SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = nullptr) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr) {
        dst.w = clip->w;
        dst.h = clip->h;
    } else {
        SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    }
    renderTexture(texture, renderer, dst, clip);
}


int main(int argc, char* argv[]) {
    //Start up SDL and make sure it went ok
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    //Setup our window and renderer
    SDL_Window *window = SDL_CreateWindow("Lesson 5", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        logSDLError(std::cout, "CreateWindow");
        SDL_Quit();
        return 1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

    const std::string resPath = getResourcePath();
    SDL_Texture* image = loadTexture(resPath + "image_clipping_sprite.png", renderer);
    if (image == nullptr){
        cleanup(image, renderer, window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    //iW and iH are the clip width and height
    //We'll be drawing only clips so get a center position for the w/h of a clip
    // 中间绘制
    int iW = 100, iH = 100;
    int x = SCREEN_WIDTH / 2 - iW / 2;
    int y = SCREEN_HEIGHT / 2 - iH / 2;

    //Setup the clips for our image
    SDL_Rect clips[4];
    for (int i = 0; i < 4; ++i){
        clips[i].x = i / 2 * iW;
        clips[i].y = i % 2 * iH;
        clips[i].w = iW;
        clips[i].h = iH;
    }

    // Specify a default clip to start with.
    int useClip = 0;

    SDL_Event e;
    bool quit = false;

    while (!quit) {
        // Event Polling
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // Use number input to select which clip should be drawn.
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_1:
                    case SDLK_KP_1:
                        useClip = 0;
                        break;
                    case SDLK_2:
                    case SDLK_KP_2:
                        useClip = 1;
                        break;
                    case SDLK_3:
                    case SDLK_KP_3:
                        useClip = 2;
                        break;
                    case SDLK_4:
                    case SDLK_KP_4:
                        useClip = 3;
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    default:
                        break;
                }
            }
        }

        // Rendering
        SDL_RenderClear(renderer);
        // Draw the image
        renderTexture(image, renderer, x, y, &clips[useClip]);
        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    cleanup(image, renderer, window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
