/* Container Klasse die die Würmer und die Welt verwaltet

*/
#ifndef WORLD_H
#define WORLD_H
#include <sstream>
#include <ostream>
#include "worm.h"
#include "World/pos.h"
#include <boost/dynamic_bitset.hpp>
#include "wormmap.h"
#include <deque>

class world;

/**************************************************************************************************
 *                                  Typedefs
 * ***********************************************************************************************/
typedef struct {
    MoveDircetion direction;
    bool head;
    unsigned int wormNumber;
} possibleMoves;

typedef std::deque<world> WorldContainer;
typedef boost::container::vector<possibleMoves> PossibleMoveContainer;
typedef std::deque<WormPart> NeighborContainer;
typedef std::vector<Worm> WormContainer;
typedef std::set<uint> IgnoringListContainer;

/**************************************************************************************************
 *                                          class
 * ***********************************************************************************************/
class world
{
public:
// Konstructor, Destructor
    world();
    ~world();
    world(const world& old);
    uint createNewWorldByMovingWorm(const possibleMoves &t, WorldContainer& wContainer)const;
    uint createNewWorldByMovingWorm(const possibleMoves &t,
                                    WorldContainer &wContainer,
                                    const IgnoringListContainer& IgnoringList) const;
    //QImage getTheWorldAsImage();
    void ReadFromHtmlData(const std::string& filename);

    virtual void move(const possibleMoves& Move);
    virtual void move(const uint wormNumber, const MoveDircetion direction, const bool head);
    virtual bool move(const WormPart& start, const WormPart& Destiny);
// Getter
    WormMap& getWormMap();
    const WormMap& getWormMap() const;
    uint getSizeX() const;
    uint getSizeY() const;
    uint getCurrentSteps() const;
    uint getWormCount()const;
    const Worm& getWorm(const uint nr)const ;
    Worm& getWorm(const uint nr);
    const WormContainer& getWorms() const;
    WormContainer getWorms();
    uint getCountWormPartsAtHome()const;
    uint getCountWormParts()const;
    void getAllPosibleMoves(PossibleMoveContainer& result ) const;
    virtual std::string getPath() const;
    NeighborContainer* getNeighborFields(const WormPart& t) const;
    NeighborContainer* getNeighborFields(const uint x, const uint y) const;
// block test
    virtual bool isBlocked(const WormPart& part, uint worm)const;
// Operatoren
    world& operator=(const world& old);
    bool operator==(const world& old) const ;
    bool operator<(const world& old)const;
    friend std::ostream& operator<<(std::ostream& stream, const world&);

// Deleter
    void deleteWorm(const uint id);
    void deleteWorm(const Worm& id);
    void deleteLastMove();

    void setWorms(const WormContainer& w) ;

    long getHash() const;
    uint getSolutionLength() const;


    WormPart getLastMoveStart() const;

    WormPart getLastMoveDestiny() const;

    bool getLastMove() const;
protected:
    long Hash;
    WormContainer worms;
    WormPart lastMoveStart, lastMoveDestiny;
    static WormMap map;
    uint CurrentSteps;
    bool lastMove;
// Funktionen
    unsigned long int getColor(unsigned int Id);
    void createWorld(const std::string& Source);

private:
    static std::vector< std::vector < NeighborContainer > > NeighborHood;

    void createNeighood();
};

std::ostream& operator<<(std::ostream& stream, const possibleMoves& Move);
#endif // WORLD_H
