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
#include "HealthManager.h"
#include "PopUpWriter.h"
#include "PickAndBan.h"
#include "PlayerStatsManager.h"
#include "Battle.h"
#include "Menu.h"
#include "Engine.h"



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
        SDL_Rect m_ExitButtonRect;
        SDL_Rect m_BackButtonRect;
        SDL_Rect m_OptionsButtonRect;

        SDL_Rect m_Map1Button;
        SDL_Rect m_Map2Button;
        SDL_Rect m_Map3Button;
        SDL_Rect m_Map4Button;

        TTF_Font* m_font;

        ConfigManager m_configManager;
        SoundManager m_soundManager;
        PickAndBan m_pickAndBan;
        HealthManager m_healthManager;
        PlayerStatsManager m_playerStatsManager;
        Battle m_battle;
        Menu m_menu;

        int m_SCREEN_WIDTH;
        int m_SCREEN_HEIGHT;
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

        SDL_Texture* m_PlayButtonTexture;
        SDL_Texture* m_OptionsButtonTexture;
        SDL_Texture* m_ExitButtonTexture;
        SDL_Texture* m_BackButtonTexture;

        coordinates m_cameraOffset;
        short int m_cameraShakeDuration = 1;
        short int m_cameraShakeMagnitude = 2;
        time_t m_startShake;
        bool m_shake;

        color m_CP1;
        color m_CP2;

        char field[26][16];

        vector<SQUAD> m_available;
        vector<SQUAD> m_banned;

        vector<Building*> m_buildings;

        void initSDL(string configFile);
        void draw();
        void update();
        void cleaner();
        void destroySDL();
        void input();

        void initSession(GAME_STATE state);

        void cameraShake();

        void pickAndBan();

        void initMap(string configFile);
        void Choose_Map();


    protected:

    private:
};

#endif // WORLD_H
