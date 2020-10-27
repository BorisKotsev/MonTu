#ifndef ENEMYAI_H
#define ENEMYAI_H

#include <vector>

#include <Tile.h>
#include <Squad.h>

class EnemyAI
{
    public:
        EnemyAI();
        virtual ~EnemyAI();

        vector<vector<Tile*> > m_battlefield;
        vector<Squad*> m_aiSquads;
        vector<Squad*> m_playerSquads;

        void takeBattlefield();
        void makeTurn();
        void returnBattlefield();
        void clearAI();

    protected:

    private:
        void chooseBestActionForUnit(Squad* squad);

};

#endif // ENEMYAI_H
