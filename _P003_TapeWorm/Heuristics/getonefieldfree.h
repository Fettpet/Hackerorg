#ifndef GETONEFIELDFREE_H
#define GETONEFIELDFREE_H
#include "astarheuristics.h"

namespace Heuristics{
    class GetOneFieldFree : public AStarHeuristics
    {
    public:
        GetOneFieldFree(world w, WormPart free);

    // Operator
        uint operator ()(world* w);

    // Die Heurisik
        uint getMovesToFreeAField(const WormPart&, const Worm& WormId);
    protected:
    // Functions
        void calcWormThatBlocksField();

    // Vars
        world* w;
        WormPart toFree;
        Worm WormId;

    };
}

#endif // GETONEFIELDFREE_H
