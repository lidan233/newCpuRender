//
// Created by 李源 on 2020-12-24.
//

#ifndef NEWCPURENDER_WINDOW_H
#define NEWCPURENDER_WINDOW_H

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_audio.h>
#include <iostream>
#include <sstream>

#include "Vec.h"
#include "LTexture.h"
#include "camera.h"

const int width = 1024;
const int height = 1024 ;
const int depth = 255  ;

//Vec3f light_dir(0,0,-1) ;
//Vec3f light_dir = Vec3f(1,-1,1).normalize();
Vec3f camera_pos(0,0,3) ;
Vec3f eye(1,1,3);
Vec3f center(0,0,0);
Vec3f up(0,1,0);
Vec3f light_dir = camera_pos - center ;

bool is_Point_Light = true ;

float yaw = NYAW ;
float pitch = NPITCH ;
camera ca(camera_pos,up,center,yaw,pitch) ;

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
    int cursor_X ,cursor_Y ;
    int cursor_X_offset=0, cursor_Y_offset = 0;
    bool firstCursor = true ;
    bool useCursor = true ;
public:

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

    int render(Uint32* data)
    {
        if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
            std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        } else {
                SDL_Event e;
                if ( SDL_PollEvent(&e) ) {
                    if (e.type == SDL_QUIT)
                        return -1;
                    else if (e.type == SDL_KEYUP)
                    {
                        switch (e.key.keysym.sym) {
                            case SDLK_ESCAPE: return -1 ; break ;
                            case SDLK_w: ca.ProcessKeyboard(FORWARD,0.1) ;break ;
                            case SDLK_a: ca.ProcessKeyboard(LEFT,0.1); break ;
                            case SDLK_s: ca.ProcessKeyboard(BACKWARD,0.1); break ;
                            case SDLK_d: ca.ProcessKeyboard(RIGHT,0.1);break ;
                            case SDLK_l: is_Point_Light = !is_Point_Light ; break ;
                        }
                    }
//                    else if (e.type==SDL_MOUSEBUTTONDOWN)
//                    {
//                        std::cout<<"push button"<<std::endl ;
//                        useCursor = true ;
//                    }
                    else if(e.type==SDL_MOUSEMOTION)
                    {

                        std::cout<<useCursor<<std::endl ;
                        if(useCursor)
                        {
                            int mouseX = e.motion.x;
                            int mouseY = e.motion.y;
                            if(firstCursor)
                            {
                                cursor_X = mouseX ;
                                cursor_Y = mouseY ;
                                firstCursor = false;
                            }

                            cursor_X_offset = cursor_X - mouseX ;
                            cursor_Y_offset = cursor_Y - mouseY ;
                            cursor_X = mouseX ;
                            cursor_Y = mouseY ;
//                            useCursor = false ;
                        }
//                        std::cout << "X offset: " << cursor_X_offset << " Y offset: " << cursor_Y_offset;
//                        std::cout << "X: " << mouseX << " Y: " << mouseY;

                    }

                }

                SDL_RenderClear(_render);
                texture[!bool(_currentBuffer)]->loadFromBuffer(data,_width,_height) ;
                SDL_RenderCopy(_render, texture[_currentBuffer]->getTexture(), &start, &end);
                texture[_currentBuffer]->loadFromBuffer(data,_width,_height) ;
                _currentBuffer = !bool(_currentBuffer) ;
                SDL_RenderPresent(_render);

//            }
//           delete texture[0] ;
//           delete texture[1] ;
//
//           SDL_DestroyRenderer(_render) ;
//           SDL_DestroyWindow(_window);
           return 0;
        }
    }

    void endrender()
    {
        delete texture[0] ;
        delete texture[1] ;

        SDL_DestroyRenderer(_render) ;
        SDL_DestroyWindow(_window);
    }

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

    Vec2f getOffset()
    {
        Vec2f offset = Vec2f(0,0) ;
        offset[0] = float(cursor_X_offset )/ float(_width )* 90.0;
        offset[1] = float(cursor_Y_offset )/ float(_height )* 45.0;
        cursor_X_offset = 0 ;
        cursor_Y_offset = 0;
        return offset ;

    }
};


#endif //NEWCPURENDER_WINDOW_H
