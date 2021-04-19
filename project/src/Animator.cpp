#include "Animator.h"
#include "World.h"
#include <fstream>

using namespace std;

extern World world;

Animator::Animator()
{
    //ctor
}

Animator::~Animator()
{

}

void Animator::initAnimation(string configFile, SDL_Rect* objRect)
{
    configFile = "config//Animations//" + configFile;

    fstream stream;
    string tmp;
    string spritesLocation;
    unsigned short numberOfSprites;

    stream.open(configFile);

    stream >> tmp >> m_duration;
    stream >> tmp >> m_frames;
    stream >> tmp >> m_srcRect.w >> m_srcRect.h;
    stream >> tmp >> spritesLocation;

    stream.close();

    m_renderer = world.m_main_renderer;

    m_sprites = LoadTexture(spritesLocation, m_renderer);

    m_objRect = objRect;

    m_timePerFrame = (double) m_duration / (double)m_frames;
}

void Animator::start()
{
    m_lastFrameTime = chrono::steady_clock::now();
}

bool Animator::update()
{
    diff = chrono::steady_clock::now() - m_lastFrameTime;

    if(diff.count() > m_timePerFrame)
    {
        m_srcRect.x += m_srcRect.w;

        m_lastFrameTime = chrono::steady_clock::now();
        if(diff.count() > m_duration)
        {
            return false;
        }
    }

    return true;
}

void Animator::draw()
{
    SDL_RenderCopy(m_renderer, m_sprites, &m_srcRect, m_objRect);
}
