#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <SDL2/SDL.h>
#include <string>

#include "Tile.h"

using namespace std;

class ConfigManager
{
public:
    ConfigManager();
    virtual ~ConfigManager();

    Tile* modelTile = NULL;

    void init(string configFile, SDL_Renderer* renderer);

protected:

private:
};

#endif // CONFIGMANAGER_H
