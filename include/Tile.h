#ifndef TILE_H
#define TILE_H

#include <string>
#include <fstream>

#include "defines.h"
#include "Engine.h"

using namespace std;

class Tile
{
    public:
        Tile();
        Tile(const Tile& model);
        virtual ~Tile();

        SDL_Texture* m_objectTexture;
        SDL_Rect m_objectRect;

        OWNER m_owner;
        short int m_walkDifficulty;
        vector<SDL_Point*> m_collisionPoints;

        coordinates m_drawCoordinates;
        coordinates m_mapCoordinates;

        void init(Tile* model);
        void load(string configFile, SDL_Renderer* renderer);
        void draw(SDL_Renderer* renderer);

    protected:

    private:
};

#endif // TILE_H
