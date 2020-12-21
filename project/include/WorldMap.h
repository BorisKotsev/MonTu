#ifndef WORLDMAP_H
#define WORLDMAP_H

#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <ctime>
#include <vector>
#include <time.h>

#include "Engine.h"
#include "defines.h"

class WorldMap
{
    public:
        WorldMap();
        virtual ~WorldMap();

        GAME_STATE gameState;

        SDL_Texture* WorldMapTexture;
        SDL_Texture* NextButtonTexture;

        SDL_Rect nextButtonRect;
        SDL_Rect maxZoom;
        SDL_Rect minZoom;
        SDL_Rect cameraRect;

        SDL_Rect cameraDstRect1;
        SDL_Rect cameraDstRect2;
        SDL_Rect cameraDstRect3;
        SDL_Rect cameraDstRect4;

        SDL_Rect cameraSrcRect1;
        SDL_Rect cameraSrcRect2;
        SDL_Rect cameraSrcRect3;
        SDL_Rect cameraSrcRect4;

        SDL_Event event;

        SDL_Scancode moveUp;
        SDL_Scancode moveDown;
        SDL_Scancode moveLeft;
        SDL_Scancode moveRight;

        coordinates mouse;
        coordinates cameraCenter;
        coordinates direction;

        vector <SDL_Texture*> mapPieces;

        int zoomMulti;
        int mapWidth;
        int mapHeight;
        int oldX, oldY;

        float directionAngle;

        bool* quitScene;
        bool mouseIsPressed;

        vector <SDL_Texture*> drawMap(SDL_Renderer* renderer);

        void init(SDL_Renderer* renderer, string configFile);
        void update();
        void draw(SDL_Renderer* renderer);
        void input();
        void framer();
        void zoom();

    protected:

    private:
};

#endif // WORLDMAP_H
