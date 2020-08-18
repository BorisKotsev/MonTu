#ifndef WORLD_H
#define WORLD_H

#define _WIN32_WINNT 0x0500

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <SDL2/SDL.h>

#include "ConfigManager.h"
#include "SoundManager.h"
#include "PopUpWriter.h"
#include "PickAndBan.h"
#include "Engine.h"

#include "Tile.h"
#include "Squad.h"


class World
{
    public:
        World();
        virtual ~World();

        SDL_Window* m_main_window;
        SDL_Cursor* m_cursor;
        SDL_Renderer* m_main_renderer;
        SDL_Texture* m_backgroundTexture;
        SDL_Texture* m_menuTexture;

        SDL_Rect m_playButtonRect;
        SDL_Rect m_exitButtonRect;

        TTF_Font* m_font;

        ConfigManager m_configManager;
        SoundManager m_soundManager;
        PickAndBan m_pickAndBan;
        PopUpWriter m_popUpWriter;

        int m_SCREEN_WIDTH;
        int m_SCREEN_HEIGHT;
        short int m_colls;
        short int m_rows;
        coordinates m_mouse;
        bool m_mouseIsPressed;
        SDL_Event m_event;
        GAME_STATE m_gameState;
        bool m_quitScene;

        SDL_Texture* m_backgroundMapTexture;

        SDL_Texture* m_Map1Texture;
        SDL_Texture* m_Map2Texture;
        SDL_Texture* m_Map3Texture;
        SDL_Texture* m_Map4Texture;

        SDL_Texture* m_Map1PickTexture;
        SDL_Texture* m_Map2PickTexture;
        SDL_Texture* m_Map3PickTexture;
        SDL_Texture* m_Map4PickTexture;

        coordinates m_cameraOffset;
        short int m_cameraShakeDuration = 1;
        short int m_cameraShakeMagnitude = 2;
        time_t m_startShake;
        bool m_shake;

        char field[26][16];

        // Those are the coordinates that we use for determining the neighbors of a tile
        coordinates directions[2][6];

        coordinates m_selected;

        vector<vector<Tile*> > m_tiles;
        vector<Squad*> m_squads;

        vector<SQUAD> m_available;
        vector<SQUAD> m_banned;

        bool canTravel(coordinates position, coordinates desiredPosition, int movement);
        bool canShoot(coordinates position, coordinates targetPosition, short int range);
        Tile* giveNeighbor(coordinates coor, int direction);


        void initSDL(string configFile);
        void initDirection(string configFile);
        void draw();
        void update();
        void cleaner();
        void destroySDL();
        void input();

        void initSession(GAME_STATE state);

        void cameraShake();

        void menu();

        void pickAndBan();

        void initTiles(string configFile);
        void selectTile();

        void initMap(string configFile);
        void Choose_Map();


    protected:

    private:
};

#endif // WORLD_H
