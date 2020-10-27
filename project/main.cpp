#include  <SDL2/SDL.h>

#include "World.h"

World world;

int main(int argc, char* argv[])
{
    world.initSDL("world.txt");

    world.m_playerStatsManager.readAllValues();
    world.m_playerStatsManager.changeValues(MONEY, 1111);
    world.m_playerStatsManager.writeAllValues();


    world.m_gameState = MENU;

    while(true)
    {
        if(world.m_gameState == MENU)
        {
            while(!world.m_quitScene)
            {
                world.input();
                world.menu();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == MAP_CHOOSING)
        {
            while(!world.m_quitScene)
            {
                world.input();
                world.Choose_Map();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == PICK_BAN)
        {
            while(!world.m_quitScene)
            {
                world.input();
                ///world.m_pickAndBan.update();
                ///world.m_pickAndBan.draw();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == GAME)
        {
            world.initGameSession();
            world.m_enemyAI.takeBattlefield();
            world.m_enemyAI.returnBattlefield();
            while(!world.m_quitScene)
            {
                world.input();
                world.update();
                world.draw();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == EXIT)
        {
            world.destroySDL();
        }

        SDL_Delay(25);
    }

    return 0;
}
