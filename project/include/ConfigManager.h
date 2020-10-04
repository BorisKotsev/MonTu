#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <SDL2/SDL.h>
#include <string>

#include "Tile.h"
#include "Squad.h"
#include "HealthManager.h"

using namespace std;

class ConfigManager
{
public:
    ConfigManager();
    virtual ~ConfigManager();

    Tile* modelTileGrass = NULL;
    Tile* modelTileWater = NULL;
    Tile* modelTileMountain = NULL;
    Tile* modelTileForest = NULL;
    Tile* modelTileDesert = NULL;
    Tile* modelTileStone = NULL;
    Tile* modelTileVolcano = NULL;
    Tile* modelTileLava = NULL;

    Squad* modelSquadWarrior = NULL;
    Squad* modelSquadArcher = NULL;

    void init(string configFile, SDL_Renderer* renderer, HealthManager* hm);

protected:

private:
};

#endif // CONFIGMANAGER_H
