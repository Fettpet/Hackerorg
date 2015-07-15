#include "tile.h"
#include "../Exceptions/stringexception.h"
uint Tile::globalId(1);
/**************************************************************************************************
 *                                  Constructor
 * ***********************************************************************************************/
Tile::Tile(std::string& t):
    sizeX(0), sizeY(0), solutionX(-1), solutionY(-1), ID(globalId), countSet(0)
{
    globalId++;
    std::vector< bool > buffer;
    for(char c: t){
        if(c == ','){
            tiles.push_back(buffer);
            buffer.clear();
        } else if(c == 'X'){
            buffer.push_back(true);
        } else {
            buffer.push_back(false);
        }
    }
    tiles.push_back(buffer);
    sizeY = tiles.size();
    sizeX = tiles[0].size();

    for(uint x= 0; x < sizeX; ++x){
        for(uint y=0; y<sizeY; ++y){
            countSet += tiles[y][x];
        }
    }
}

Tile::Tile(const Tile& other):
    sizeX(other.sizeX), sizeY(other.sizeY),
    solutionX(other.solutionX), solutionY(other.solutionY),
    ID(other.ID), tiles(other.tiles), countSet(other.countSet)
{
}

Tile::Tile(const uint _sizeX, const uint _sizeY):
    sizeX(_sizeX), sizeY(_sizeY), solutionX(-1), solutionY(-1), ID(globalId)
{
    globalId++;
    std::vector< bool > buffer;
    for(uint y=0; y<sizeX; ++y){
        buffer.push_back(false);
    }
    for(uint x=0; x<sizeY; ++x){
        tiles.push_back(buffer);
    }
    countSet =0;
}

Tile::Tile(const uint _sizeX, const uint _sizeY, const int solX, const int solY):
    sizeX(_sizeX), sizeY(_sizeY), solutionX(solX), solutionY(solY), ID(globalId)
{
    globalId++;
    std::vector< bool > buffer;
    for(uint y=0; y<sizeX; ++y){
        buffer.push_back(false);
    }
    for(uint x=0; x<sizeY; ++x){
        tiles.push_back(buffer);
    }
    countSet = 0;
}

/**************************************************************************************************
 *                                          Getter
 * ***********************************************************************************************/

uint Tile::getSizeX() const{
    return sizeX;
}

uint Tile::getSizeY() const{
    return sizeY;
}
bool Tile::getField(const uint x, const uint y) const{
    if(x >= sizeX) return false;
    if(y >= sizeY) return false;
    return tiles[y][x];
}

uint Tile::getId()const{
    return ID;
}

int Tile::getSolutionX() const{
    return solutionX;
}

int Tile::getSolutionY() const{
    return solutionY;
}

uint Tile::getCountSet() const{
    return countSet;
}
/**************************************************************************************************
 *                                              Setter
 * *****************************++****************************************************************/
/**
 * @brief resize chance the size of the tile. Set all values to 0.
 * @param _sizeX: The new count of rows
 * @param _sizeY: The new count of columns
 */
void Tile::resize(const uint _sizeX, const uint _sizeY){
    sizeX = _sizeX;
    sizeY = _sizeY;
     std::vector< bool> buffer;
    for(uint x=0; x<sizeY; ++x){
        buffer.push_back(0);
    }
    for(uint y=0; y<sizeX; ++y){
        tiles.push_back(buffer);
    }
}

/**
 * @brief setField: set the value to value of the field (x,y)
 * @param x
 * @param y
 * @param value
 */
void Tile::setField(const uint _x, const uint _y, const bool _value){
    if(_y >= tiles.size()){
        throw Exception::StringException("The size x is to big");
    }
    if(_x >= tiles[0].size()){
        throw Exception::StringException("The size y is to big");
    }
// The setting
    if(tiles[_y][_x] != _value){
        if(_value == true){
            countSet++;
        } else {
            countSet--;
        }
    }
    tiles[_y][_x] = _value;
}

void Tile::setSolutionX(const int solX){
    solutionX = solX;
}

void Tile::setSolutionY(const int solY) {
    solutionY = solY;
}

/**************************************************************************************************
 *                                      operator
 * ***********************************************************************************************/
Tile& Tile::operator=(const Tile& other){
    sizeX = other.sizeX;
    sizeY = other.sizeY;
    tiles = other.tiles;
    ID = other.ID;
    return *this;
}

/**************************************************************************************************
 *                          output
 * ***********************************************************************************************/
std::ostream& operator<<(std::ostream& out, const Tile& other){
    uint countTile = 2 * other.getSizeX() + 1;
    for(uint i=0; i<countTile; ++i){
        out << "-";
    }
    out << std::endl;
    for(uint y=0; y<other.getSizeY(); ++y){
        out << "|";
        for(uint x=0; x<other.getSizeX(); ++x){
            if(other.getField(x,y)){
                out << "X|";
            }else{
                out << " |";
            }
        }
        out << std::endl;
        for(uint i=0; i<countTile; ++i){
            out << "-";
        }
        out << std::endl;
    }
    return out;
}
