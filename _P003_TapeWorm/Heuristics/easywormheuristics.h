#ifndef EASYWORMHEURISTICS_H
#define EASYWORMHEURISTICS_H
#include "Heuristics/astarheuristics.h"
#include "World/pos.h"

typedef std::deque<WormPart> HomePostionContainer;
typedef std::deque<WormPart> HomePathContainer;

namespace Heuristics{
    class EasyWormHeuristics : public AStarHeuristics
    {
    public:
        EasyWormHeuristics(world& _w, uint _wormNumber);

        uint operator() (world* _w);
        uint getBlockedHomeFields();
        uint getBlockedPathFields();
        uint getBlockedFields();
        uint getMaxDist();
        HomePathContainer& getHomePath(bool head);
         void createInverseDistances(world& _w, Worm _wormNumber);
    private:
        void findHomeFields();
        void findHomePath(bool head);
// (x, y, worm, part)
        std::vector< std::vector< std::vector < std::vector< uint > > > > costgetBlockedFields;
        std::vector< std::vector < uint > > distancesInverse;
// create cost

        void createCost(world& _w, Worm _worm);
        WormPart getMinNeighbor(const WormPart& p, Color color);
// pos of worm in container
        uint wormNumber;
        HomePostionContainer homeFields;
        HomePathContainer homePath;
    };
}

#endif // EASYWORMHEURISTICS_H
