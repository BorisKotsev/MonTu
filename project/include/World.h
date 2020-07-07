#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <SDL2/SDL.h>

#include "ConfigManager.h"
#include "SoundManager.h"
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
        SDL_Renderer* m_main_renderer;
        SDL_Texture* m_backgroundTexture;
        SDL_Texture* m_menuTexture;

        SDL_Rect m_playButtonRect;
        SDL_Rect m_exitButtonRect;

        TTF_Font* m_font;

        ConfigManager m_configManager;
        SoundManager m_soundManager;
        PickAndBan m_pickAndBan;

        int m_SCREEN_WIDTH;
        int m_SCREEN_HEIGHT;
        short int m_colls;
        short int m_rows;
        coordinates m_mouse;
        bool m_mouseIsPressed;
        SDL_Event m_event;
        GAME_STATE m_gameState;
        bool m_quitScene;

        coordinates directions[2][6];

        coordinates m_selected;

        vector<vector<Tile*> > m_tiles;
        vector<Squad*> m_squads;

        vector<SQUAD> m_available;
        vector<SQUAD> m_banned;

        bool canTravel(coordinates position, coordinates desiredPosition);
        Tile* giveNeighbor(coordinates coor, int direction);


        void initSDL(string configFile);
        void initDirection(string configFile);
        void draw();
        void update();
        void cleaner();
        void destroySDL();
        void input();

        void initSession(GAME_STATE state);

        void menu();

        void pickAndBan();

        void initTiles(string configFile);
        void selectTile();

    protected:

    private:
};

#endif // WORLD_H
