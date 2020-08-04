#include "World.h"

World::World()
{
    m_main_window = NULL;
    m_main_renderer = NULL;
    m_backgroundTexture = NULL;
    m_gameState = NOSCENE;
    m_quitScene = false;
    m_selected.x = 3;
    m_selected.y = 3;
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
    string menuImg;

    stream.open(configFile.c_str());
    stream >> tmp >> m_SCREEN_WIDTH >> m_SCREEN_HEIGHT;
    stream >> tmp >> backgroundImg;
    stream >> tmp >> menuImg;
    stream >> tmp >> m_playButtonRect.x >> m_playButtonRect.y >> m_playButtonRect.w >> m_playButtonRect.h;
    stream >> tmp >> m_colls >> m_rows;
    stream.close();

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    m_main_window = SDL_CreateWindow("Montu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_SCREEN_WIDTH, m_SCREEN_HEIGHT, 0);
    m_main_renderer = SDL_CreateRenderer(m_main_window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetWindowFullscreen(m_main_window, SDL_WINDOW_FULLSCREEN);

    m_configManager.init("config_manager.txt", m_main_renderer);
    m_soundManager.init("SoundManager.txt");
    m_pickAndBan.init("pick_And_Ban.txt", m_main_renderer);

    m_backgroundTexture = LoadTexture(backgroundImg, m_main_renderer);
    m_menuTexture = LoadTexture(menuImg, m_main_renderer);

    Choose_Map();

    initTiles("tileMap.txt");

    /// m_soundManager.play_sound("General.mp3");

    initDirection("directions.txt");

    coordinates buff1;
    coordinates buff2;
    buff1.x = 4;
    buff1.y = 3;
    buff2.x = 0;
    buff2.y = 1;
    cout << canShoot(buff1, buff2) << endl;

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

    coordinates buff;
    buff.x = m_playButtonRect.x + 115;
    buff.y = m_playButtonRect.y + 10;
    write("PLAY", buff, m_main_renderer, 40);

    //Switch scenes if needed
    if(m_mouseIsPressed)
    {
        if(checkForMouseCollision(m_mouse.x, m_mouse.y, m_playButtonRect))
        {
            m_quitScene = true;
            m_gameState = GAME;
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
                tile = new Tile(*(m_configManager.modelTileGrass));
                break;
            case 'W':
                tile = new Tile(*(m_configManager.modelTileWater));
                break;
            case 'M':
                tile = new Tile(*(m_configManager.modelTileMountain));
                break;
            case 'F':
                tile = new Tile(*(m_configManager.modelTileForest));
                break;
            case 'D':
                tile = new Tile(*(m_configManager.modelTileDesert));
                break;
            case 'S':
                tile = new Tile(*(m_configManager.modelTileStone));
                break;
            case 'V':
                tile = new Tile(*(m_configManager.modelTileVolcano));
                break;
            case 'L':
                tile = new Tile(*(m_configManager.modelTileLava));
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
    for(short int i = 0; i < m_tiles.size(); i++)
    {
        for(short int j = 0; j < m_tiles[i].size(); j++)
        {
            if(m_mouseIsPressed)
            {
                if(isInsideAHexagon(m_tiles[i][j]->m_collisionPoints, LoadPoint(m_mouse)))
                {
                    cout << "selected " << i << " " << j << endl;
                    m_selected.x = i;
                    m_selected.y = j;
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

bool World::canTravel(coordinates position, coordinates desiredPosition, int movement)
{
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
                            /// cout << " Neighbor value: " << movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] << " ";
                            if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] != -1)
                            {
                                if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] > movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty)
                                {
                                    movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                                }
                            }
                            else
                            {
                                movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] = movementMap[r][c] + giveNeighbor(buff, i)->m_walkDifficulty;
                            }
                        }
                    }
                }
                /// cout << r << " " << c << " " << movementMap[r][c] << endl;
                if(c == desiredPosition.x && r == desiredPosition.y && movementMap[r][c] != -1)
                {
                    valueFound = true;
                    if (movementMap[r][c] <= movement)
                    {
                        // We want to find the road tile by tile
                        valueFound = false;
                        // Here we store the path
                        vector<coordinates> path;
                        buff.x = c;
                        buff.y = r;
                        while(!valueFound)
                        {
                            short int minimumIndex = 0;
                            short int minimumValue = movementMap[giveNeighbor(buff, minimumIndex)->m_mapCoordinates.y][giveNeighbor(buff, minimumIndex)->m_mapCoordinates.x];
                            for (int i = 0; i < 6; i++)
                            {
                                if(movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x] <
                                   minimumValue)
                                {
                                    minimumValue = movementMap[giveNeighbor(buff, i)->m_mapCoordinates.y][giveNeighbor(buff, i)->m_mapCoordinates.x];
                                    minimumIndex = i;
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
                        for(int i = 0; i < path.size(); i++)
                        {
                            cout << path[i].x << " " << path[i].y << endl;
                        }
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
        }
    }
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
    int random_number;
    random_number = rand() % 4 + 1;
    initMap("Map" + to_string(random_number) + ".txt");
}

bool World::canShoot(coordinates position, coordinates targetPosition)
{
    coordinates logicalPosition = position;
    position.x = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.x + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.w) / 2;
    position.y = m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.y + (m_tiles[logicalPosition.y][logicalPosition.x]->m_objectRect.h) / 2;
    cout << position.x << " " << position.y << endl;
    coordinates logicalTargetostion = targetPosition;
    targetPosition.x = m_tiles[logicalTargetostion.y][logicalTargetostion.x]->m_objectRect.x + (m_tiles[logicalTargetostion.y][logicalTargetostion.x]->m_objectRect.w) / 2;
    targetPosition.y = m_tiles[logicalTargetostion.y][logicalTargetostion.x]->m_objectRect.y + (m_tiles[logicalTargetostion.y][logicalTargetostion.x]->m_objectRect.h) / 2;
    cout << targetPosition.x << " " << targetPosition.y << endl;

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
                     cout << r << " " << c << endl;
                    /// cout << logicalTargetostion.y << " " << logicalTargetostion.x << endl;
                    if(c == logicalTargetostion.x && r == logicalTargetostion.y)
                    {
                        finished = true;
                    }
                }
            }
        }
    }
    return true;
}
