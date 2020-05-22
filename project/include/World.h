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

        ConfigManager m_configManager;
        SoundManager m_soundManager;

        int m_SCREEN_WIDTH;
        int m_SCREEN_HEIGHT;
        short int m_colls;
        short int m_rows;
        coordinates m_mouse;
        bool m_mouseIsPressed;
        SDL_Event m_event;
        MENU_STATE m_menuState;
        bool m_quitScene;

        vector<vector<Tile*> > m_tiles;
        vector<Squad*> m_squads;

        void initSDL(string configFile);
        void draw();
        void update();
        void cleaner();
        void destroySDL();
        void input();

        void menu();

        void pickAndBan();

        void initTiles(string configFile);
        void selectTile();

    protected:

    private:
};

#endif // WORLD_H
