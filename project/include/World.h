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

#include "HealthManager.h"
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
        SDL_Rect m_ExitButtonRect;
        SDL_Rect m_BackButtonRect;
        SDL_Rect m_OptionsButtonRect;

        SDL_Rect m_Map1Button;
        SDL_Rect m_Map2Button;
        SDL_Rect m_Map3Button;
        SDL_Rect m_Map4Button;

        TTF_Font* m_font;

        HealthManager m_healthManager;
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
        OWNER m_playerTurn;

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

        UI_object m_selectedTileUI;
        UI_object m_attackTileUI;
        UI_object m_skipTurnFillBtn;
        UI_object m_skipTurnTransBtn;

        bool m_showFillBtn;
        bool m_showAttackTiles;

        coordinates m_cameraOffset;
        short int m_cameraShakeDuration = 1;
        short int m_cameraShakeMagnitude = 2;
        time_t m_startShake;
        bool m_shake;

        char field[26][16];

        // Those are the coordinates that we use for determining the neighbors of a tile
        coordinates directions[2][6];

        coordinates m_selected;
        Squad* m_selectedSquad;
        Tile* m_selectedTile;

        vector<vector<Tile*> > m_tiles;
        vector<Squad*> m_squads;

        vector<Tile*> m_availableWalkTiles;
        vector<Tile*> m_availableShootTiles;

        vector<SQUAD> m_available;
        vector<SQUAD> m_banned;

        bool canTravel(Squad* squad, coordinates desiredPosition);
        vector<Tile*> showAvailableWalkTiles(Squad* squad);
        vector<Tile*> showAvailableShootTiles(Squad* squad);
        bool canShoot(Squad* squad, coordinates targetPosition);
        Tile* giveNeighbor(coordinates coor, int direction);
        Squad* findSquadByCoor (coordinates coor);


        void initSDL(string configFile);
        void initDirection(string configFile);
        void initGameSession();
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

        void initSquad(SQUAD type, coordinates mapCoor, OWNER owner);
        void initSquads(string configFile);

        void checkForTurnSwitch();
        void switchTurn();
        void shoot(Squad* attackingSquad, Squad* defendingSquad);
        void takeDamage(Squad* attacker, Squad* defender);

    protected:

    private:
};

#endif // WORLD_H
