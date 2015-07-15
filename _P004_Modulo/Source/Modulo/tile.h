#ifndef TILE_H
#define TILE_H
#include <vector>
#include <string>
#include <iostream>

typedef unsigned int uint;

class Tile
{
public:
    Tile(const uint sizeX, const uint sizeY);
    Tile(const uint sizeX, const uint sizeY, const int solX, const int solY);
    Tile(std::string& t);
    Tile(const Tile& other);

    Tile& operator=(const Tile& other);
// Getter
    uint getSizeX() const;
    uint getSizeY() const;
    int getSolutionX() const;
    int getSolutionY() const;
    bool getField(const uint x, const uint y) const;
    uint getId()const;
    uint getCountSet() const;
// Setter
    void setSolutionX(const int solX);
    void setSolutionY(const int solY);
    void resize(const uint _sizeX, const uint sizeY);
    void setField(const uint x, const uint y, const bool value);
private:
    uint countSet;
    uint sizeX, sizeY;
    int solutionX, solutionY;
    static uint globalId;
    uint ID;

    std::vector< std::vector< bool> > tiles;
};

std::ostream& operator<<(std::ostream& out, const Tile& other);
#endif // TILE_H
