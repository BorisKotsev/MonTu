#ifndef DEFINES_H
#define DEFINES_H

#include <cmath>
#include <vector>
#include <SDL2/SDL.h>

using namespace std;

enum OWNER
{
    NOOWNER = 0,
    PLAYER1 = 1,
    PLAYER2 = 2
};

enum GAME_STATE
{
    NOSCENE = 0,
    MENU = 1,
    PICK_BAN = 2,
    GAME = 3,
    STATS = 4,
    MAP_CHOOSING = 5,
    EXIT = 6
};

enum SQUAD
{
    NOSQUAD = 0,
    ARCHER = 1,
    WARRIOR = 2,
    SPEARMEN = 3,
    CROSSBOWMEN = 4,
    KNIGHTS = 5
};

enum ICON_STATE
{
    AVAILABLE = 0,
    BANNED = 1,
    PICKED = 2
};

enum PLAYER_STAT
{
    NOSTAT = 0,
    MONEY = 1,
    FOOD = 2
};

struct icon
{
    SQUAD m_type;
    ICON_STATE m_iconState;
    SDL_Rect m_rect;
    SDL_Texture* m_texture;
};

struct coordinates
{
    int x;
    int y;

    void operator=(const SDL_Rect& rect)
    {
        x = rect.x;
        y = rect.y;
    }

    void operator=(coordinates coor)
    {
        x = coor.x;
        y = coor.y;
    }

    void operator-(coordinates coor)
    {
        x -= coor.x;
        y -= coor.y;
    }

    void operator+(coordinates coor)
    {
        x += coor.x;
        y += coor.y;
    }

    bool operator==(coordinates coor)
    {
        if(x == coor.x && y == coor.y)
        {
            return true;
        }
        return false;
    }

    bool operator!=(coordinates coor)
    {
        if(x != coor.x && y != coor.y)
        {
            return true;
        }
        return false;
    }
};

struct UI_object
{
    SDL_Texture* objTexture = NULL;
    vector<SDL_Texture*> objAnimation;
    SDL_Rect objRect;

};

struct color
{
    short int r;
    short int g;
    short int b;
};
#endif // DEFINES_H
