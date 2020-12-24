//
// Created by 李源 on 2020-12-24.
//

#ifndef NEWCPURENDER_WINDOW_H
#define NEWCPURENDER_WINDOW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SD
#include <printf.h>
#include <iostream>
class Window {
private:
    SDL_Window *_window;
    SDL_Renderer* _render ;
    SDL_Texture* _buffer[2] ;
    SDL_Rect _headerTextRect;
    int _currentBuffer = 0;
    int _height, _width;
    TTF_Font font ;

public:
    void init_sdl()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            printf("[Error] SDL Init : %s \n", SDL_GetError());
        } else {
            printf("SDL INITIALISED\n");
            SDL_DisplayMode dm;
            SDL_GetCurrentDisplayMode(0, &dm);

            printf("Display mode is %dx%dpx @ %dhz\n", dm.w, dm.h, dm.refresh_rate);
        }
    }

    void init_window_and_renderer()
    {
        if (SDL_CreateWindowAndRenderer(_width, _height, SDL_WINDOW_SHOWN, &_window, &_render) != 0) {
            printf("[Error] Creating Window and Renderer: %s\n", SDL_GetError());
            exit(0);
        } else {
            printf("Created Window and Renderer %dx%d\n", _width, _height);
        }
    }

    Window(int height, int width)
    {

        if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        } else {

            SDL_CreateWindow(
                    "SDL2 Demo",
                    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                    width, height,
                    SDL_WINDOW_SHOWN
            );

            SDL_Delay(2000);
        }

        SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
        return 0;
    }
};


#endif //NEWCPURENDER_WINDOW_H
