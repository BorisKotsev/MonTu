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
    string selectedImg;
    string menuImg;
    string cursorImg;

    stream.open(configFile.c_str());
    stream >> tmp >> m_SCREEN_WIDTH >> m_SCREEN_HEIGHT;
    stream >> tmp >> backgroundImg;
    stream >> tmp >> menuImg;
    stream >> tmp >> m_playButtonRect.x >> m_playButtonRect.y >> m_playButtonRect.w >> m_playButtonRect.h;
    stream >> tmp >> m_colls >> m_rows;
    stream >> tmp >> Map1Img;
    stream >> tmp >> Map2Img;
    stream >> tmp >> Map3Img;
    stream >> tmp >> Map4Img;
    stream >> tmp >> backgroundMap;
    stream >> tmp >> Map1PickImg;
    stream >> tmp >> Map2PickImg;
    stream >> tmp >> Map3PickImg;
    stream >> tmp >> Map4PickImg;
    stream >> tmp >> selectedImg;
    stream >> tmp >> cursorImg;
    stream.close();

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    m_main_window = SDL_CreateWindow("Montu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_SCREEN_WIDTH, m_SCREEN_HEIGHT, 0);
    m_main_renderer = SDL_CreateRenderer(m_main_window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetWindowFullscreen(m_main_window, SDL_WINDOW_FULLSCREEN);

    m_configManager.init("config_manager.txt", m_main_renderer);
    m_soundManager.init("SoundManager.txt");
    m_pickAndBan.init("pick_And_Ban.txt", m_main_renderer);
    m_popUpWriter.init("PopUpWriter.txt", m_main_renderer);


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

void World::update()
{
    cameraShake();

    selectTile();

    m_selectedTileUI.objRect = m_tiles[m_selected.y][m_selected.x]->m_objectRect;

    if(m_mouseIsPressed)
    {
        if (m_selectedSquad != NULL)
        {
            bool seletedASquad = false;
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
                if (canTravel(m_selectedSquad, m_tiles[m_selected.y][m_selected.x]->m_mapCoordinates))
                {
                    m_selectedSquad->m_tileTaken = m_tiles[m_selected.y][m_selected.x];
                }
                m_availableTiles.clear();
                m_selectedSquad = NULL;
            }
        }
        else
        {
            bool seletedASquad = false;
            for(int i = 0; i < m_squads.size(); i++)
            {
                if (m_squads[i]->m_mapCoor == m_selected)
                {
                    m_selectedSquad = m_squads[i];
                    seletedASquad = true;
                }
            }
            if(seletedASquad)
            {
                m_availableTiles = showAvailableTiles(m_selectedSquad);
            }
        }
    }

    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        (*it) -> update();
    }

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

    SDL_RenderCopy(m_main_renderer, m_selectedTileUI.objTexture, NULL, &(m_selectedTileUI.objRect));

    for(vector <Squad*> :: iterator it = m_squads.begin(); it != m_squads.end(); it++)
    {
        (*it) -> draw();
    }

    for(vector <Tile*> :: iterator it = m_availableTiles.begin(); it != m_availableTiles.end(); it++)
    {
        SDL_RenderCopy(m_main_renderer, m_selectedTileUI.objTexture, NULL, &((*it) -> m_objectRect));
    }

    m_popUpWriter.draw(m_tiles[m_selected.y][m_selected.x]->m_objectRect, m_popUpWriter.m_buildingListRect, m_popUpWriter.m_buildingListTexture);

    SDL_RenderPresent(m_main_renderer);
}

void World::cleaner()
{

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
    }
    //Print
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

    short int hexagonWidth, hexagonHeight;
    short int sizeOfHexagon;

    stream.open(configFile.c_str());
    stream >> tmp >> sizeOfHexagon;
    stream.close();

    hexagonWidth = sqrt(3) * sizeOfHexagon;
    hexagonHeight = 2 * sizeOfHexagon;


    Tile* tile = NULL;
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
    for(short int r = 0; r < m_tiles.size(); r++)
    {
        for(short int c = 0; c < m_tiles[r].size(); c++)
        {
            if(m_mouseIsPressed)
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
    2 - left right
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
    coordinates position = squad->m_mapCoor;
    int movement = squad->m_startSpeed;
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
        // We want to find the road tile by tile
        valueFound = false;
        // Here we store the path
        vector<coordinates> path;
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
            path.push_back(buff);
            // Then we move to the next tile
            buff = giveNeighbor(buff, minimumIndex)->m_mapCoordinates;
            if(movementMap[buff.y][buff.x] == 0)
            {
                valueFound = true;
            }
        }
        /// cout << "INFO: Moving is possible " << movementMap[buff.y][buff.x] << " " << movement << endl;
        squad->m_path = path;
        return true;
    }
    else
    {
        /// cout << "INFO: Moving is not possible " << movementMap[buff.y][buff.x] << " " << movement << endl;
        return false;
    }
}

vector<Tile*> World::showAvailableTiles(Squad* squad)
{
    // take the position and the speed
    coordinates position = squad->m_mapCoor;
    int movement = squad->m_startSpeed;
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
            if(movementMap[r][c] <= movement && !(r == position.y && c == position.x))
            {
                returnVector.push_back(m_tiles[r][c]);
            }
        }
    }
    return returnVector;
    cout << "INFO: THE VALUES OF m_availableTiles have been set \n";
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
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map2Button))
        {
            initMap("Map2.txt");
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map3Button))
        {
            initMap("Map3.txt");
        }
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_pickAndBan.m_Map4Button))
        {
            initMap("Map4.txt");
        }

        initTiles("tileMap.txt");
        m_quitScene = true;
        m_gameState = GAME;
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

    SDL_RenderPresent(m_main_renderer);

}

bool World::canShoot(coordinates position, coordinates targetPosition, short int range)
{
    // Converting the logical coordinates to real coordinates
    coordinates logicalPosition = position;
    position.x = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.x + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.w) / 2;
    position.y = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.y + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.h) / 2;
    /// cout << position.x << " " << position.y << endl;
    coordinates logicalTargetPosition = targetPosition;
    targetPosition.x = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.x + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.w) / 2;
    targetPosition.y = m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.y + (m_tiles[logicalTargetPosition.y][logicalTargetPosition.x]->m_objectRect.h) / 2;
    /// cout << targetPosition.x << " " << targetPosition.y << endl;

    // Using the pythagorean theorem, we can check if we have big enough range
    short int a = abs(position.x - targetPosition.x);
    short int b = abs(position.y - targetPosition.y);
    short int c = sqrt(a^2 + b^2);

    if(c > range)
    {
        // If we are out of range than we stop the function
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

void World::initSquad(SQUAD type, coordinates mapCoor, OWNER owner)
{
    Squad* squad;
    Tile* tile = m_tiles[mapCoor.y][mapCoor.x];
    switch(type)
    {
        case WARRIOR:
            squad = new Squad(*(m_configManager.modelSquadWarrior), &m_cameraOffset, tile, owner);
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
