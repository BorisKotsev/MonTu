#include "Menu.h"
#include "World.h"

extern World world;

Menu::Menu()
{

}

Menu::~Menu()
{
    //dtor
}

void Menu::init(string configFile)
{
    renderer = world.m_main_renderer;

    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;

    string backgroundMap;
    string Map1Img;
    string Map2Img;
    string Map3Img;
    string Map4Img;
    string Map1PickImg;
    string Map2PickImg;
    string Map3PickImg;
    string Map4PickImg;
    string menuImg;
    string PlayButtonImg;
    string ExitButtonImg;
    string OptionsButtonImg;
    string BackButtonImg;

    stream.open(configFile.c_str());
    stream >> tmp >> menuImg;
    stream >> tmp >> m_playButtonRect.x >> m_playButtonRect.y >> m_playButtonRect.w >> m_playButtonRect.h;
    stream >> tmp >> backgroundMap;
    stream >> tmp >> m_ExitButtonRect.x >> m_ExitButtonRect.y >> m_ExitButtonRect.w >> m_ExitButtonRect.h;
    stream >> tmp >> PlayButtonImg;
    stream >> tmp >> ExitButtonImg;
    stream >> tmp >> OptionsButtonImg;
    stream >> tmp >> BackButtonImg;
    stream >> tmp >> m_BackButtonRect.x >> m_BackButtonRect.y >> m_BackButtonRect.w >> m_BackButtonRect.h;
    stream >> tmp >> m_OptionsButtonRect.x >> m_OptionsButtonRect.y >> m_OptionsButtonRect.w >> m_OptionsButtonRect.h;
    stream >> tmp >> Map1Img;
    stream >> tmp >> Map2Img;
    stream >> tmp >> Map3Img;
    stream >> tmp >> Map4Img;
    stream >> tmp >> Map1PickImg;
    stream >> tmp >> Map2PickImg;
    stream >> tmp >> Map3PickImg;
    stream >> tmp >> Map4PickImg;
    stream >> tmp >> m_Map1Button.x >> m_Map1Button.y >> m_Map1Button.w >> m_Map1Button.h;
    stream >> tmp >> m_Map2Button.x >> m_Map2Button.y >> m_Map2Button.w >> m_Map2Button.h;
    stream >> tmp >> m_Map3Button.x >> m_Map3Button.y >> m_Map3Button.w >> m_Map3Button.h;
    stream >> tmp >> m_Map4Button.x >> m_Map4Button.y >> m_Map4Button.w >> m_Map4Button.h;
    stream.close();

    m_menuTexture = LoadTexture(menuImg, renderer);
    m_Map1Texture = LoadTexture(Map1Img, renderer);
    m_Map2Texture = LoadTexture(Map2Img, renderer);
    m_Map3Texture = LoadTexture(Map3Img, renderer);
    m_Map4Texture = LoadTexture(Map4Img, renderer);
    m_backgroundMapTexture = LoadTexture(backgroundMap, renderer);
    m_Map1PickTexture = LoadTexture(Map1PickImg, renderer);
    m_Map2PickTexture = LoadTexture(Map2PickImg, renderer);
    m_Map3PickTexture = LoadTexture(Map3PickImg, renderer);
    m_Map4PickTexture = LoadTexture(Map4PickImg, renderer);
    m_PlayButtonTexture = LoadTexture(PlayButtonImg, renderer);
    m_OptionsButtonTexture = LoadTexture(OptionsButtonImg, renderer);
    m_ExitButtonTexture = LoadTexture(ExitButtonImg, renderer);
    m_BackButtonTexture = LoadTexture(BackButtonImg, renderer);
}

void Menu::menu()
{

}

void Menu::Choose_Map()
{
    SDL_RenderCopy(renderer, m_backgroundMapTexture, NULL, NULL);
    SDL_RenderCopy(renderer, m_Map1Texture, NULL, &(m_Map1Button));
    SDL_RenderCopy(renderer, m_Map2Texture, NULL, &(m_Map2Button));
    SDL_RenderCopy(renderer, m_Map3Texture, NULL, &(m_Map3Button));
    SDL_RenderCopy(renderer, m_Map4Texture, NULL, &(m_Map4Button));

    if(world.m_mouseIsPressed)
    {
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.x, m_Map1Button))
        {
            world.m_quitScene = true;
            world.m_gameState = GAME;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.x, m_Map2Button))
        {
            world.m_quitScene = true;
            world.m_gameState = GAME;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map3Button))
        {
            world.m_quitScene = true;
            world.m_gameState = GAME;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map4Button))
        {
            world.m_quitScene = true;
            world.m_gameState = GAME;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_BackButtonRect))
        {
            world.m_quitScene = true;
            //world.m_gameState = MENU;
        }
    }

    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map1Button))
    {
        SDL_RenderCopy(renderer, m_Map1PickTexture, NULL, &(m_Map1Button));
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map2Button))
    {
        SDL_RenderCopy(renderer, m_Map2PickTexture, NULL, &(m_Map2Button));
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map3Button))
    {
        SDL_RenderCopy(renderer, m_Map3PickTexture, NULL, &(m_Map3Button));
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_Map4Button))
    {
        SDL_RenderCopy(renderer, m_Map4PickTexture, NULL, &(m_Map4Button));
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_BackButtonRect))
    {
        if(m_BackButtonRect.w <= 39)
        {
            m_BackButtonRect.w += 20;
            m_BackButtonRect.h += 20;
            m_BackButtonRect.x -= 10;
            m_BackButtonRect.y -= 10;
        }
    }
    else
    {
        m_BackButtonRect.w = 39;
        m_BackButtonRect.h = 47;
        m_BackButtonRect.x = 31;
        m_BackButtonRect.y = 40;
    }

    SDL_RenderCopy(renderer, m_BackButtonTexture, NULL, &(m_BackButtonRect));
    SDL_RenderPresent(renderer);
}

void Menu::draw()
{
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, m_menuTexture, NULL, NULL);

    SDL_RenderCopy(renderer, m_PlayButtonTexture, NULL, &(m_playButtonRect));

    SDL_RenderCopy(renderer, m_OptionsButtonTexture, NULL, &(m_OptionsButtonRect));

    SDL_RenderCopy(renderer, m_ExitButtonTexture, NULL, &(m_ExitButtonRect));

    SDL_RenderPresent(renderer);
}

void Menu::update()
{
    if(world.m_mouseIsPressed)
    {
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_playButtonRect))
        {
            world.m_quitScene = true;
            world.m_gameState = MAP_CHOOSING;
        }
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_ExitButtonRect))
        {
            world.m_quitScene = true;
            world.m_gameState = EXIT;
        }
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_playButtonRect))
    {
        if(m_playButtonRect.w <= 442)
        {
            m_playButtonRect.w += 32;
            m_playButtonRect.h += 18;
            m_playButtonRect.x -= 16;
            m_playButtonRect.y -= 9;
        }
    }else{
        m_playButtonRect.w = 400;
        m_playButtonRect.h = 80;
        m_playButtonRect.x = 483;
        m_playButtonRect.y = 302;
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_OptionsButtonRect)){
        if(m_OptionsButtonRect.w <= 410)
        {
            m_OptionsButtonRect.w += 20;
            m_OptionsButtonRect.h += 20;
            m_OptionsButtonRect.x -= 10;
            m_OptionsButtonRect.y -= 10;
        }
    }else{
        m_OptionsButtonRect.x = 483;
        m_OptionsButtonRect.y = 451;
        m_OptionsButtonRect.w = 400;
        m_OptionsButtonRect.h = 80;
    }
    if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_ExitButtonRect))
    {
        if(m_ExitButtonRect.w <= 57)
        {
            m_ExitButtonRect.w += 20;
            m_ExitButtonRect.h += 20;
            m_ExitButtonRect.x -= 10;
            m_ExitButtonRect.y -= 10;
        }
    }else{
        m_ExitButtonRect.x = 1283;
        m_ExitButtonRect.y = 40;
        m_ExitButtonRect.w = 47;
        m_ExitButtonRect.h = 47;
    }

}
