#ifndef ASTARHEURISTICS_H
#define ASTARHEURISTICS_H
#include "World/world.h"
#include "World/worm.h"
#include <vector>
#include <boost/graph/astar_search.hpp>

typedef unsigned int uint;

namespace Heuristics{
    class AStarHeuristics//: boost::astar_heuristic<Graph, int>
    {
    public:
        AStarHeuristics(world& w);
        uint getWormPartsAreNotAtHome();
        uint getWormPartsAreNotAtHome(const Worm& wurm);

        uint getTheCountOfTheFirstOutsider();
        uint getTheCountOfTheFirstOutsider(const Worm& wurm);

        uint getDistanceFromHeadToHome();
        uint getDistanceFromHeadToHome(const WormPart& part, const uint color);
        uint getDistanceFromHeadToHome(const Worm& wurm);

        uint getDistancePlusCountOfFirstOutsider();
        uint getDistancePlusCountOfFirstOutsider(uint wormNumber);
        uint getDistancePlusCountOfFirstOutsider(const Worm& wurm);

        void printDistanceGraph();
        void createDistanceGraph();

        virtual uint operator()(world* _w){
            w = _w;
            return getDistancePlusCountOfFirstOutsider();
        }
    protected:
    // Test exeption
        bool canSetToUnused(const Worm& wurm);
        world* w;
    // distance Map
    // Innnerbuild: (x, y, Color)
        static std::vector< std::vector< std::vector < int > > > distances;
    };
}

#endif // ASTARHEURISTICS_H
