#include <fstream>
#include "world.h"
#include <cstdlib>
//#include <QPainter>

#define CELLACCESS(X,Y) X*cols+Y

using namespace World;
world::world():
    numberDeadEnd(0),
    numberPassageList(0),
    rows(0),
    cols(0),
    numberRegions(0)
{
}


/**************************************************************************************************
 *                      Getter
 * ***********************************************************************************************/
uint world::getColumnCount()const{
    return cols;
}

uint world::getRowCount()const{
    return rows;
}

bool world::isBlocked(const uint col, const uint row) const{
    return blocked[CELLACCESS(row, col)];
}

uint world::getNumberRegions() const{
    return numberRegions;
}

void world::readFromFile(const std::string& file){
    std::ifstream data;
    data.open(file.c_str());
    readTheWorld(data);
    readTheRegions(data);
    data.close();
}

uint world::getNumberDeadEnds() const{
    return numberDeadEnd;
}

World::Region& World::world::getRegion(const uint nr){
    return regions[nr];
}

const World::Region& World::world::getRegion(const uint nr) const
{
    return regions[nr];
}

Position World::world::getDeadEnd(uint id) const
{
    return deadEndList[id];
}

const std::vector< Position >& World::world::getDeadEnds() const
{
    return deadEndList;
}

const std::vector<World::Region>& World::world::getRegions() const
{
    return regions;
}


const std::vector<Solver::Data::Pos< uint > >& World::world::getPassages() const
{
    return passageList;
}

/**************************************************************************************************
 *                          Read from file
 * ***********************************************************************************************/

void world::readTheRegions(std::ifstream& file){
    file >> numberRegions;
    for(uint i=0; i<numberRegions; ++i){
        regions.push_back(World::Region(cols, rows, file));
    }
}


void world::readTheWorld(std::ifstream &file){
    file >> cols;
    file >> rows;
    blocked.resize(rows * cols);
    for(uint r=0; r<rows;++r){
        for(uint c=0; c<cols;++c){
            char buffer;
            file >> buffer;
            if(buffer == '0'){
                blocked[CELLACCESS(r, c)] = false;
            } else if(buffer == '1'){
                blocked[CELLACCESS(r, c)] = true;
            } else if(buffer == '2'){
                blocked[CELLACCESS(r, c)] = false;
                passageList.push_back( Position(c, r));
                numberPassageList++;
            } else if(buffer == '3'){
                blocked[CELLACCESS(r, c)] = false;
                deadEndList.push_back( Position(c,r));
                numberDeadEnd++;
            }
        }
    }

}

/**************************************************************************************************
 *                          outstream
 * ***********************************************************************************************/
std::ostream& operator<<(std::ostream& out, const world& w){
    for(uint c=0; c<w.getColumnCount(); ++c){
        out << "X";
    }
    out << "XX" << std::endl;

    for(uint r=0; r<w.getRowCount(); ++r){
        out << "X";
        for(uint c=0; c<w.getColumnCount(); ++c){
            if(w.isBlocked(c,r)){
                out << "X";
            } else {
                out << " ";
            }

        }
        out << "X" <<std::endl;
    }
    for(uint c=0; c<w.getColumnCount(); ++c){
        out << "X";
    }
    out << "XX" << std::endl;
    return out;
}
/*
QImage world::transform() const{
    QImage img((cols+2)*15, (rows+2)*15, QImage::Format_RGB32);
    QPainter painter(&img);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(Qt::red);
    painter.drawRect(0,0,(cols+2)*15, 15);
    painter.end();
    img.save("/home/fettpet/aus.png");
    return img;
}
*/
#undef CELLACCESS
