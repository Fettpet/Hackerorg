#ifndef WORLDDEEP_H
#define WORLDDEEP_H
#include "World/world.h"
#include <boost/dynamic_bitset.hpp>
#include <deque>
#include "World/pos.h"
#include <set>
/**************************************************************************************************
 *                                  Typedef and Helpclass
 * ***********************************************************************************************/
class MoveInDeep{
public:
    MoveInDeep(){}
    MoveInDeep(const MoveInDeep& other):
        movedHead(other.movedHead),
        old(other.old),
        start(other.start),
        destiny(other.destiny),
        wormNumber(other.wormNumber),
        costToDest(other.costToDest)
        {}
    MoveInDeep(const WormPart& _start, const WormPart& _dest, const uint& _wormNumber, const uint& _costToDest,
               const bool& _moveHead, const WormPart& o):
        movedHead(_moveHead),
        old(o),
        start(_start),
        destiny(_dest),
        wormNumber(_wormNumber),
        costToDest(_costToDest){}
    MoveInDeep(const WormPart& _start, const WormPart& _dest, const uint& _wormNumber,
               const bool& _moveHead, const WormPart& o):
        movedHead(_moveHead),
        old(o),
        start(_start),
        destiny(_dest),
        wormNumber(_wormNumber) {}

    bool operator<(const MoveInDeep& other) const{
        if(costToDest < other.costToDest) return true;
        if(other.costToDest < costToDest) return false;
        if(movedHead < other.movedHead) return true;
        if(other.movedHead < movedHead) return false;
        if(destiny < other.destiny) return true;
        if(other.destiny < destiny) return false;
        if(start < other.start) return true;
        return false;
    }
    bool movedHead;
    WormPart old;
    WormPart start, destiny;
    uint wormNumber;
    uint costToDest;
    friend std::ostream& operator<<(std::ostream& out, const MoveInDeep&);
};

typedef std::set<MoveInDeep> MoveInDeepContainer;
namespace Heuristics{
class AStarHeuristics;
}
/**************************************************************************************************
 *                                          mainclass
 * ***********************************************************************************************/
class WorldDeep: public world
{
public:
    WorldDeep();
    ~WorldDeep(){}
    WorldDeep(const WorldDeep& old);
    WorldDeep(const world& old): world(old){}
    void getAllMoves(MoveInDeepContainer& con, Heuristics::AStarHeuristics& heur, uint deep);

    virtual std::string getPath() const;

    WorldDeep& operator=(const WorldDeep& t);

    WorldDeep& operator=(const world& old){
        Hash = old.getHash();
        worms = old.getWorms();
        lastMoveStart = old.getLastMoveStart();
        lastMoveDestiny = old.getLastMoveDestiny();
        CurrentSteps = old.getCurrentSteps();
        lastMove = old.getLastMove();
        return *this;
    }

    void move(const MoveInDeep& m);
    void oneMoveBack();
    void clearMoves();

    bool isBlocked(const WormPart& part, uint worm)const;
    static std::vector<std::set< world> > visitedList;
    void createBlockedMap();
protected:
    static std::vector< boost::dynamic_bitset<> > blocked;
    std::deque<MoveInDeep> MoveContainer;
};

#endif // WORLDDEEP_H
