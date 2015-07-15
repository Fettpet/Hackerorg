#ifndef DEPTHFIRST_H
#define DEPTHFIRST_H
#include "World/worlddeep.h"
#include "Heuristics/astarheuristics.h"
#include <string>
#include <boost/timer.hpp>
#define DEEPENING 5

class DepthFirst
{
public:
    DepthFirst();
    std::string calc( world& w, Heuristics::AStarHeuristics& heur);

    long long int getCreatedStates();
    uint getMaxDepth();
    uint getStartDepth();
    long unsigned int getNotExpand();
    uint getSolutionLength();
protected:
    virtual bool doDepth(Heuristics::AStarHeuristics& heur, int maxDeep);
    uint maxDepth;
    uint startDepth;
    long unsigned int notExpand;
    long long int visitedStates;
    WorldDeep wo;
    boost::timer timer;
};

#endif // DEPTHFIRST_H
