#ifndef DEFINES_H
#define DEFINES_H

#include <SDL2/SDL.h>
#include <cmath>

enum OWNER
{
    NONE = 0,
    PLAYER1 = 1,
    PLAYER2 = 2
};

enum GAME_STATE
{
    NOSCENE = 0,
    MENU = 1,
    PICK_BAN = 2,
    GAME = 3,
    STATS = 4
};

enum SQUAD
{
    NOSQUAD = 0,
    ARCHER = 1,
    WARRIOR = 2
};

enum ICON_STATE
{
    AVAILABLE = 0,
    BANNED = 1,
    PICKED = 2
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
};
#endif // DEFINES_H
