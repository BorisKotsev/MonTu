#ifndef SQUAD_H
#define SQUAD_H

#include <SDL2/SDL.h>
#include "defines.h"

class Squad
{
    public:
        Squad();
        virtual ~Squad();

        SDL_Texture* m_objectTexture;

        OWNER m_owner;
        short int m_maxHealth;
        short int m_health;
        short int m_speed;
        short int m_attackRange;
        short int m_attackDamage;
        short int m_units;


    protected:

    private:
};

#endif // SQUAD_H
