#include  <SDL2/SDL.h>

#include "World.h"
#include "FPSManager.h"
#include "WorldBuilding.h"
#include "Animator.h"

World world;
FPSManager frameManager;

WorldBuilding cityView;

Animator animator;

int main(int argc, char* argv[])
{
    world.initSDL("world.txt");

    coordinates coor;
    coor.x = 40;
    coor.y = 0;

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
                world.m_menu.update();
                world.m_menu.draw();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == MAP_CHOOSING)
        {
            while(!world.m_quitScene)
            {
                world.input();
                world.m_menu.Choose_Map();
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
            world.m_battle.initGameSession();
            string file = "warriorMove.txt";
            animator.initAnimation(file, &(world.m_battle.m_squads[0]->m_objectRect));

            world.m_battle.m_enemyAI.init("enemyAI.txt");
            ///world.m_castleUI.loadData("soldier_data_0.txt");
            animator.start();
            while(!world.m_quitScene)
            {
                const Uint8 *state = SDL_GetKeyboardState(NULL);
                /*
                if(animator.update())
                {
                    animator.draw();
                }
                */
                world.input();
                world.m_battle.update();
                world.m_battle.draw();
                if(state[SDL_SCANCODE_B])
                {
                    world.m_battle.switchTurn();
                    cout << "AI MAKING A TURN \n";
                }
                ///world.m_castleUI.update();
                ///world.m_castleUI.draw();
            }
            world.m_quitScene = false;
        }
        if(world.m_gameState == WORLD_MAP)
        {
            world.m_worldMap.init("worldMap.txt");
            world.m_worldMap.loadMap("worldMapPieces.txt");

            while(!world.m_quitScene)
            {
                world.input();
                world.m_worldMap.update();
                world.m_worldMap.draw();
            }
            world.m_quitScene = false;
        }

        if(world.m_gameState == CITYBUILDING)
        {
            cityView.initCity("cityView.txt");
            while(!world.m_quitScene)
            {
                world.input();
                cityView.updateBuilding();
                cityView.drawBuilding();
                //cityView.saveBuildings("buildingsSave.txt");
            }

            world.m_quitScene = false;
        }

        if(world.m_gameState == EXIT)
        {
            world.destroySDL();
        }

        while(frameManager.elapsedTime() < MIN_FRAMETIME) SDL_Delay(1);
        frameManager.start();
    }

    return 0;
}
