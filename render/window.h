//
// Created by 李源 on 2020-12-24.
//

#ifndef NEWCPURENDER_WINDOW_H
#define NEWCPURENDER_WINDOW_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_audio.h>
#include <iostream>

#include "LTexture.h"

class Window {
private:
    SDL_Window *_window ;
    SDL_Renderer* _render ;
    LTexture* texture[2] ;
    Uint32* data[2] ;
    SDL_Rect start;
    SDL_Rect end ;
    int _currentBuffer = 0 ;
    int _height, _width ;

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


    Window(int height, int width,Uint32* pixels)
    {
        _height = height ;
        _width = width ;
        if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        }else{
            _window =  SDL_CreateWindow(
                    "SDL2 Demo",
                    100, 100,
                    _width, _height,
                    0
            );
            _render = SDL_CreateRenderer(_window, -1, 0);

            start.x = end.x = 0;
            start.y = end.y = 0;
            start.w = end.w = width;
            start.h = end.h = height;

            texture[1] = new LTexture(_render) ;
            texture[0] = new LTexture(_render) ;
            data[0] = pixels;
            texture[0]->loadFromBuffer(pixels,_width,_height) ;
            _currentBuffer = 0 ;
        }

    }

    void render(Uint32* data)
    {
        if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        } else {

           while (1) {

                // event handling
                SDL_Event e;
                if ( SDL_PollEvent(&e) ) {
                    if (e.type == SDL_QUIT)
                        break;
                    else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
                        break;
                }

                SDL_RenderClear(_render);
                texture[!bool(_currentBuffer)]->loadFromBuffer(data,_width,_height) ;
                SDL_RenderCopy(_render, texture[_currentBuffer]->getTexture(), &start, &end);
                texture[_currentBuffer]->loadFromBuffer(data,_width,_height) ;
                _currentBuffer = !bool(_currentBuffer) ;
                SDL_RenderPresent(_render);

            }
           delete texture[0] ;
           delete texture[1] ;

           SDL_DestroyRenderer(_render) ;
           SDL_DestroyWindow(_window);
           return ;
        }
    }
};


#endif //NEWCPURENDER_WINDOW_H
