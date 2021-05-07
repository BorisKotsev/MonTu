#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <SDL2/SDL.h>
#include <string>

#include "Tile.h"
#include "Building.h"
#include "Castle.h"
#include "Bridge.h"

#include "UISelectable.h"
#include "UI.h"
#include "Buttons.h"

#include "SpearSquad.h"
#include "HookSquad.h"
#include "ArmyCamp.h"
#include "HealthManager.h"

#include "Particle.h"

using namespace std;

class ConfigManager
{
public:
    ConfigManager();
    virtual ~ConfigManager();

    Tile* modelTileGrass = nullptr;
    Tile* modelTileWater = nullptr;
    Tile* modelTileMountain = nullptr;
    Tile* modelTileForest = nullptr;
    Tile* modelTileDesert = nullptr;
    Tile* modelTileStone = nullptr;
    Tile* modelTileVolcano = nullptr;
    Tile* modelTileLava = nullptr;

    Squad* modelSquadWarrior = nullptr;
    Squad* modelSquadArcher = nullptr;
    SpearSquad* modelSquadSpearmen = nullptr;
    HookSquad* modelSquadCrossbowmen = nullptr;
    Squad* modelSquadKnights = nullptr;
    HealthManager* modelHealthManager = nullptr;

    Castle* modelCastle = nullptr;
    Building* modelArchery = nullptr;
    Building* modelShop = nullptr;
    Building* modelStreet = nullptr;

    ArmyCamp* modelArmyCamp = nullptr;
    Bridge* modelBridge = nullptr;

    UI* modelUI = nullptr;
    UISelectable* modelUISelectable = nullptr;
    UISelectable* modelUISelectableSelected = nullptr;

    Buttons* modelButtons = nullptr;

    Particle* modelMeleeHit = nullptr;

    void init(string configFile, SDL_Renderer* renderer, HealthManager* hm);

protected:

private:
};

#endif // CONFIGMANAGER_H
