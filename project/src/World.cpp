#include "World.h"

World::World()
{
    m_main_window = NULL;
    m_main_renderer = NULL;
    m_backgroundTexture = NULL;
    m_gameState = NOSCENE;
    m_quitScene = false;
    m_cameraOffset.x = 0;
    m_cameraOffset.y = 0;
}

World::~World()
{
    //dtor
}

void World::initSDL(string configFile)
{

    srand(time(NULL));

    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;

    string backgroundImg;
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
    string cursorImg;
    string PlayButtonImg;
    string ExitButtonImg;
    string OptionsButtonImg;
    string BackButtonImg;

    stream.open(configFile.c_str());
    stream >> tmp >> m_SCREEN_WIDTH >> m_SCREEN_HEIGHT;
    stream >> tmp >> backgroundImg;
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
    stream >> tmp >> m_CP1.r >> m_CP1.g >> m_CP1.b;
    stream >> tmp >> m_CP2.r >> m_CP2.g >> m_CP2.b;
    stream >> tmp >> cursorImg;

    stream.close();

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    m_main_window = SDL_CreateWindow("Montu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_SCREEN_WIDTH, m_SCREEN_HEIGHT, 0);
    m_main_renderer = SDL_CreateRenderer(m_main_window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetWindowFullscreen(m_main_window, SDL_WINDOW_FULLSCREEN);

    m_healthManager.init("health_manager.txt", m_main_renderer);
    m_configManager.init("config_manager.txt", m_main_renderer, &m_healthManager);
    m_soundManager.init("SoundManager.txt");
    m_pickAndBan.init("pick_And_Ban.txt", m_main_renderer);
    m_playerStatsManager.init("mainStats.txt");
    m_battle.initBattle("battle_manager.txt");
    m_menu.init("menu.txt");

    cursorImg = "img\\" + cursorImg;
    SDL_Surface* loadSurface = SDL_LoadBMP(cursorImg.c_str());
    m_cursor = SDL_CreateColorCursor(loadSurface, 10, 5);
    SDL_SetCursor(m_cursor);

    m_backgroundTexture = LoadTexture(backgroundImg, m_main_renderer);
    m_menuTexture = LoadTexture(menuImg, m_main_renderer);
    m_Map1Texture = LoadTexture(Map1Img, m_main_renderer);
    m_Map2Texture = LoadTexture(Map2Img, m_main_renderer);
    m_Map3Texture = LoadTexture(Map3Img, m_main_renderer);
    m_Map4Texture = LoadTexture(Map4Img, m_main_renderer);
    m_backgroundMapTexture = LoadTexture(backgroundMap, m_main_renderer);
    m_Map1PickTexture = LoadTexture(Map1PickImg, m_main_renderer);
    m_Map2PickTexture = LoadTexture(Map2PickImg, m_main_renderer);
    m_Map3PickTexture = LoadTexture(Map3PickImg, m_main_renderer);
    m_Map4PickTexture = LoadTexture(Map4PickImg, m_main_renderer);
    m_PlayButtonTexture = LoadTexture(PlayButtonImg, m_main_renderer);
    m_OptionsButtonTexture = LoadTexture(OptionsButtonImg, m_main_renderer);
    m_ExitButtonTexture = LoadTexture(ExitButtonImg, m_main_renderer);
    m_BackButtonTexture = LoadTexture(BackButtonImg, m_main_renderer);


    /// m_soundManager.play_sound("General.mp3");


    /// ShowWindow(GetConsoleWindow(), SW_HIDE);

}


void World::destroySDL()
{
    m_gameState = NOSCENE;
    SDL_DestroyTexture(m_backgroundTexture);
    SDL_DestroyRenderer(m_main_renderer);
    SDL_DestroyWindow(m_main_window);
}

void World::input()
{
    m_event.type = SDLK_UNKNOWN;
    m_mouseIsPressed = false;

    SDL_PollEvent(&m_event);

    if (m_event.type == SDL_MOUSEMOTION)
    {
        SDL_GetGlobalMouseState(&(m_mouse.x), &(m_mouse.y));
    }
    if (m_event.type == SDL_MOUSEBUTTONDOWN)
    {
        m_mouseIsPressed = true;
    }
    ///cout << m_mouse.x << " " << m_mouse.y << endl;
}

void World::cameraShake()
{
    if (m_startShake + m_cameraShakeDuration > time(NULL))
    {
        // We pick a number random for the magnitude and than we decide if we add, or if we subtract
        // this number from the cameraOffset
        m_cameraOffset.x += (rand() % m_cameraShakeMagnitude) * (rand() % 2 == 0 ? 1 : -1);
        m_cameraOffset.y += (rand() % m_cameraShakeMagnitude) * (rand() % 2 == 0 ? 1 : -1);
        ///cout << m_cameraOffset.x << " " << m_cameraOffset.y << endl;
    }
    else
    {
        m_cameraOffset.x = 0;
        m_cameraOffset.y = 0;
    }
}

void World::initSession(GAME_STATE state)
{
    if(state == PICK_BAN)
    {
        m_available.push_back(ARCHER);
        m_available.push_back(WARRIOR);
    }
}

void World::pickAndBan()
{
    //Check for mouse input and select squad to ban
    if(false)
    {
        ///m_banned.push_back();
    }
}

void World::initMap(string configFile)
{
    configFile = "config\\Maps\\" + configFile;

    fstream in_file;

    in_file.open(configFile.c_str());

    int counter = 0;
    for(int r = 0; r < m_battle.m_rows; r++)
    {
        for(int c = 0; c < m_battle.m_colls; c++)
        {
            in_file >> field[c][r];
        }
    }

    in_file.close();
}

void World::Choose_Map()
{
    SDL_RenderCopy(m_main_renderer, m_backgroundMapTexture, NULL, NULL);
    SDL_RenderCopy(m_main_renderer, m_Map1Texture, NULL, &(m_pickAndBan.m_Map1Button));
    SDL_RenderCopy(m_main_renderer, m_Map2Texture, NULL, &(m_pickAndBan.m_Map2Button));
    SDL_RenderCopy(m_main_renderer, m_Map3Texture, NULL, &(m_pickAndBan.m_Map3Button));
    SDL_RenderCopy(m_main_renderer, m_Map4Texture, NULL, &(m_pickAndBan.m_Map4Button));

    if(m_mouseIsPressed)
    {
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map1Button))
        {
            initMap("Map1.txt");
            m_battle.initTiles("tileMap.txt");
            m_quitScene = true;
            m_gameState = GAME;
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map2Button))
        {
            initMap("Map2.txt");
            m_battle.initTiles("tileMap.txt");
            m_quitScene = true;
            m_gameState = GAME;
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map3Button))
        {
            initMap("Map3.txt");
            m_battle.initTiles("tileMap.txt");
            m_quitScene = true;
            m_gameState = GAME;
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map4Button))
        {
            initMap("Map4.txt");
            m_battle.initTiles("tileMap.txt");
            m_quitScene = true;
            m_gameState = GAME;
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_BackButtonRect))
        {
            m_quitScene = true;
            m_gameState = MENU;
        }
    }

    if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map1Button))
    {
        SDL_RenderCopy(m_main_renderer, m_Map1PickTexture, NULL, &(m_pickAndBan.m_Map1Button));
    }
    if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map2Button))
    {
        SDL_RenderCopy(m_main_renderer, m_Map2PickTexture, NULL, &(m_pickAndBan.m_Map2Button));
    }
    if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map3Button))
    {
        SDL_RenderCopy(m_main_renderer, m_Map3PickTexture, NULL, &(m_pickAndBan.m_Map3Button));
    }
    if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map4Button))
    {
        SDL_RenderCopy(m_main_renderer, m_Map4PickTexture, NULL, &(m_pickAndBan.m_Map4Button));
    }
    if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_BackButtonRect))
    {
        if(m_BackButtonRect.w <= 39)
        {
            m_BackButtonRect.w += 20;
            m_BackButtonRect.h += 20;
            m_BackButtonRect.x -= 10;
            m_BackButtonRect.y -= 10;
        }
    }else{
        m_BackButtonRect.w = 39;
        m_BackButtonRect.h = 47;
        m_BackButtonRect.x = 31;
        m_BackButtonRect.y = 40;
    }

    SDL_RenderCopy(m_main_renderer, m_BackButtonTexture, NULL, &(m_BackButtonRect));

    SDL_RenderPresent(m_main_renderer);

}

