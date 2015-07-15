/*
 * do dfs first. If it can not be finish, the state is add in a "finish list". The finish list save all states,
 * where a endstate can not be reached with the steps
 */

#ifndef DFS_ASTAR_H
#define DFS_ASTAR_H
#include "depthfirst.h"
#include <vector>
#include <set>

class DFS_AStar: public DepthFirst
{
public:
    DFS_AStar();
    uint getBlockListSize();
protected:
    virtual bool doDepth(Heuristics::AStarHeuristics& heur, int maxDeep);

    std::vector<std::set< world> > visitedList;
};

#endif // DFS_ASTAR_H
