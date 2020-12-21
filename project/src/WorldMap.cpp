#include "WorldMap.h"
#include "World.h"

extern World world;

WorldMap::WorldMap()
{
    //ctor
}

WorldMap::~WorldMap()
{
    //dtor
}

void WorldMap::init(SDL_Renderer* renderer, string configFile)
{
    srand(time(NULL));

    configFile = "config\\" + configFile;
    fstream stream;

    string tmp;

    vector <string> MapImg;
    string nextButtonImg;
    string buff;

    stream.open(configFile.c_str());

    stream >> tmp >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);
    stream >> buff;
    MapImg.push_back(buff);

    stream >> tmp >> nextButtonImg;
    stream >> tmp >> nextButtonRect.x >> nextButtonRect.y >> nextButtonRect.w >> nextButtonRect.h;
    stream >> tmp >> zoomMulti;
    stream >> tmp >> maxZoom.w >> maxZoom.h;
    stream >> tmp >> minZoom.w >> minZoom.h;

    stream.close();

    mapPieces.push_back(LoadTexture(MapImg[0], renderer));
    mapPieces.push_back(LoadTexture(MapImg[1], renderer));
    mapPieces.push_back(LoadTexture(MapImg[2], renderer));
    mapPieces.push_back(LoadTexture(MapImg[3], renderer));
    NextButtonTexture = LoadTexture(nextButtonImg, renderer);

    moveUp = SDL_SCANCODE_W;
    moveDown = SDL_SCANCODE_S;
    moveLeft = SDL_SCANCODE_A;
    moveRight = SDL_SCANCODE_D;

    cameraRect.x = 0;
    cameraRect.y = 0;
    cameraRect.w = world.m_SCREEN_WIDTH;
    cameraRect.h = world.m_SCREEN_HEIGHT;
}

void WorldMap::update()
{
    oldX = cameraRect.x;
    oldY = cameraRect.y;

    const Uint8 *state = SDL_GetKeyboardState(NULL);

    SDL_PollEvent(&event);

    if (state[moveUp])
    {
        cameraRect.y -= 3;
    }
    if (state[moveDown])
    {
        cameraRect.y += 3;
    }
    if (state[moveLeft])
    {
        cameraRect.x -= 3;
    }
    if (state[moveRight])
    {
        cameraRect.x += 3;
    }

    zoom();

    if(mouseIsPressed)
    {
        if(checkForMouseCollision(mouse.x, mouse.y, nextButtonRect))
        {
            world.m_quitScene = true;
            world.m_gameState = MAP_CHOOSING;
        }
    }

    framer();
}

void WorldMap::draw(SDL_Renderer* renderer)
{
    SDL_RenderClear(renderer);

    drawMap(renderer);

    SDL_RenderCopy(renderer, NextButtonTexture, NULL, &(nextButtonRect));

    SDL_RenderPresent(renderer);
}

void WorldMap::input()
{
    event.type = SDLK_UNKNOWN;
    mouseIsPressed = false;

    SDL_PollEvent(&event);

    if (event.type == SDL_MOUSEMOTION)
    {
        SDL_GetGlobalMouseState(&(mouse.x), &(mouse.y));
    }
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        mouseIsPressed = true;
    }
}

void WorldMap::framer()
{
    if(cameraRect.w < minZoom.w || cameraRect.h < minZoom.h)
    {
        cameraRect.x = oldX;
        cameraRect.y = oldY;
        cameraRect.w = minZoom.w;
        cameraRect.h = minZoom.h;
    }
    if(cameraRect.w > maxZoom.w || cameraRect.h > maxZoom.h)
    {
        cameraRect.x = oldX;
        cameraRect.y = oldY;
        cameraRect.w = maxZoom.w;
        cameraRect.h = maxZoom.h;
    }
    if(cameraRect.x + cameraRect.w > 3840)
    {
        cameraRect.x = 3840 - cameraRect.w;
    }
    if(cameraRect.x < 0)
    {
        cameraRect.x = 0;
    }
    if(cameraRect.y + cameraRect.h > 2160)
    {
        cameraRect.y = 2160 - cameraRect.h;
    }
    if(cameraRect.y < 0)
    {
        cameraRect.y = 0;
    }
}

void WorldMap::zoom()
{
    if(event.type == SDL_MOUSEWHEEL)
    {
        cameraRect.w -= zoomMulti * event.wheel.y * 16.0;
        cameraRect.h -= zoomMulti * event.wheel.y * 9.0;

        cameraRect.x += zoomMulti * event.wheel.y * 8.0;
        cameraRect.y += zoomMulti * event.wheel.y * 4.5;

        cameraCenter.x = cameraRect.x + cameraRect.w / 2;
        cameraCenter.y = cameraRect.y + cameraRect.h / 2;

        direction.x = mouse.x - cameraCenter.x;
        direction.y = mouse.y - cameraCenter.y;

        directionAngle = returnAngleByCoordinates(direction);

        //cameraRect.x += sin(directionAngle * PI / 180) * 32.0;
        //cameraRect.y -= cos(directionAngle * PI / 180) * 18.0;
    }
}

vector <SDL_Texture*> WorldMap::drawMap(SDL_Renderer* renderer)
{
    cameraDstRect1.x = 0;
    cameraDstRect1.y = 0;
    cameraDstRect1.w = 1920 - cameraRect.x;
    cameraDstRect1.h = 1080 - cameraRect.y;

    cameraSrcRect1.x = cameraRect.x;
    cameraSrcRect1.y = cameraRect.y;
    cameraSrcRect1.w = cameraDstRect1.w;
    cameraSrcRect1.h = cameraDstRect1.h;

    cameraDstRect2.x = cameraDstRect1.w;
    cameraDstRect2.y = 0;
    cameraDstRect2.w = cameraRect.w - cameraDstRect1.w;
    cameraDstRect2.h = cameraDstRect1.h;

    cameraSrcRect2.x = 0;
    cameraSrcRect2.y = cameraRect.y;
    cameraSrcRect2.w = cameraDstRect2.w;
    cameraSrcRect2.h = cameraDstRect2.h;

    cameraDstRect3.x = 0;
    cameraDstRect3.y = cameraDstRect1.h;
    cameraDstRect3.w = cameraDstRect1.w;
    cameraDstRect3.h = cameraRect.h - cameraDstRect1.h;

    cameraSrcRect3.x = cameraRect.x;
    cameraSrcRect3.y = 0;
    cameraSrcRect3.w = cameraDstRect3.w ;
    cameraSrcRect3.h = cameraDstRect3.h;

    cameraDstRect4.x = cameraDstRect2.x;
    cameraDstRect4.y = cameraDstRect3.y;
    cameraDstRect4.w = cameraDstRect2.w;
    cameraDstRect4.h = cameraDstRect3.h;

    cameraSrcRect4.x = 0;
    cameraSrcRect4.y = 0;
    cameraSrcRect4.w = cameraDstRect4.w;
    cameraSrcRect4.h = cameraDstRect4.h;

    //Calculating the src rect


    //cout << cameraSrcRect1.x << " " << cameraSrcRect1.y << " " << cameraSrcRect1.w << " " << cameraSrcRect1.h << endl;
    //cout << cameraDstRect1.x << " " << cameraDstRect1.y << " " << cameraDstRect1.w << " " << cameraDstRect1.h << endl;
    //cout << cameraSrcRect2.x << " " << cameraSrcRect2.y << " " << cameraSrcRect2.w << " " << cameraSrcRect2.h << endl << endl;

    SDL_RenderCopy(renderer, mapPieces[3], &cameraSrcRect1, &cameraDstRect1);
    SDL_RenderCopy(renderer, mapPieces[1], &cameraSrcRect2, &cameraDstRect2);
    SDL_RenderCopy(renderer, mapPieces[1], &cameraSrcRect3, &cameraDstRect3);
    SDL_RenderCopy(renderer, mapPieces[3], &cameraSrcRect4, &cameraDstRect4);
}
