#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <chrono>
#include "Engine.h"

using namespace std;

class Animator
{
    public:
        Animator();
        virtual ~Animator();

        SDL_Rect* m_objRect;
        SDL_Renderer* m_renderer;

        ///TO-DO make loop animation
        unsigned short m_duration;
        SDL_Texture* m_sprites;
        SDL_Rect m_srcRect;
        unsigned short m_frames;
        double m_timePerFrame;

        void initAnimation(string configFile, SDL_Rect* objRect);
        void start();
        bool update();
        void draw();
    protected:

    private:
        chrono::time_point<chrono::steady_clock> m_lastFrameTime = chrono::steady_clock::now();
        chrono::duration<double> diff;
};

#endif // ANIMATOR_H
