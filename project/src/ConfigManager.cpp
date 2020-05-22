#include "ConfigManager.h"
#include <fstream>

ConfigManager::ConfigManager()
{
    //ctor
}

ConfigManager::~ConfigManager()
{
    //dtor
}

void ConfigManager::init(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;

    fstream stream;
    string tmp, buff;
    stream.open(configFile.c_str());

    stream >> tmp >> buff;

    modelTile = new Tile;
    modelTile->load(buff, renderer);

    stream.close();
}
