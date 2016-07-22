#include "region.h"
#include <iostream>
#define CELLACCESS(c, r) ((r) * cols + (c))
/**************************************************************************************************
 *                      constructor & Destructor
 * ***********************************************************************************************/
World::Region::Region():
    numberDeadEnd(0),
    numberPassages(0),
    numberUnblocked(0),
    rows(0), cols(0)
{

}

World::Region::Region(uint numCols, uint numRows, std::ifstream& data):
    rows(numRows), cols(numCols)
{
    uint c, r, i;


// block and passage init
    blocked.resize(rows * cols, true);
    passage.resize(rows* cols, false);
// Read the passages
    data >> numberPassages;
    for(i=0; i<numberPassages; ++i){
        data >> c >> r;
        passage.flip( CELLACCESS(c,r));
    }
// Read the unblocked fields
    data >> numberUnblocked;
    for(i=0; i<numberUnblocked; ++i){
        data >> c >> r;
        blocked.flip( CELLACCESS(c,r));
    }

}

/**
  Copy constructor
 * @brief World::Region::Region
 * @param other
 */
World::Region::Region(const Region& other):
    numberDeadEnd(other.numberDeadEnd),
    numberPassages(other.numberPassages),
    numberUnblocked(other.numberUnblocked),
    rows(other.rows), cols(other.cols),
    blocked(other.blocked), passage(other.passage)
{
}

World::Region::~Region(){
}

/**************************************************************************************************
 *                      Getter
 * ***********************************************************************************************/
bool World::Region::isBlocked(const int c, const int r) const{
    if(r >= rows) return true;
    if(c >= cols) return true;
    if(r < 0) return true;
    if(c < 0) return true;
    return blocked[CELLACCESS(c,r)];
}
bool World::Region::isBlocked(const Position& pos) const
{
    return isBlocked(pos.getCol(), pos.getRow());
}

bool World::Region::isPassage(const uint c, const uint r) const{
    return passage[CELLACCESS(c,r)];
}

bool World::Region::isPassage(const Position& pos) const{
    return isPassage(pos.getCol(), pos.getRow());
}

uint World::Region::getNumberPassages() const{
    return numberPassages;
}

uint World::Region::getNumberDeadEnd() const{
    return numberDeadEnd;
}

uint World::Region::getColumnCount()const{
    return cols;
}

uint World::Region::getRowCount()const{
    return rows;
}

uint World::Region::getNumberUnblocked()const{
    return numberUnblocked;
}

std::vector<Position > World::Region::getPassages() const
{
    uint col, row;
    std::vector< Position > result;
    for(col = 0; col < cols; ++col)
    {
        for(row = 0; row < rows; ++row)
        {
            if(isPassage(col, row))
            {
                result.push_back(Position(col, row));
            }
        }
    }
    return result;
}

std::vector<Position > World::Region::getUnblocked() const
{
    uint col, row;
    std::vector< Position > result;
    for(col = 0; col < cols; ++col)
    {
        for(row = 0; row < rows; ++row)
        {
            if(!isBlocked(col, row))
            {
                result.push_back(Position(col, row));
            }
        }
    }
    return result;
}

/**************************************************************************************************
 *
 * ***********************************************************************************************/
void World::Region::swap(const uint c, const uint r){
    blocked.flip(CELLACCESS(c,r));
    if(blocked[CELLACCESS(c,r)] == false)
    {
        numberUnblocked++;
    }else{
        numberUnblocked--;
    }
}

void World::Region::setBlocked(const uint c, const uint r)
{
    //std::cout << "Set Blocked: " << c << ", " << r << std::endl;
    if(blocked[CELLACCESS(c,r)] == false)
        numberUnblocked--;
    blocked[CELLACCESS(c,r)] = true;
}

void World::Region::setUnblocked(const uint c, const uint r)
{
    //std::cout << "Set Unblocked: " << c << ", " << r << std::endl;
    if(blocked[CELLACCESS(c,r)] == true)
        numberUnblocked++;
    blocked[CELLACCESS(c,r)] =false;
}

/**************************************************************************************************
 *                          output
 *************************************************************************************************/
std::ostream& operator <<(std::ostream& out, const World::Region& w){
    for(uint c=0; c<w.getColumnCount(); ++c){
        out << "-";
    }
    out << "--" << std::endl;
    for(uint r=0; r<w.getRowCount(); ++r){

        out << "|";
        for(uint c=0; c<w.getColumnCount(); ++c){

            if(w.isPassage(c,r)){
                out << "O";
            } else if(w.isBlocked(c,r)){
                out << "X";
            } else {
                out << " ";
            }

        }
        out << "|" <<std::endl;
    }
    for(uint c=0; c<w.getColumnCount(); ++c){
        out << "-";
    }
    out << "--" << std::endl;
    return out;
}

/**************************************************************************************************
 *                                  operator
 * ***********************************************************************************************/
World::Region& World::Region::operator=(const World::Region& other)
{
    numberDeadEnd = other.numberDeadEnd;
    numberPassages = other.numberPassages;
    numberUnblocked = other.numberUnblocked;
    rows = other.rows;
    cols = other.cols;
    blocked = other.blocked;
    passage = other.passage;
    return *this;
}

#undef CELLACCESS
