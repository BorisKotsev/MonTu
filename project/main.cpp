#include  <SDL2/SDL.h>

#include "World.h"

int main(int argc, char* argv[])
{
    World world;

    world.initSDL("world.txt");

    world.m_menuState = GAME;

    while(true)
    {
        if(world.m_menuState == MENU)
        {
            while(!world.m_quitScene)
            {
                world.input();

            }
            world.m_quitScene = false;
        }
        if(world.m_menuState == PICK_BAN)
        {
            while(!world.m_quitScene)
            {
                world.input();
            }
            world.m_quitScene = false;
        }
        if(world.m_menuState == GAME)
        {
            while(!world.m_quitScene)
            {
                world.input();
                world.update();
                world.draw();
            }
            world.m_quitScene = false;
        }

        SDL_Delay(25);
    }

    return 0;
}
