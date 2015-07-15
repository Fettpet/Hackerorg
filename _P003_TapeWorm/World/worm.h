#ifndef WORM_H
#define WORM_H
#include <boost/container/vector.hpp>
#include <string>
#include <boost/container/set.hpp>
#include <cstdlib>
#include <iostream>
#include <deque>
#include <set>
#include "World/pos.h"

enum MoveDircetion{
   Nothing, Up, Down, LeftUp, LeftDown, RightUp, RightDown
};

/**************************************************************************************************
 *                                      typedefs
 * ***********************************************************************************************/
typedef unsigned char WormPosIntern;
typedef Pos<WormPosIntern> WormPart;

typedef unsigned short Color;
typedef std::deque<WormPart> WormPartContainer;
/**************************************************************************************************
 *                                  Klasse
 * ***********************************************************************************************/
class Worm
{
public:
// Constructor
    Worm();
    Worm(std::string str);
    Worm(const Worm& old);
    Worm(const std::set<WormPart> parts);
    ~Worm();
// Getter
    uint getId() const;
    WormPart getHead()const;
    WormPart getTail()const;
    const WormPart& getPart(const uint number)const;
    uint getLength() const;
    Color getColor() const;
    WormPartContainer& getWormParts();
    const WormPartContainer& getWormParts()const;
    bool contains(const WormPart& p) const;
    void calcHash();
// setter
    void setColor(const Color color);
// TestFunktionen
    void move(const MoveDircetion direction, const bool head);
    bool move(const WormPart& start, const WormPart& destiny);
    void move(const WormPart& destiny, const bool& moveHead);
    static WormPart getNewPos(const MoveDircetion dir, const WormPart part);
// operatoren
    Worm& operator=(const Worm& old);
    bool  operator==(const Worm& other)const;
    bool operator<(const Worm& other) const;
private:
    long hash;
    uint id;
    int posHead;
    Color color;
    static unsigned int globalId;
    WormPartContainer wormparts;
    uint length;

};

 std::ostream& operator<<(std::ostream& str, const Worm& t);

#endif // WORM_H
