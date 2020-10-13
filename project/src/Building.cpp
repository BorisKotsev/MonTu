#include "Building.h"
#include "World.h"

extern World world;

Building::Building()
{
    //ctor
}

Building::Building(const Building& model):Tile(model)
{
    m_groundTexture = model.m_groundTexture;
    m_ownerTextureP1 = model.m_ownerTextureP1;
    m_ownerTextureP2 = model.m_ownerTextureP2;
    m_buildTexture = model.m_buildTexture;
    m_damageTexture = model.m_damageTexture;
    m_height = model.m_height;
    m_width = model.m_width;
    m_imgRows = model.m_imgRows;
    m_imgCols = model.m_imgCols;
    m_adjustment = model.m_adjustment;
    m_rotationAngle = model.m_rotationAngle;
    m_drawState = model.m_drawState;
    m_maxHealth = model.m_maxHealth;
    m_healthPerState = model.m_healthPerState;
    m_capacity = model.m_capacity;
    m_states = model.m_states;
    m_maxState = model.m_maxState;
    m_duration = model.m_duration;
    m_moralImpact = model.m_moralImpact;
    m_healthImpact = model.m_healthImpact;
    m_damageImpact = model.m_damageImpact;
    m_effectPeriod = model.m_effectPeriod;
    m_moralImpactEffect = model.m_moralImpactEffect;
    m_healthImpactEffect = model.m_healthImpactEffect;
    m_damageImpactEffect = model.m_damageImpactEffect;
    m_currState = model.m_currState;
    m_health = model.m_health;
    m_usedCapacity = model.m_usedCapacity;
    m_initializeGround = model.m_initializeGround;
    m_constructing = model.m_constructing;
    m_currTimePoint = steady_clock::now();
}

Building::~Building()
{
    SDL_DestroyTexture(m_objectTexture);
    SDL_DestroyTexture(m_groundTexture);
    SDL_DestroyTexture(m_ownerTextureP1);
    SDL_DestroyTexture(m_ownerTextureP2);
    SDL_DestroyTexture(m_buildTexture);
    SDL_DestroyTexture(m_damageTexture);
}

void Building::load(ifstream& stream)
{
    string tmp;
    stream >> tmp >> m_groundImg;
    stream >> tmp >> m_ownerImgP1;
    stream >> tmp >> m_ownerImgP2;
    stream >> tmp >> m_buildImg;
    stream >> tmp >> m_damageImg;
    stream >> tmp >> m_height;
    stream >> tmp >> m_width;
    stream >> tmp >> m_imgRows >> m_imgCols;
    stream >> tmp >> m_adjustment;
    stream >> tmp >> m_rotationAngle;
    stream >> tmp >> m_maxHealth;
    stream >> tmp >> m_capacity;
    stream >> tmp >> m_states;
    stream >> tmp >> m_maxState;
    stream >> tmp >> m_duration;
    stream >> tmp >> m_moralImpact;
    stream >> tmp >> m_healthImpact;
    stream >> tmp >> m_damageImpact;
    stream >> tmp >> m_effectPeriod;
    stream >> tmp >> m_moralImpactEffect;
    stream >> tmp >> m_healthImpactEffect;
    stream >> tmp >> m_damageImpactEffect;
    stream >> tmp >> m_currState;
    stream >> tmp >> m_health;
    stream >> tmp >> m_usedCapacity;
    stream >> tmp >> m_initializeGround;
    stream >> tmp >> m_constructing;
}

void Building::load(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;
    ifstream stream;
    stream.open(configFile.c_str());

    Tile::load(stream);
    load(stream);

    stream.close();

    m_objectTexture = LoadTexture(m_img, renderer);

    m_groundTexture = LoadTexture(m_groundImg, renderer);

    m_ownerTextureP1 = LoadTexture(m_ownerImgP1, renderer);
    SDL_SetTextureColorMod(m_ownerTextureP1, world.m_CP1.r, world.m_CP1.g, world.m_CP1.b);
    m_ownerTextureP2 = LoadTexture(m_ownerImgP2, renderer);
    SDL_SetTextureColorMod(m_ownerTextureP2, world.m_CP2.r, world.m_CP2.g, world.m_CP2.b);

    m_buildTexture = LoadTexture(m_buildImg, renderer);

    m_damageTexture = LoadTexture(m_damageImg, renderer);

    if (m_rotationAngle != 0 && m_rotationAngle != 180 && m_rotationAngle != 360)
    {
        m_healthPerState = m_maxHealth / (m_maxState + 1);
    }
    else
    {
        m_healthPerState = m_maxHealth / m_maxState;
    }

    m_drawState.h = m_height;
    m_drawState.w = m_width;
    m_drawState.x = 0;
}

void Building::setTextureDetails(SDL_Renderer* renderer)
{
    if (m_rotationAngle == 0 || m_rotationAngle == 180 || m_rotationAngle == 360)
    {
        if (m_owner == PLAYER1)
        {
            SDL_RenderCopyEx(renderer, m_ownerTextureP1, &m_drawState, &m_objectRect, m_rotationAngle, NULL, SDL_FLIP_NONE);
        }
        else if (m_owner == PLAYER2)
        {
            SDL_RenderCopyEx(renderer, m_ownerTextureP2, &m_drawState, &m_objectRect, m_rotationAngle, NULL, SDL_FLIP_NONE);
        }
        if (m_constructing)
        {
            SDL_RenderCopyEx(renderer, m_buildTexture, &m_drawState, &m_objectRect, m_rotationAngle, NULL, SDL_FLIP_NONE);
        }
        else if (m_health < m_maxHealth)
        {
            SDL_RenderCopyEx(renderer, m_damageTexture, &m_drawState, &m_objectRect, m_rotationAngle, NULL, SDL_FLIP_NONE);
        }
    }
}

void Building::limitHealth()
{
    if (m_health >= m_maxHealth)
    {
        m_health = m_maxHealth;
        m_healthPerState = m_maxHealth / (m_states - m_maxState);
        m_constructing = false;
    }
}

void Building::initializeGround()
{
    if (m_initializeGround)
    {
        m_groundRect = m_objectRect;
        m_groundRect.y -= m_adjustment;
        m_groundRect.h = 60;
        m_initializeGround = false;
    }
}

void Building::getStateWithAngle()
{
    if (m_constructing)
    {
        m_drawState.h = m_height / m_imgRows;
        m_drawState.w = m_width / m_imgCols;
        m_currState = m_health / m_healthPerState;
    }
    else
    {
        m_currState = (m_maxHealth - m_health) / m_healthPerState + m_maxState;
    }
    m_drawState.x = ((m_width / m_imgCols) * m_currState) % m_width;
    m_drawState.y = (m_height / m_imgRows) * (m_currState / m_imgCols);
    if (m_currState == m_states)
    {
        m_drawState.h = 0;
        m_drawState.w = 0;
    }
}

void Building::getStateRegular()
{
    if (m_constructing)
    {
        m_currState = m_health / m_healthPerState;
        m_drawState.y = (m_maxState - m_currState) * (m_height / (m_maxState + 1));
        m_objectRect.h = (world.m_hexagonHeight / (m_maxState + 1)) * (m_currState + 1);
        m_objectRect.y -= m_adjustment;
        if (m_rotationAngle == 0 || m_rotationAngle == 360)
        {
            m_adjustment = world.m_hexagonHeight - (world.m_hexagonHeight / (m_maxState + 1)) * (m_currState + 1);
        }
        m_objectRect.y += m_adjustment;
    }
    else
    {
        m_currState = (m_maxHealth - m_health) / m_healthPerState + m_maxState;
        m_drawState.y = (m_currState - m_maxState) * (m_height / (m_states - m_maxState + 1));
        m_objectRect.h = (world.m_hexagonHeight / (m_states - m_maxState)) * (m_states - m_currState);
        m_objectRect.y -= m_adjustment;
        if (m_rotationAngle == 0 || m_rotationAngle == 360)
        {
            m_adjustment = - world.m_hexagonHeight + (world.m_hexagonHeight / (m_maxState + 1)) * (m_currState + 1);
        }
        m_objectRect.y += m_adjustment;
    }
}

void Building::getState()
{
    limitHealth();
    if (m_constructing &&
            milliseconds(duration_cast<milliseconds>(steady_clock::now() - m_currTimePoint).count()) >= milliseconds(m_duration))
    {
        m_health += m_healthPerState;
        m_currTimePoint = steady_clock::now();
    }
    if (m_rotationAngle != 0 && m_rotationAngle != 180 && m_rotationAngle != 360)
    {
        getStateWithAngle();
    }
    else
    {
        getStateRegular();
    }
    initializeGround();
}

void Building::impactEntities()
{
    for (vector<Squad*>::iterator it = world.m_squads.begin(); it != world.m_squads.end(); it++)
    {
        /*
        (*it)->
        update all:
        + m_moralImpact;
        + m_healthImpact;
        + m_damageImpact;
        */
    }
    //TO-DO:
    //The effect
    //Way to know if squads are in or not
}

void Building::update()
{
    getState();
    impactEntities();
}

void Building::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, m_groundTexture, NULL, &m_groundRect);
    SDL_RenderCopyEx(renderer, m_objectTexture, &m_drawState, &m_objectRect, m_rotationAngle, NULL, SDL_FLIP_NONE);
    setTextureDetails(renderer);
}
