#ifndef SQUAD_H
#define SQUAD_H

#include <string>
#include <fstream>
#include <iostream>

#include "defines.h"
#include "Engine.h"
#include "Tile.h"

class Squad
{
    public:
        Squad();
        Squad(const Squad& model, coordinates* cameraOffset, Tile* tile, OWNER owner);
        virtual ~Squad();

        SDL_Renderer* m_renderer;
        SDL_Texture* m_objectTexture;

        SDL_Rect m_objectRect;
        SDL_Rect m_presentRect;
        coordinates m_mapCoor;

        coordinates* m_cameraOffset;

        OWNER m_owner;
        short int m_startHealth;
        short int m_health;
        short int m_startSpeed;
        short int m_speed;
        short int m_startAttackRange;
        short int m_attackRange;
        short int m_startAttackDamage;
        short int m_attackDamage;
        short int m_startFaith;
        short int m_faith;

        bool m_traveling;
        bool m_moved;

        Tile* m_tileTaken;

        vector<coordinates> m_path;

        virtual void load(string configFile, SDL_Renderer* renderer);
        virtual void update();
        virtual void draw();

    protected:

    private:
};

#endif // SQUAD_H
