#include "castleUI.h"
#include "World.h"
#include "squad_manager.h"

extern World world;

castleUI::castleUI()
{
    m_tab = 0;
}

castleUI::~castleUI()
{
    //dtor
}

void castleUI::init(string configFile, string cityName, SDL_Renderer* renderer)
{
    m_cityName = cityName;

    m_draggedSquad = -1;

    m_renderer = renderer;

    configFile = "config\\" + configFile;
    fstream stream;
    string tmp;
    string mainWindow[3];
    string slotTexture;

    USHORT margin;
    USHORT slotSize;
    USHORT bonusSlotSize;
    coordinates slotsStartCoor;

    stream.open(configFile);

    /// Main windows init
    stream >> tmp >> mainWindow[0] >> mainWindow[1] >> mainWindow[2];
    stream >> tmp >> m_mainWindow[0].objRect.x >> m_mainWindow[0].objRect.y >> m_mainWindow[0].objRect.w >> m_mainWindow[0].objRect.h;
    stream >> tmp >> m_tabButtons[0].x >> m_tabButtons[0].y >> m_tabButtons[0].w >> m_tabButtons[0].h;
    stream >> tmp >> m_tabButtons[1].x >> m_tabButtons[1].y >> m_tabButtons[1].w >> m_tabButtons[1].h;
    stream >> tmp >> m_tabButtons[2].x >> m_tabButtons[2].y >> m_tabButtons[2].w >> m_tabButtons[2].h;

    /// City Army
    stream >> tmp >> slotsStartCoor.x >> slotsStartCoor.y;
    stream >> tmp >> margin;    stream >> tmp >> slotSize;
    stream >> tmp >> bonusSlotSize;
    stream >> tmp >> slotTexture;

    /// Create Squad
    stream >> tmp >> m_startOfCreateSquad.x >> m_startOfCreateSquad.y;
    stream >> tmp >> m_createSquadMargin;
    stream >> tmp >> m_arrowTextureLocation;

    stream.close();

    m_mainWindow[0].objTexture = LoadTexture(mainWindow[0], m_renderer);
    m_mainWindow[1].objTexture = LoadTexture(mainWindow[1], m_renderer);
    m_mainWindow[2].objTexture = LoadTexture(mainWindow[2], m_renderer);

    /// Save the data needed for the hover animation of every slot, when hovered
    m_hoveredSlot.startRect.w = slotSize;
    m_hoveredSlot.startRect.h = slotSize;
    m_hoveredSlot.objectRect.w = slotSize;
    m_hoveredSlot.objectRect.h = slotSize;
    m_hoveredSlot.bonusW = 3;
    m_hoveredSlot.bonusH = 3;
    m_hoveredSlot.maxWidth = bonusSlotSize;
    m_hoveredSlot.maxHeigth = bonusSlotSize;
    m_hoveredSlot.objTexture = LoadTexture(slotTexture, m_renderer);

    for (USHORT r = 0; r < 3; r ++)
    {
        for (USHORT c = 0; c < 8; c ++)
        {
            m_colliders[r][c].y = slotsStartCoor.y + (slotSize + margin) * r;
            m_colliders[r][c].x = slotsStartCoor.x + (slotSize + margin) * c;
            m_colliders[r][c].w = slotSize;
            m_colliders[r][c].h = slotSize;
        }
    }
}

void castleUI::hover()
{
    /// Check all slots with squads inside
    for (USHORT i = 0; i < m_squads.size(); i ++)
    {
        if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_squads[i]->objectRect))
        {
            if(m_squads[i]->objectRect.w < m_squads[i]->startRect.w + m_squads[i]->maxWidth)
            {
                m_squads[i]->currentBonusW += m_squads[i]->bonusW;
                m_squads[i]->currentBonusH += m_squads[i]->bonusH;

                m_squads[i]->objectRect.w = m_squads[i]->startRect.w + m_squads[i]->currentBonusW;
                m_squads[i]->objectRect.h = m_squads[i]->startRect.h + m_squads[i]->currentBonusH;
                m_squads[i]->objectRect.x = m_squads[i]->startRect.x - m_squads[i]->currentBonusW / 2;
                m_squads[i]->objectRect.y = m_squads[i]->startRect.y - m_squads[i]->currentBonusH / 2;
            }
            break;
        }
        else
        {
            m_squads[i]->objectRect = m_squads[i]->startRect;

            m_squads[i]->currentBonusW = 0;
            m_squads[i]->currentBonusH = 0;
        }
    }

    for(USHORT r = 0; r < 3; r ++)
    {
        for (USHORT c = 0; c < 8; c ++)
        {
            if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_colliders[r][c]))
            {
                if(m_hoveredSlot.objectRect.w < m_hoveredSlot.startRect.w + m_hoveredSlot.maxWidth)
                {
                    m_lastHoveredSlot.x = r;
                    m_lastHoveredSlot.y = c;

                    m_hoveredSlot.currentBonusW += m_hoveredSlot.bonusW;
                    m_hoveredSlot.currentBonusH += m_hoveredSlot.bonusH;

                    m_hoveredSlot.objectRect.w = m_hoveredSlot.startRect.w + m_hoveredSlot.currentBonusW;
                    m_hoveredSlot.objectRect.h = m_hoveredSlot.startRect.h + m_hoveredSlot.currentBonusH;
                    m_hoveredSlot.objectRect.x = m_colliders[r][c].x - m_hoveredSlot.currentBonusW / 2;
                    m_hoveredSlot.objectRect.y = m_colliders[r][c].y - m_hoveredSlot.currentBonusH / 2;
                }
                /// if your mouse is over a slot, stop the func
                return;
            }
        }
    }
    /// If we are here than the mouse is not on a tab, then we can make the hoveredSlot with currentBonus = 0
    m_hoveredSlot.objectRect = m_hoveredSlot.startRect;

    m_hoveredSlot.objectRect.w = 0;
    m_hoveredSlot.objectRect.h = 0;
    m_hoveredSlot.currentBonusW = 0;
    m_hoveredSlot.currentBonusH = 0;
}

void castleUI::moveSquad()
{
    if(world.m_mouseIsReleased && m_draggedSquad != -1)
    {
        m_squads[m_draggedSquad]->startRect = m_colliders[m_lastHoveredSlot.x][m_lastHoveredSlot.y];
        m_squads[m_draggedSquad]->objectRect = m_colliders[m_lastHoveredSlot.x][m_lastHoveredSlot.y];
        m_squads[m_draggedSquad]->data.coord.x = m_lastHoveredSlot.x;
        m_squads[m_draggedSquad]->data.coord.y = m_lastHoveredSlot.y;
    }
}

void castleUI::update()
{
    if(world.m_mouseIsPressed)
    {
        for(usi i = 0; i < 3; i++)
        {
            if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_tabButtons[i]))
            {
                m_tab = i;
                return;
            }
        }
    }

    switch(m_tab)
    {
        case 0:
            updateCitySquad();
            break;
        case 1:
            updateCreateSquad();
            break;
    }
}

void castleUI::draw()
{
    SDL_RenderCopy(m_renderer, m_mainWindow[m_tab].objTexture, NULL, &(m_mainWindow[0].objRect));

    switch(m_tab)
    {
        case 0:
            drawCitySquad();
            break;
        case 1:
            drawCreateSquad();
            break;
    }

    SDL_RenderPresent(m_renderer);
}

void castleUI::updateCitySquad()
{
    hover();

    moveSquad();

    saveData("squad1.txt");

    if(world.m_mouseIsPressed)
    {
        for (USHORT i = 0; i < m_squads.size(); i ++)
        {
            if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_squads[i]->objectRect))
            {
                m_draggedSlot.objTexture = m_squads[i]->objTexture;
                m_draggedSquad = i;
                break;
            }
        }
    }

    if(!world.m_drag)
    {
        m_draggedSlot.objRect.w = 0;
        m_draggedSlot.objRect.h = 0;
        m_draggedSlot.objTexture = nullptr;
        m_draggedSquad = -1;
    }else{
        m_draggedSlot.objRect.w = m_squads[0]->objectRect.w;
        m_draggedSlot.objRect.h = m_squads[0]->objectRect.h;
    }

    m_draggedSlot.objRect.x = world.m_mouse.x - m_draggedSlot.objRect.w / 2;
    m_draggedSlot.objRect.y = world.m_mouse.y - m_draggedSlot.objRect.h / 2;
}

void castleUI::drawCitySquad()
{
    SDL_RenderCopy(m_renderer, m_hoveredSlot.objTexture, NULL, &(m_hoveredSlot.objectRect));
    for(unsigned short i = 0; i < m_squads.size(); i ++)
    {
        if(!world.m_drag)
        {
            SDL_RenderCopy(m_renderer, m_squads[i]->objTexture, NULL, &(m_squads[i]->objectRect));
        }else{
            if(i != m_draggedSquad)
            {
                SDL_RenderCopy(m_renderer, m_squads[i]->objTexture, NULL, &(m_squads[i]->objectRect));
            }
        }
    }
    SDL_RenderCopy(m_renderer, m_draggedSlot.objTexture, NULL, &(m_draggedSlot.objRect));
}

void castleUI::loadData(string configFile)
{
    fstream stream;

    configFile = "data//" + configFile;

    stream.open(configFile);

    USHORT type;

    string tmp;

    squadSlot* squadBuff;
    soldier* dataBuff;

    while(stream >> type)
    {
        dataBuff = new soldier;
        squadBuff = new squadSlot;
        stream >> tmp >> squadBuff->data.health >> tmp >> squadBuff->data.coord.y >> squadBuff->data.coord.x;
        dataBuff->type = type;
        dataBuff->health = squadBuff->data.health;
        squadBuff->data.type = (SQUAD)type;
        squadBuff->bonusW = m_hoveredSlot.bonusW;
        squadBuff->bonusH = m_hoveredSlot.bonusH;
        squadBuff->maxWidth = m_hoveredSlot.maxWidth;
        squadBuff->maxHeigth = m_hoveredSlot.maxHeigth;
        D(squadBuff->data.coord.x);
        D(squadBuff->data.coord.y);
        squadBuff->startRect = m_colliders[squadBuff->data.coord.x][squadBuff->data.coord.y];
        squadBuff->objectRect = m_colliders[squadBuff->data.coord.x][squadBuff->data.coord.y];
        squadBuff->objTexture = loadSquadTexture(squadBuff->data.type);
        m_squads.push_back(squadBuff);
    }

    stream.close();
}

void castleUI::saveData(string configFile)
{
    fstream stream;

    configFile = "data//" + configFile;

    stream.open(configFile);

    for(vector<squadSlot*> :: iterator it = m_squads.begin(); it != m_squads.end(); it ++)
    {
        stream << (int)((*it) -> data.type) << " HEALTH: " << (*it) -> data.health << " COORDINATES: " << (*it) -> data.coord.y << " " << (*it) -> data.coord.x << endl;
    }

    stream.close();
}

SDL_Texture* castleUI::loadSquadTexture(SQUAD type)
{
    SDL_Texture* returnData = nullptr;

    D(type);

    switch(type)
    {
        case WARRIOR:
            returnData = LoadTexture("warrior.bmp", m_renderer);
            break;
        case ARCHER:
            returnData = LoadTexture("archer.bmp", m_renderer);
            break;
        case SPEARMEN:
            returnData = LoadTexture("spearmen.bmp", m_renderer);
            break;
        case KNIGHTS:
            returnData = LoadTexture("knights.bmp", m_renderer);
            break;
        case CROSSBOWMEN:
            returnData = LoadTexture("crossbowmen.bmp", m_renderer);
            break;
        default:
            break;
    }

    return  returnData;
}

void castleUI::createSquad()
{
    int squadIndex = world.m_squadManager.addSquad(m_cityName);

    for (unsigned short i = 0; i < m_exportSquad.size(); i ++)
    {
        world.m_squadManager.addSoldier(squadIndex, m_exportSquad[i].numberOfSoldiers, m_exportSquad[i].type, i % 8, i / 8);
    }
}

void castleUI::updateCreateSquad()
{
    if (world.m_mouseIsPressed)
    {
        /// Update the main ones
        for (unsigned short i = 0; i < m_createSquadEl.size(); i ++)
        {
            if(checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_createSquadEl[i].m_upBtnCoor, m_arrowSize))
            {
                cout << "The up btn for the " << i << " element is pressed \n";
            }else if (checkForMouseCollision(world.m_mouse.x, world.m_mouse.y, m_createSquadEl[i].m_downBtnCoor, m_arrowSize))
            {
                cout << "The down btn for the " << i << " element is pressed \n";
            }
        }

        /// Update the ones in the side menu
    }
}

void castleUI::drawCreateSquad()
{
    /// TODO
    for (unsigned short i = 0; i < m_createSquadEl.size(); i ++)
    {
        SDL_RenderCopy(m_renderer, m_createSquadEl[i].m_pic.objTexture, NULL, &(m_createSquadEl[i].m_pic.objRect));
        SDL_RenderCopy(m_renderer, m_arrowTexture, NULL, &(m_createSquadEl[i].m_upBtnRect));
        SDL_RenderCopyEx(m_renderer, m_arrowTexture, NULL, &(m_createSquadEl[i].m_downBtnRect), 0, 0, SDL_FLIP_HORIZONTAL);

        coordinates buff;
        buff.x = m_createSquadEl[i].m_pic.objRect.x + 10;
        buff.y = m_createSquadEl[i].m_pic.objRect.y;

        write(m_createSquadEl[i].m_typeName, (buff), m_renderer, 20);
    }
}
