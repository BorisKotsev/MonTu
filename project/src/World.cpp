#include "World.h"

World::World()
{
    m_main_window = NULL;
    m_main_renderer = NULL;
    m_backgroundTexture = NULL;
    m_menuState = NOSCENE;
    m_quitScene = false;
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

    stream.open(configFile.c_str());
    stream >> tmp >> m_SCREEN_WIDTH >> m_SCREEN_HEIGHT;
    stream >> tmp >> backgroundImg;
    stream >> tmp >> m_colls >> m_rows;
    stream.close();

    SDL_Init(SDL_INIT_EVERYTHING);

    m_main_window = SDL_CreateWindow("Montu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_SCREEN_WIDTH, m_SCREEN_HEIGHT, 0);
    m_main_renderer = SDL_CreateRenderer(m_main_window, -1, SDL_RENDERER_ACCELERATED);

    m_configManager.init("config_manager.txt", m_main_renderer);
    m_soundManager.init("SoundManager.txt");

    backgroundImg = "img\\" + backgroundImg;

    SDL_Surface* loadingSurface = SDL_LoadBMP(backgroundImg.c_str());
    m_backgroundTexture = SDL_CreateTextureFromSurface(m_main_renderer, loadingSurface);
    SDL_FreeSurface(loadingSurface);

    initTiles("tileMap.txt");

    m_soundManager.play_sound("General.mp3");
}

void World::update()
{
    selectTile();

    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        ///(*it) -> update(m_main_renderer);
    }

    cleaner();
}

void World::draw()
{
    SDL_RenderClear(m_main_renderer);

    ///SDL_RenderCopy(m_main_renderer, m_backgroundTexture, NULL, NULL);

    for(vector<vector <Tile*> > :: iterator vit = m_tiles.begin(); vit != m_tiles.end(); vit++)
    {
        for(vector<Tile*> :: iterator it = (*vit).begin(); it != (*vit).end(); it++)
        {
            (*it) -> draw(m_main_renderer);
        }
    }
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        ///(*it) -> draw(m_main_renderer);
    }

    SDL_RenderPresent(m_main_renderer);
}

void World::cleaner()
{

}

void World::destroySDL()
{
    m_menuState = NOSCENE;
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
}

void World::menu()
{
    //Draw the menu img

    //Switch scenes if needed
}

void World::pickAndBan()
{

}

void World::initTiles(string configFile)
{
    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;

    int hexagonWidth, hexagonHeight;
    int sizeOfHexagon;

    stream.open(configFile.c_str());
    stream >> tmp >> sizeOfHexagon;
    stream.close();

    hexagonWidth = sqrt(3) * sizeOfHexagon;
    hexagonHeight = 2 * sizeOfHexagon;


    Tile* tile = NULL;
    SDL_Point* buffPoint = NULL;

    for (int r = 0; r < m_rows; r ++)
    {
        m_tiles.push_back(vector<Tile*>());
        for (int c = 0; c < m_colls; c ++)
        {
            tile = new Tile(*(m_configManager.modelTile));
            if(r % 2 == 0)
            {
                tile->m_drawCoordinates.x = c * hexagonWidth;
            }
            else
            {
                tile->m_drawCoordinates.x = hexagonWidth / 2 + c * hexagonWidth;
            }
            tile->m_drawCoordinates.y = r * hexagonHeight * 3 / 4;
            tile->m_mapCoordinates.x = c;
            tile->m_mapCoordinates.y = r;

            CoordinatesToRect(tile->m_drawCoordinates, tile->m_objectRect);
            tile->m_objectRect.w = hexagonWidth;
            tile->m_objectRect.h = hexagonHeight;

            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + hexagonWidth / 2;
            buffPoint->y = tile->m_drawCoordinates.y + 0;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + 0;
            buffPoint->y = tile->m_drawCoordinates.y + hexagonHeight / 4;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + 0;
            buffPoint->y = tile->m_drawCoordinates.y + hexagonHeight / 4 * 3;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + hexagonWidth / 2;
            buffPoint->y = tile->m_drawCoordinates.y + hexagonHeight;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + hexagonWidth;
            buffPoint->y = tile->m_drawCoordinates.y + hexagonHeight / 4 * 3;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + hexagonWidth;
            buffPoint->y = tile->m_drawCoordinates.y + hexagonHeight / 4;
            tile->m_collisionPoints.push_back(buffPoint);

            m_tiles[r].push_back(tile);
        }
    }
}

void World::selectTile()
{
    for(int i = 0; i < m_tiles.size(); i++)
    {
        for(int j = 0; j < m_tiles[i].size(); j++)
        {
            if(m_mouseIsPressed)
            {
                if(isInsideAHexagon(m_tiles[i][j]->m_collisionPoints, LoadPoint(m_mouse)))
                {
                    cout << i << " " << j << endl;
                }
            }
        }
    }
}
