#ifndef BUILDING_H
#define BUILDING_H

#include <chrono>

#include <Tile.h>

using namespace chrono;

class Building : public Tile
{
    public:
        Building();
        Building(const Building& model);
        virtual ~Building();

        short int m_height;
        short int m_width;
        short int m_imgRows;
        short int m_imgCols;
        short int m_adjustment;
        double m_rotationAngle;
        SDL_Rect m_drawState;
        SDL_Rect m_groundRect;

        string m_groundImg;
        string m_ownerImgP1;
        string m_ownerImgP2;
        string m_buildImg;
        string m_damageImg;
        SDL_Texture* m_groundTexture;
        SDL_Texture* m_ownerTextureP1;
        SDL_Texture* m_ownerTextureP2;
        SDL_Texture* m_buildTexture;
        SDL_Texture* m_damageTexture;

        short int m_duration;
        steady_clock::time_point m_currTimePoint;

        short int m_maxHealth;
        short int m_healthPerState;
        short int m_health;
        short int m_capacity;
        short int m_usedCapacity;
        short int m_states;
        short int m_maxState;
        short int m_currState;
        short int m_moralImpact;
        short int m_healthImpact;
        short int m_damageImpact;
        short int m_effectPeriod;
        short int m_moralImpactEffect;
        short int m_healthImpactEffect;
        short int m_damageImpactEffect;

        bool m_initializeGround;
        bool m_constructing;

        void load(ifstream& stream);
        virtual void load(string configFile, SDL_Renderer* renderer);
        virtual void update();
        virtual void draw(SDL_Renderer* renderer);

    protected:

    private:
        void limitHealth();
        void initializeGround();
        void getStateRegular();
        void getStateWithAngle();
        void getState();
        void setTextureDetails(SDL_Renderer* renderer);
        void impactEntities();
};

#endif // BUILDING_H
