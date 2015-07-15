#ifndef TAPEWORLD_H
#define TAPEWORLD_H
#include <iostream>
#include <boost/container/vector.hpp>

class WormMap
{
public:

    WormMap();
    WormMap(std::string Source);
    WormMap(const WormMap& old);
    WormMap(const uint x, const uint y);
    ~WormMap();
// Setter
    void setWormMap(std::string Source);
    void setTheHomeFor(const uint x, const uint y, const uint color);
// Getter
    uint getColorCount() const;
    uint getLevel() const;
    uint getMaxSteps() const;
    uint getSizeX() const;
    uint getSizeY() const;
    uint getSizeY(const uint Line) const;
    uint getTheHomeFor(const uint x, const uint y) const;
    bool isBlocked(const uint x, const uint y) const;
// Operatoren
    WormMap& operator=(const WormMap& old);
private:
    boost::container::vector< boost::container::vector<uint> > world;
    uint level;
    uint maxSteps;
    uint sizeX, sizeY;
    uint colorCount;
};

std::ostream& operator<<(std::ostream& stream, const WormMap& m);

#endif // TAPEWORLD_H
