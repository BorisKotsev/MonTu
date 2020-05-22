#include "Tile.h"

Tile::Tile()
{
    m_owner = NONE;
}

Tile::~Tile()
{
    //dtor
}

Tile::Tile(const Tile& model)
{
    m_walkDifficulty = model.m_walkDifficulty;
    m_objectTexture = model.m_objectTexture;
}

void Tile::load(string configFile, SDL_Renderer* renderer)
{
    configFile = "config\\" + configFile;
    ifstream stream;
    string tmp, buff;
    stream.open(configFile.c_str());

    stream >> tmp >> m_walkDifficulty;
    stream >> tmp >> buff;

    stream.close();

    m_objectTexture = LoadTexture(buff, renderer);
}

void Tile::draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, m_objectTexture, NULL, &m_objectRect);
}
