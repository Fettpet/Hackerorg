#ifndef RECOLOR_H
#define RECOLOR_H
#include "World/world.h"
#include <vector>
#include "World/pos.h"

class Recolor
{
public:
    static void getValidWormsNewColors(world& w);

private:
    static void seperateWorms(world& w, std::vector< std::vector< uint > >& result);
    static bool foundSeperatePartInMap(const world& w, const uint wurmId, std::vector<WormPart>& positions);
    static uint getLength(const world& w, std::vector< std::vector< bool> >& visitedMap, uint x, uint y, uint color, std::vector<WormPart>& p);
};

#endif // RECOLOR_H
