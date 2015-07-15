#ifndef GETWORMTOFIELD_H
#define GETWORMTOFIELD_H
#include "astarheuristics.h"
#include "getonefieldfree.h"
#include "World/pos.h"

namespace Heuristics{
    class GetWormToField: public GetOneFieldFree
    {
    public:
        GetWormToField(world& w, const WormPart& dest, const Worm& wu);

    // Operator
        uint operator ()(world* w);
        uint getCostToField(const Worm& wormId);
    private:
    // functions
        void createDistanceGraphForOne();

    // Vars

        Worm WormDest;
    };
}
#endif // GETWORMTOFIELD_H
