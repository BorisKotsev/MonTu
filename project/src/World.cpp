#include "World.h"

World::World()
{
    m_main_window = NULL;
    m_main_renderer = NULL;
    m_backgroundTexture = NULL;
    m_gameState = NOSCENE;
    m_quitScene = false;
    m_selected.x = 0;
    m_selected.y = 0;
    m_cameraOffset.x = 0;
    m_cameraOffset.y = 0;
    m_selectedSquad = NULL;
    m_playerTurn = PLAYER1;
    m_showFillBtn = true;
    m_showAttackTiles = false;
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
    string selectedImg, attackTileImg;
    string menuImg;
    string cursorImg;
    string skipBtnFillImg;
    string skipBtnTransImg;
    string PlayButtonImg;
    string ExitButtonImg;
    string OptionsButtonImg;
    string BackButtonImg;

    stream.open(configFile.c_str());
    stream >> tmp >> m_SCREEN_WIDTH >> m_SCREEN_HEIGHT;
    stream >> tmp >> backgroundImg;
    stream >> tmp >> menuImg;
    stream >> tmp >> m_playButtonRect.x >> m_playButtonRect.y >> m_playButtonRect.w >> m_playButtonRect.h;
    stream >> tmp >> m_colls >> m_rows;
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
    stream >> tmp >> selectedImg;
    stream >> tmp >> attackTileImg;
    stream >> tmp >> cursorImg;
    stream >> tmp >> skipBtnFillImg;
    stream >> tmp >> skipBtnTransImg;
    stream >> tmp >> m_skipTurnFillBtn.objRect.x >> m_skipTurnFillBtn.objRect.y >> m_skipTurnFillBtn.objRect.w >> m_skipTurnFillBtn.objRect.h;
    stream >> tmp >> m_CP1.r >> m_CP1.g >> m_CP1.b;
    stream >> tmp >> m_CP2.r >> m_CP2.g >> m_CP2.b;
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
    m_popUpWriter.init("PopUpWriter.txt", m_main_renderer);
    m_armyManager.init("armyManager.txt");
    m_playerStatsManager.init("mainStats.txt");

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
    m_selectedTileUI.objTexture = LoadTexture(selectedImg, m_main_renderer);
    m_attackTileUI.objTexture = LoadTexture(attackTileImg, m_main_renderer);
    m_skipTurnFillBtn.objTexture = LoadTexture(skipBtnFillImg, m_main_renderer);
    m_skipTurnTransBtn.objTexture = LoadTexture(skipBtnTransImg, m_main_renderer);
    m_PlayButtonTexture = LoadTexture(PlayButtonImg, m_main_renderer);
    m_OptionsButtonTexture = LoadTexture(OptionsButtonImg, m_main_renderer);
    m_ExitButtonTexture = LoadTexture(ExitButtonImg, m_main_renderer);
    m_BackButtonTexture = LoadTexture(BackButtonImg, m_main_renderer);

    m_skipTurnTransBtn.objRect = m_skipTurnFillBtn.objRect;

    /// m_soundManager.play_sound("General.mp3");

    initDirection("directions.txt");


    /// ShowWindow(GetConsoleWindow(), SW_HIDE);



}

void World::initDirection(string configFile)
{
    // Declaring the value of the directions used for the giveNeighbor function
    configFile = "config\\" + configFile;
    fstream stream;

    stream.open(configFile.c_str());

    for(int i = 0; i < 2; i ++)
    {
        for (int j = 0; j < 6; j++)
        {
            stream >> directions[i][j].x >> directions[i][j].y;
        }
    }
    stream.close();
}

void World::initGameSession()
{
    m_startShake = time(NULL);
    coordinates buff;
    buff.x = 23;
    buff.y = 5;
    initSquad(WARRIOR, buff, PLAYER1);
    m_armyManager.deployArmy(PLAYER1);
}

void World::update()
{

    cameraShake();

    selectTile();

    for(vector <Building*> :: iterator it = m_buildings.begin(); it != m_buildings.end(); it++)
    {
        (*it) -> update();
    }

    m_selectedTileUI.objRect = m_tiles[m_selected.y][m_selected.x]->m_objectRect;

    squadActionsCheck();

    if(m_mouseIsPressed)
    {
        if(m_showFillBtn && checkForMouseCollision(m_mouse.x, m_mouse.y, m_skipTurnFillBtn.objRect))
        {
            switchTurn();
        }
    }

    checkForTurnSwitch();

    cleaner();

}

void World::draw()
{
    SDL_RenderClear(m_main_renderer);

    for(vector<vector <Tile*> > :: iterator vit = m_tiles.begin(); vit != m_tiles.end(); vit++)
    {
        for(vector<Tile*> :: iterator it = (*vit).begin(); it != (*vit).end(); it++)
        {
            (*it) -> draw(m_main_renderer);
        }
    }

    for(vector <Building*> :: iterator it = m_buildings.begin(); it != m_buildings.end(); it++)
    {
        (*it) -> draw(m_main_renderer);
        ///A second iteration is needed so bridges and wall sprites (because they will probably be bigger) are not under the ground sprites
        ///Or we could add bridges and walls to a different vector and then iterate it
    }
    SDL_RenderCopy(m_main_renderer, m_selectedTileUI.objTexture, NULL, &(m_selectedTileUI.objRect));

    for(vector <Tile*> :: iterator it = m_availableWalkTiles.begin(); it != m_availableWalkTiles.end(); it++)
    {
        SDL_RenderCopy(m_main_renderer, m_selectedTileUI.objTexture, NULL, &((*it) -> m_objectRect));
    }

    for(vector <Tile*> :: iterator it = m_availableShootTiles.begin(); it != m_availableShootTiles.end(); it++)
    {
        SDL_RenderCopy(m_main_renderer, m_attackTileUI.objTexture, NULL, &((*it) -> m_objectRect));
    }

    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        (*it) -> draw();
    }

    m_popUpWriter.draw(m_tiles[m_selected.y][m_selected.x]->m_objectRect, m_popUpWriter.m_buildingListRect, m_popUpWriter.m_buildingListTexture);

    if (m_showFillBtn)
    {
        SDL_RenderCopy(m_main_renderer, m_skipTurnFillBtn.objTexture, NULL, &(m_skipTurnFillBtn.objRect));
    }

    SDL_RenderPresent(m_main_renderer);
}

void World::cleaner()
{
    for(vector<Building*>::iterator it = m_buildings.begin(); it != m_buildings.end(); it++)
    {
        if((*it) -> m_health <= 0)
        {
            it = m_buildings.erase(it);
            it--;
        }
    }
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

void World::menu()
{
    //Draw the menu img
    SDL_RenderCopy(m_main_renderer, m_menuTexture, NULL, NULL);

    //Switch scenes if needed
    if(m_mouseIsPressed)
    {
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_playButtonRect))
        {
            m_quitScene = true;
            m_gameState = MAP_CHOOSING;
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_ExitButtonRect))
        {
            m_quitScene = true;
            m_gameState = EXIT;
        }
    }

    if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_playButtonRect))
    {
        if(m_playButtonRect.w <= 410)
        {
            m_playButtonRect.w += 20;
            m_playButtonRect.h += 20;
            m_playButtonRect.x -= 10;
            m_playButtonRect.y -= 10;
        }
    }else{
        m_playButtonRect.w = 400;
        m_playButtonRect.h = 80;
        m_playButtonRect.x = 483;
        m_playButtonRect.y = 302;
    }
    if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_OptionsButtonRect)){
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
    if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_ExitButtonRect))
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

    SDL_RenderCopy(m_main_renderer, m_PlayButtonTexture, NULL, &(m_playButtonRect));

    SDL_RenderCopy(m_main_renderer, m_OptionsButtonTexture, NULL, &(m_OptionsButtonRect));

    SDL_RenderCopy(m_main_renderer, m_ExitButtonTexture, NULL, &(m_ExitButtonRect));

    SDL_RenderPresent(m_main_renderer);
}


void World::pickAndBan()
{
    //Check for mouse input and select squad to ban
    if(false)
    {
        ///m_banned.push_back();
    }
}

void World::initTiles(string configFile)
{
    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;

    short int sizeOfHexagon;

    stream.open(configFile.c_str());
    stream >> tmp >> sizeOfHexagon;
    stream.close();

    m_hexagonWidth = sqrt(3) * sizeOfHexagon;
    m_hexagonHeight = 2 * sizeOfHexagon;


    Tile* tile = NULL;
    Building* building = NULL;
    SDL_Point* buffPoint = NULL;

    for (short int r = 0; r < m_rows; r ++)
    {
        m_tiles.push_back(vector<Tile*>());
        for (short int c = 0; c < m_colls; c ++)
        {
            switch(field[c][r])
            {
            case 'G':
                tile = new Tile(*(m_configManager.modelTileGrass), &m_cameraOffset);
                break;
            case 'W':
                tile = new Tile(*(m_configManager.modelTileWater), &m_cameraOffset);
                break;
            case 'M':
                tile = new Tile(*(m_configManager.modelTileMountain), &m_cameraOffset);
                break;
            case 'F':
                tile = new Tile(*(m_configManager.modelTileForest), &m_cameraOffset);
                break;
            case 'D':
                tile = new Tile(*(m_configManager.modelTileDesert), &m_cameraOffset);
                break;
            case 'S':
                tile = new Tile(*(m_configManager.modelTileStone), &m_cameraOffset);
                break;
            case 'V':
                tile = new Tile(*(m_configManager.modelTileVolcano), &m_cameraOffset);
                break;
            case 'L':
                tile = new Tile(*(m_configManager.modelTileLava), &m_cameraOffset);
                break;
            }
            m_tiles[r].push_back(tile);
            /*if ((r == m_rows / 2 && c == m_colls - 1) || (r == m_rows / 2 && c == 3))
            {
                building = new Building(*(m_configManager.modelCastle));
                tile = building;
                if (c == 3)
                {
                    tile->m_owner = PLAYER1;
                }
                else
                {
                    tile->m_owner = PLAYER2;
                }
                m_buildings.push_back(building);
            }
            */
            if(r % 2 == 0)
            {
                tile->m_drawCoordinates.x = c * m_hexagonWidth;
            }
            else
            {
                tile->m_drawCoordinates.x = m_hexagonWidth / 2 + c * m_hexagonWidth;
            }

            tile->m_drawCoordinates.y = r * m_hexagonHeight * 3 / 4;
            tile->m_mapCoordinates.x = c;
            tile->m_mapCoordinates.y = r;

            CoordinatesToRect(tile->m_drawCoordinates, tile->m_objectRect);
            tile->m_objectRect.w = m_hexagonWidth;
            tile->m_objectRect.h = m_hexagonHeight;

            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + m_hexagonWidth / 2;
            buffPoint->y = tile->m_drawCoordinates.y + 0;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + 0;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight / 4;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + 0;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight / 4 * 3;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + m_hexagonWidth / 2;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + m_hexagonWidth;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight / 4 * 3;
            tile->m_collisionPoints.push_back(buffPoint);
            buffPoint = new SDL_Point;
            buffPoint->x = tile->m_drawCoordinates.x + m_hexagonWidth;
            buffPoint->y = tile->m_drawCoordinates.y + m_hexagonHeight / 4;
            tile->m_collisionPoints.push_back(buffPoint);
        }
    }
}

void World::selectTile()
{
    if(m_mouseIsPressed)
    {
        for(short int r = 0; r < m_tiles.size(); r++)
        {
            for(short int c = 0; c < m_tiles[r].size(); c++)
            {
                if(isInsideAHexagon(m_tiles[r][c]->m_collisionPoints, LoadPoint(m_mouse)))
                {
                    m_popUpWriter.m_presentBuildingList = (m_selected.x == c && m_selected.y == r) ? true : false;
                    cout << "selected " << c << " " << r << endl;

                    m_selected.x = c;
                    m_selected.y = r;
                }
            }
        }
    }
}

Tile* World::giveNeighbor(coordinates coor, int direction)
{
    /*
    the value of direction should be the following:
    *short version* - starts from right and goes backwards
    0 - right
    1 - top right
    2 - top left
    3 - left
    4 - down left
    5 - down right
    */

    int evenR = (coor.y % 2 == 0) ? 0 : 1;
    //selects the coordinates that we add to the current position
    coordinates addedCoor = directions[evenR][direction];

    if(coor.y + addedCoor.y < 0 || coor.y + addedCoor.y >= m_rows || coor.x + addedCoor.x < 0 || coor.x + addedCoor.x >= m_colls)
    {
        return NULL;
    }
    return m_tiles[coor.y + addedCoor.y][coor.x + addedCoor.x];
}

bool World::canTravel(Squad* squad, coordinates desiredPosition)
{
    int movementMap[m_rows][m_colls];
    // Takes the position and speed of the Squad
    coordinates position = squad->m_tileTaken->m_mapCoordinates;
    int movement = squad->m_speed;
    // Makes all tiles uncrossable (giving impossible values)
    for (short int r = 0; r < m_rows; r ++)
    {
        for (short int c = 0; c < m_colls; c ++)
        {
            movementMap[r][c] = -1;
        }
    }
    // We need 0 movement for traveling to our own position
    movementMap[position.y][position.x] = 0;

    // Used to find the neighbor with smallest walkingDifficulty
    int minimum;
    int indexBuff;
    coordinates buff;
    bool valueFound = false;

    while(!valueFound)
    {
        valueFound = true;
        for (short int r = 0; r < m_rows; r ++)
        {
            for (short int c = 0; c < m_colls; c ++)
            {
                buff.x = c;
                buff.y = r;

                // If we have found a cell that has assigned value than we continue the algorithm
                if (movementMap[r][c] != -1)
                {
                    for (int i = 0; i < 6; i++)
                    {
                        if(giveNeighbor(buff, i) != NULL)
                        {
                            if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] != -1)
                            {
                                if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] > movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty)
                                {
                                    valueFound = false;
                                    movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                                }
                            }
                            else
                            {
                                valueFound = false;
                                movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                            }
                        }
                    }
                }
            }
        }
    }
    if (movementMap[desiredPosition.y][desiredPosition.x] <= movement)
    {
        movement -= movementMap[desiredPosition.y][desiredPosition.x];
        squad->m_speed = movement;
        if (movement == 0)
        {
            squad->m_moved = true;
            /// cout << "INFO: The squad has " << squad->m_speed << "left \n";
        }
        else
        {
            /// cout << "INFO: The squad has " << squad->m_speed << "left \n";
        }

        // We want to find the road tile by tile
        valueFound = false;
        // Here we store the path
        stack<Tile*> path;
        buff.x = desiredPosition.x;
        buff.y = desiredPosition.y;
        while(!valueFound)
        {
            short int minimumIndex = 0;
            short int minimumValue = 1000;
            for (int i = 0; i < 6; i++)
            {
                if (giveNeighbor(buff, i) != NULL)
                {
                    if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] <
                    minimumValue )
                    {
                        minimumValue = movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x];
                        minimumIndex = i;
                    }
                }
            }
            // We store the road
            path.push(m_tiles[buff.y][buff.x]);
            /// cout << "INFO: Added tile " << buff.y << " " << buff.x << "to the queue \n";
            // Then we move to the next tile
            buff = giveNeighbor(buff, minimumIndex)->m_mapCoordinates;
            if(movementMap[buff.y][buff.x] == 0)
            {
                valueFound = true;
            }
        }
        /// cout << "INFO: Moving is possible " << movementMap[buff.y][buff.x] << " " << movement << endl;
        squad->m_path = path;

        // Check if the squad is surrounded with impossible to walk-through tiles, but has speed
        if(movement != 0)
        {
            bool hasPossibleMove = false;
            for (int i = 0; i < 6; i++)
            {
                if (giveNeighbor(buff, i) != NULL)
                {
                    if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x]
                    < squad->m_speed)
                    {
                        hasPossibleMove = true;
                    }
                }
            }
            if (!hasPossibleMove)
            {
                squad->m_moved = true;
            }
        }
        return true;
    }
    else
    {
        /// cout << "INFO: Moving is not possible " << movementMap[buff.y][buff.x] << " " << movement << endl;
        return false;
    }
}

vector<Tile*> World::showAvailableWalkTiles(Squad* squad)
{
    // take the position and the speed
    coordinates position = squad->m_mapCoor;
    int movement = squad->m_speed;
    // The vector, that we return
    vector<Tile*> returnVector;
    int movementMap[m_rows][m_colls];
    // Makes all tiles uncrossable (giving impossible values)
    for (short int r = 0; r < m_rows; r ++)
    {
        for (short int c = 0; c < m_colls; c ++)
        {
            movementMap[r][c] = -1;
        }
    }
    // We need 0 movement for traveling to our own position
    movementMap[position.y][position.x] = 0;

    // Used to find the neighbor with smallest walkingDifficulty
    int minimum;
    int indexBuff;
    coordinates buff;
    bool valueFound = false;

    while(!valueFound)
    {
        valueFound = true;
        for (short int r = 0; r < m_rows; r ++)
        {
            for (short int c = 0; c < m_colls; c ++)
            {
                // We set the value to true as default, because if we don't change any values in
                // the movementMap than we have found the value of all cells
                buff.x = c;
                buff.y = r;

                // If we have found a cell that has assigned value than we continue the algorithm
                if (movementMap[r][c] != -1)
                {
                    for (int i = 0; i < 6; i++)
                    {
                        if(giveNeighbor(buff, i) != NULL)
                        {
                            if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] != -1)
                            {
                                if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] > movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty)
                                {
                                    valueFound = false;
                                    movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                                }
                            }
                            else
                            {
                                valueFound = false;
                                movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                            }
                        }
                    }
                }

                ///cout << r << " " << c << " " << movementMap[r][c] << endl;
            }
        }

    }
    for (short int r = 0; r < m_rows; r ++)
    {
        for (short int c = 0; c < m_colls; c ++)
        {
            coordinates buff;
            buff.y = r;
            buff.x = c;
            if(movementMap[r][c] <= movement && !(r == position.y && c == position.x) && findSquadByCoor(buff) == NULL)
            {
                returnVector.push_back(m_tiles[r][c]);
            }
        }
    }
    return returnVector;
}

void World::initMap(string configFile)
{
    configFile = "config\\Maps\\" + configFile;

    fstream in_file;

    in_file.open(configFile.c_str());

    int counter = 0;

    for(int r = 0; r < m_rows; r++)
    {
        for(int c = 0; c < m_colls; c++)
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
            initTiles("tileMap.txt");
            m_quitScene = true;
            m_gameState = GAME;
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map2Button))
        {
            initMap("Map2.txt");
            initTiles("tileMap.txt");
            m_quitScene = true;
            m_gameState = GAME;
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map3Button))
        {
            initMap("Map3.txt");
            initTiles("tileMap.txt");
            m_quitScene = true;
            m_gameState = GAME;
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map4Button))
        {
            initMap("Map4.txt");
            initTiles("tileMap.txt");
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

bool World::canShoot(Squad* squad, coordinates targetPosition)
{
    int range = squad->m_attackRange;

    // Converting the logical coordinates to real coordinates
    coordinates logicalPosition = squad->m_mapCoor;
    coordinates position;
    position.x = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.x
                    + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.w) / 2;
    position.y = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.y
                    + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.h) / 2;
    /// cout << position.x << " " << position.y << endl;

    coordinates logicalTargetPosition = targetPosition;
    targetPosition.x = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.x
                        + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.w) / 2;
    targetPosition.y = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.y
                        + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.h) / 2;
    /// cout << targetPosition.x << " " << targetPosition.y << endl;

    // Using the pythagorean theorem, we can check if we have big enough range
    short int a = abs(position.x - targetPosition.x);
    short int b = abs(position.y - targetPosition.y);
    short int c = sqrt(a*a + b*b);

    if(c > range)
    {
        // If we are out of range or if there isn't a squad on this coor than we stop the function
        return false;
    }
    if(findSquadByCoor(logicalPosition) == NULL || findSquadByCoor(logicalPosition)->m_owner == squad->m_owner)
    {
        return false;
    }

    // Using this bool to determine if the function is done
    bool finished = false;
    // The angle used to calculate how we will travel to the target position
    short int rotationAngle;
    // We find the rotation angle by using the fnct in the Engine
    coordinates direction;
    direction.x = targetPosition.x - position.x;
    direction.y = targetPosition.y - position.y;
    rotationAngle = returnAngleByCoordinates(direction);
    /// cout << rotationAngle << endl;
    vector<Tile*> passing;

    while(!finished)
    {
        position.x += (sin(rotationAngle * PI / 180) * 6);
        position.y -= (cos(rotationAngle * PI / 180) * 6);

        for(short int r = 0; r < m_tiles.size(); r++)
        {
            for(short int c = 0; c < m_tiles[r].size(); c++)
            {
                if(isInsideAHexagon(m_tiles[r][c]->m_collisionPoints, LoadPoint(position)))
                {
                    passing.push_back(m_tiles[r][c]);
                    /// cout << logicalTargetPosition.y << " " << logicalTargetPosition.x << endl;
                    if(c == logicalTargetPosition.x && r == logicalTargetPosition.y)
                    {
                        finished = true;
                    }
                }
            }
        }
    }
    return true;
}

vector<Tile*> World::showAvailableShootTiles(Squad* squad)
{
    // the vector that we return
    vector<Tile*> returnVector;
    // buff variable used to pass the coordinates, to the canShoot function
    coordinates buffCoor;
    for (short int r = 0; r < m_rows; r ++)
    {
        for (short int c = 0; c < m_colls; c ++)
        {
            buffCoor.x = c;
            buffCoor.y = r;

            int range = squad->m_attackRange;
            // Converting the logical coordinates to real coordinates
            coordinates logicalPosition = squad->m_mapCoor;
            coordinates position;
            position.x = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.x + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.w) / 2;
            position.y = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.y + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.h) / 2;

            coordinates logicalTargetPosition = buffCoor;
            coordinates targetPosition;
            targetPosition.x = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.x + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.w) / 2;
            targetPosition.y = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.y + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.h) / 2;

            // Using the pythagorean theorem, we can check if we have big enough range
            short int a = abs(position.x - targetPosition.x);
            short int b = abs(position.y - targetPosition.y);
            short int distance = sqrt(a*a + b*b);

            if (distance < range)
            {
                returnVector.push_back(m_tiles[r][c]);
            }
        }
    }
    return returnVector;
}

void World::initSquad(SQUAD type, coordinates mapCoor, OWNER owner)
{
    Squad* squad;
    Tile* tile = m_tiles[mapCoor.y][mapCoor.x];
    switch(type)
    {
        case WARRIOR:
            squad = new Squad(*(m_configManager.modelSquadWarrior), &m_cameraOffset, tile, owner);
            break;
        case ARCHER:
            squad = new Squad(*(m_configManager.modelSquadArcher), &m_cameraOffset, tile, owner);
            break;
        case SPEARMEN:
            squad = new Squad(*(m_configManager.modelSquadSpearmen), &m_cameraOffset, tile, owner);
            break;
        case CROSSBOWMEN:
            squad = new Squad(*(m_configManager.modelSquadCrossbowmen), &m_cameraOffset, tile, owner);
            break;
        case KNIGHTS:
            squad = new Squad(*(m_configManager.modelSquadKnights), &m_cameraOffset, tile, owner);
            break;
        default:
            squad = NULL;
            break;
    }
    if(tile != NULL)
    {
        m_squads.push_back(squad);
    }
}

void World::switchTurn()
{
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        if((*it)->m_owner != m_playerTurn)
        {
            (*it)->m_moved = false;
            (*it)->m_shooted = false;
            (*it)->m_speed = (*it)->m_startSpeed;

        }
        else
        {
            (*it)->m_shooted = true;
            (*it)->m_moved = true;
        }
    }

    if (m_playerTurn == PLAYER1)
    {
        m_playerTurn = PLAYER2;
        m_enemyAI.takeBattlefield();
        m_enemyAI.makeTurn();
        m_enemyAI.returnBattlefield();
        switchTurn();
    }
    else if(m_playerTurn == PLAYER2)
    {
        m_playerTurn = PLAYER1;
    }
}

void World::checkForTurnSwitch()
{
    bool checkForSwitch = true;
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        if((*it)->m_owner == m_playerTurn)
        {
            if(!((*it)->m_moved) || !((*it)->m_shooted))
            {
                checkForSwitch = false;
            }
        }
    }
    if (checkForSwitch)
    {
        switchTurn();
    }
}

Squad* World::findSquadByCoor(coordinates coor)
{
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        if((*it)->m_mapCoor == coor)
        {
            return (*it);
        }
    }
    return NULL;
}

void World::takeDamage(Squad* attacker, Squad* defender)
{
    defender->m_health -= attacker->m_attackDamage;
}

void World::squadActionsCheck()
{
    if(m_mouseIsPressed)
    {
        if (m_selectedSquad != NULL)
        {
            bool seletedASquad = false;
            Squad* oldSquad = m_selectedSquad;
            for(int i = 0; i < m_squads.size(); i++)
            {
                if (m_squads[i]->m_mapCoor == m_selected)
                {
                    m_selectedSquad = m_squads[i];
                    seletedASquad = true;
                }
            }
            if(!seletedASquad)
            {
                m_showFillBtn = true;
                if (m_selectedSquad->m_owner == m_playerTurn)
                {
                    if (m_selectedSquad->m_moved == false)
                    {
                        // Have we clicked on an empty tile or on a squad
                        if (m_selectedSquad == oldSquad)
                        {
                            if (canTravel(m_selectedSquad, m_tiles[m_selected.y][m_selected.x]->m_mapCoordinates))
                            {
                                m_selectedSquad->m_tileTaken = m_tiles[m_selected.y][m_selected.x];
                            }
                            m_availableWalkTiles.clear();
                            m_availableShootTiles.clear();
                            m_selectedSquad = NULL;
                        }
                    }
                    else if(m_selectedSquad->m_shooted == false)
                    {
                        m_availableShootTiles.clear();
                        m_selectedSquad = NULL;
                    }
                }
            }
            else
            {
                if(oldSquad != m_selectedSquad && !(oldSquad->m_shooted) && canShoot(oldSquad, m_selectedSquad->m_mapCoor))
                {
                    takeDamage(oldSquad, m_selectedSquad);
                    m_startShake = time(NULL);
                    oldSquad->m_shooted = true;
                    oldSquad->m_moved = true;
                    m_availableShootTiles.clear();
                    m_availableWalkTiles.clear();
                    m_selectedSquad = NULL;
                }
            }
        }
        else
        {
            bool seletedASquad = false;
            for(int i = 0; i < m_squads.size(); i++)
            {
                if (m_squads[i]->m_mapCoor == m_selected && m_squads[i]->m_owner == m_playerTurn)
                {
                    m_selectedSquad = m_squads[i];
                    seletedASquad = true;
                }
            }
            if(seletedASquad && m_selectedSquad->m_owner == m_playerTurn)
            {
                if(!(m_selectedSquad->m_moved))
                {
                    m_showFillBtn = false;
                    m_showAttackTiles = false;
                    m_availableWalkTiles = showAvailableWalkTiles(m_selectedSquad);
                }
                else if(!(m_selectedSquad->m_shooted))
                {
                    m_showFillBtn = false;
                    m_showAttackTiles = true;
                    m_availableShootTiles = showAvailableShootTiles(m_selectedSquad);
                }
            }
            else if (!seletedASquad)
            {
                m_selectedSquad = NULL;
            }
        }
    }

    // Hide the skip_turn_btn if there is a squad behind it
    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        (*it) -> update();
        if (checkForCollisionBetweenRects(m_skipTurnFillBtn.objRect, (*it)->m_objectRect))
        {
            m_showFillBtn = false;
        }
        if(m_selectedSquad != NULL)
        {
            if(canShoot(m_selectedSquad, (*it)->m_mapCoor) && m_selectedSquad != (*it) && m_availableWalkTiles.size() != 0)
            {
                m_availableShootTiles.push_back(m_tiles[(*it)->m_mapCoor.y][(*it)->m_mapCoor.x]);
            }
        }
    }
}
