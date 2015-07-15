/**
    This function count how many World parts are in the World
*/
#ifndef WORLDPARTS_H
#define WORLDPARTS_H
#include "World/world.h"

namespace Tools{
    uint getWorldPartSize(const world& w, const WormContainer& worms, const WormPart& p);
    uint getWorldPartSize(const world &w, const Worm &worms, const WormPart& p);
    uint getWorldPartSize(const world &w, const uint &worms, const WormPart& p);

    uint countWorldParts(const world& w, const WormContainer& blockedWorms);
    uint countWorldParts(const world& w, const Worm& blockedWorms);
    uint countWorldParts(const world &w, const uint &blockedWorm);
}

#endif // WORLDPARTS_H
