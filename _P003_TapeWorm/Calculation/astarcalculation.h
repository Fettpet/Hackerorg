#ifndef ASTARCALCULATION_H
#define ASTARCALCULATION_H
#include <vector>
#include "Heuristics//astarheuristics.h"
#include "World/world.h"
#include <list>
#include <set>
#include "Exceptions/nowayfound.h"
/**************************************************************************************************
 *                                          typedefs
 * ***********************************************************************************************/

class worldRep {
public:
    worldRep(): w(NULL), worldNumber(0){}
    worldRep(world* _w, uint _worldNumber): w(_w), worldNumber(_worldNumber){}
    worldRep(const worldRep& other):
        w(other.w), worldNumber(other.worldNumber)
    {}

    world* w;
    uint worldNumber;
    worldRep& operator=(const worldRep& other){
        this->w = other.w;
        this->worldNumber = other.worldNumber;
        return *this;
    }
    bool operator==(const worldRep& other) const{
        return ((*w) == (*(other.w)));
    }
    bool operator<(const worldRep& other) const{
        return ((*w) < (*(other.w)));
    }
};

typedef std::deque<uint> ParentList;
typedef std::deque<worldRep> worldRepContainer;
typedef std::set<worldRep> InnerListContainer;
typedef std::vector<InnerListContainer> OuterListContainer;

/**************************************************************************************************
 *                              Class
 * ***********************************************************************************************/

class AStarCalculation
{
public:
    AStarCalculation();
    uint getCountOpenList()const;
    uint getCountCloseList()const;
    std::string getPath(world& w, Heuristics::AStarHeuristics& calc);
    void addToOpenList(world& w);
private:
//
    WorldContainer worldList;
    WorldContainer worldToList;
    ParentList parents;
// Der erste Vector steht für die Kosten
// Die Liste steht für eine Welten, die diesen wert erreichen
    OuterListContainer openCosts;
// Diese Liste ist die Geschlossene
    OuterListContainer closeList;

// Helpfunctions
    std::string getSolution(uint wormNumber);
    void init(world& w, Heuristics::AStarHeuristics& h);
    const worldRep getBestWorld(uint&);
};

#endif // ASTARCALCULATION_H
