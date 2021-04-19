#include "ArmyManager.h"
#include "World.h"

extern World world;

ArmyManager::ArmyManager()
{
    //ctor
}

ArmyManager::~ArmyManager()
{
    //dtor
}

void ArmyManager::init(string configFile)
{
    configFile = "data\\" + configFile;
    fstream stream;

    string tmp;

    stream.open(configFile.c_str());
    stream >> tmp >> armyFormationPath;
    stream.close();
    armyFormationPath = "data\\" + armyFormationPath;
}

void ArmyManager::deployArmy(OWNER owner)
{
    fstream stream;

    char formation[world.m_battle.m_rows][3];

    stream.open(armyFormationPath.c_str());
    cout << "THE FORMATION IS: \n-------------------- \n";
    for (int r = 0; r < world.m_battle.m_rows; r ++)
    {
        for (int c = 0; c < 3; c++)
        {
            stream >> formation[r][c];
            cout << formation[r][c];
        }
        cout << endl;
    }
    cout << "-------------------- \n";
    stream.close();

    coordinates buff;
    for (int r = 0; r < world.m_battle.m_rows; r ++)
    {
        for (int c = 0; c < 3; c++)
        {
            switch (formation[r][c])
            {
            case 'W':
                buff.x = c;
                buff.y = r;
                world.m_battle.initSquad(WARRIOR, buff, owner);
                break;
            case 'A':
                buff.x = c;
                buff.y = r;
                world.m_battle.initSquad(ARCHER, buff, owner);
                break;
            default :
                break;
            }
        }
    }
}

void ArmyManager::deployArmy(int index)
{
    string location = "data\\Enemies\\enemy_army_" + to_string(index) + ".txt";

    fstream stream;

    char formation[world.m_battle.m_rows][6];

    stream.open(location);
    cout << "THE FORMATION IS: \n-------------------- \n";
    for (int r = 0; r < world.m_battle.m_rows; r ++)
    {
        for (int c = 0; c < 6; c++)
        {
            stream >> formation[r][c];
            cout << formation[r][c];
        }
        cout << endl;
    }
    cout << "-------------------- \n";
    stream.close();

    coordinates buff;
    for (int r = 0; r < world.m_battle.m_rows; r ++)
    {
        for (int c = 0; c < 6; c++)
        {
            switch (formation[r][c])
            {
            case 'W':
                buff.x = c + 15;
                buff.y = r;
                world.m_battle.initSquad(WARRIOR, buff, PLAYER2);
                break;
            case 'A':
                buff.x = c + 15;
                buff.y = r;
                world.m_battle.initSquad(ARCHER, buff, PLAYER2);
                break;
            }
        }
    }
}

