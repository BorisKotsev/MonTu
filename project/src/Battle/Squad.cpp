#include "Squad.h"

Squad::Squad()
{
    m_goalTile = nullptr;
}

Squad::~Squad()
{
    //dtor
}

Squad::Squad(const Squad& model, coordinates* cameraOffset, Tile* tile, OWNER owner)
{
    m_startHealth = model.m_startHealth;
    m_startAttackDamage = model.m_startAttackDamage;
    m_startSpeed = model.m_startSpeed;
    m_startAttackRange = model.m_startAttackRange;
    m_startFaith = model.m_startFaith;

    m_cameraOffset = cameraOffset;

    m_tileTaken = tile;

    m_mapCoor = tile->m_mapCoordinates;

    m_objectRect.x = tile->m_drawCoordinates.x;
    m_objectRect.y = tile->m_drawCoordinates.y;
    m_objectRect.w = model.m_objectRect.w;
    m_objectRect.h = model.m_objectRect.h;

    m_health = m_startHealth;
    m_attackDamage = m_startAttackDamage;
    m_speed = m_startSpeed;
    m_attackRange = m_startAttackRange;
    m_faith = m_startFaith;

    if(owner == PLAYER1)
    {
        m_objectTexture = model.m_playerTexture;
    }
    else
    {
        m_objectTexture = model.m_enemyTexture;
        m_maxFramesPerStep /= 10;
    }

    m_renderer = model.m_renderer;

    m_owner = owner;

    m_hm = model.m_hm;

    m_moved = false;
    m_attacked = false;
}


void Squad::load(string configFile, SDL_Renderer* renderer, HealthManager* hm)
{
    configFile = "config\\" + configFile;
    ifstream stream;
    string tmp, imgPlayer, imgEnemy;
    stream.open(configFile.c_str());

    stream >> tmp >> imgPlayer >> imgEnemy;
    stream >> tmp >> m_objectRect.w >> m_objectRect.h;

    stream >> tmp >> m_startHealth;
    stream >> tmp >> m_startAttackDamage;
    stream >> tmp >> m_startSpeed;
    stream >> tmp >> m_startAttackRange;
    stream >> tmp >> m_startFaith;

    stream.close();

    m_playerTexture = LoadTexture(imgPlayer, renderer);
    m_enemyTexture = LoadTexture(imgEnemy, renderer);

    m_renderer = renderer;

    m_hm = hm;
}

void Squad::update()
{
//    if (!(m_path.empty()) && m_framesPerStep >= m_maxFramesPerStep)
//    {
//         cout << "INFO: The size of the walk queue is " << m_path.size() << endl;
//        m_tileTaken = m_path.top();
//
//        m_path.pop();
//
//        m_mapCoor = m_tileTaken->m_mapCoordinates;
//
//        m_objectRect.x = m_tileTaken->m_drawCoordinates.x;
//        m_objectRect.y = m_tileTaken->m_drawCoordinates.y;
//
//        m_framesPerStep = 0;
//    }
//    else if (!(m_path.empty()))
//    {
//        m_framesPerStep ++;
//    }
    //idleAnimation();
    walk();
}

void Squad::draw()
{
    m_presentRect.x = m_objectRect.x + m_cameraOffset->x;
    m_presentRect.y = m_objectRect.y + m_cameraOffset->y;
    m_presentRect.w = m_objectRect.w;
    m_presentRect.h = m_objectRect.h;
    SDL_RenderCopy(m_renderer, m_objectTexture, NULL, &m_presentRect);
    //m_hm->drawHealthbar(m_renderer, m_presentRect, m_health, m_startHealth);
}

void Squad::idleAnimation()
{
    if(!m_moved || !m_attacked)
    {
        if(m_moveUp)
        {
            if(m_idleAnimationCounter >= m_idleAnimationRange)
            {
                m_moveUp = false;
            }
            else
            {
                m_objectRect.y += 1;
                m_idleAnimationCounter += 1;
            }
        }
        else
        {
            if(m_idleAnimationCounter <= -m_idleAnimationRange)
            {
                m_moveUp = true;
            }
            else
            {
                m_objectRect.y -= 1;
                m_idleAnimationCounter -= 1;
            }
        }
    }
}

void Squad::attack(Squad* defender)
{
    defender->m_health -= m_attackDamage;
    m_attacked = true;
    m_moved = true;
}

void Squad::syncCoor()
{
}

void Squad::walk()
{
    if(m_goalTile == nullptr)
    {
        if(!(m_path.empty()))
        {
            // Loading the goalTile in the begging of the walk
            m_goalTile = m_path.top();
            m_tileTaken = m_path.top();
            m_path.pop();

            coordinates direction;
            direction.x = m_goalTile->m_objectRect.x - m_objectRect.x;
            direction.y = m_goalTile->m_objectRect.y - m_objectRect.y;

            m_directionAngle = returnAngleByCoordinates(direction);

            m_walkCoor.x = m_objectRect.x;
            m_walkCoor.y = m_objectRect.y;
        }
    }else{
        m_walkCoor.x += sin(m_directionAngle * PI / 180);
        m_walkCoor.y -= cos(m_directionAngle * PI / 180);

        m_objectRect.x = m_walkCoor.x;
        m_objectRect.y = m_walkCoor.y;

        if(distance(m_objectRect, m_goalTile->m_objectRect) < 2)
        {
            // We are close enough to the target
            m_objectRect.x = m_goalTile->m_objectRect.x;
            m_objectRect.y = m_goalTile->m_objectRect.y;

            // Load the next tile, if there is one
            if(!(m_path.empty()))
            {
                m_goalTile = m_path.top();
                m_tileTaken = m_path.top();
                m_path.pop();

                coordinates direction;
                direction.x = m_goalTile->m_objectRect.x - m_objectRect.x;
                direction.y = m_goalTile->m_objectRect.y - m_objectRect.y;

                m_directionAngle = returnAngleByCoordinates(direction);

                m_walkCoor.x = m_objectRect.x;
                m_walkCoor.y = m_objectRect.y;

                m_mapCoor = m_tileTaken->m_mapCoordinates;
            }else{
                m_goalTile = nullptr;
            }
        }
    }
}
