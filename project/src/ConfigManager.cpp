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

void ConfigManager::init(string configFile, SDL_Renderer* renderer, HealthManager* hm)
{
    configFile = "config\\" + configFile;

    fstream stream;
    string tmp, buff;
    stream.open(configFile.c_str());

    stream >> tmp >> buff;
    modelTileGrass = new Tile;
    modelTileGrass->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileWater = new Tile;
    modelTileWater->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileMountain = new Tile;
    modelTileMountain->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileForest = new Tile;
    modelTileForest->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileDesert = new Tile;
    modelTileDesert->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileStone = new Tile;
    modelTileStone->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileVolcano = new Tile;
    modelTileVolcano->load(buff, renderer);

    stream >> tmp >> buff;
    modelTileLava = new Tile;
    modelTileLava->load(buff, renderer);

    stream >> tmp >> buff;
    modelSquadWarrior = new Squad;
    modelSquadWarrior->load(buff, renderer, hm);

    stream >> tmp >> buff;
    modelSquadArcher = new Squad;
    modelSquadArcher->load(buff, renderer, hm);

    stream >> tmp >> buff;
    modelCastle = new Building;
    modelCastle->load(buff, renderer);

    stream.close();
}
