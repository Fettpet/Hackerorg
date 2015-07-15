#include "wormmap.h"
#include <map>
#include <cstdlib>
/*********************************************************************************************************
                                                Konstruktoren
*********************************************************************************************************/


/** StandartKonstruktor
  setzt alles auf 0
*/

WormMap::WormMap():
    sizeX(0), sizeY(0)
{}
/**
    @param Source den Quelltext von Hacker.org als String übergeben KEINE URL
*/
WormMap::WormMap(std::string Source){
    if(!Source.empty()){
        setWormMap(Source);
    }
}
/**
    @param Eine Alte Wormmap
    Diese Funktion ist der Kopierkonstruktor
*/
WormMap::WormMap(const WormMap &old):
    world(old.world),
    level(old.level),
    maxSteps(old.maxSteps),
    sizeX(old.sizeX), sizeY(old.sizeY)
{}
/**
    Der Destruktor
*/
WormMap::~WormMap(){
    world.clear();
}

/**
    Erstellt eine Map mit der Größe (x, y) = false;
 * @brief WormMap::WormMap
 * @param x
 * @param y
 */
WormMap::WormMap(const uint x, const uint y){
    for(uint i=0; i<y; ++i){
        boost::container::vector< uint> buffer;
        for(uint j=0; j<x; ++j){
            buffer.push_back(0);
        }
        world.push_back(buffer);
    }
}

/**********************************************************************************************************
                                                Setter
**********************************************************************************************************/
/**
    @param Source den Quelltext von Hacker.org
    Ließt den Source ein und erstellt eine Map
*/
void WormMap::setWormMap(std::string Source){
    std::string boardString;
    colorCount = 0;
    world.clear();
// Den boardString extrahieren
    boardString = Source.substr(Source.find("boardString"));
    boardString = boardString.substr(boardString.find("\"")+1);
    boardString = boardString.substr(0, boardString.find("\""));
// Den String ins Feld übernehmen
    do{
        if(boardString.at(0) ==',') boardString.erase(0,1);

        std::string buffer;
        if(boardString.find(',') != std::string::npos) {
            buffer = boardString.substr(0,boardString.find(','));
        } else {
            buffer = boardString;
        }

        boost::container::vector<uint> Line;

        for(uint i=0; i<buffer.size(); ++i){
            uint entry(std::atoi(buffer.substr(i,1).c_str()));
            if(entry > colorCount)
                colorCount = entry;
            Line.push_back(entry);
        }
        world.push_back(Line);
        if(boardString.find(',') != std::string::npos)
            boardString = boardString.substr(boardString.find(','));
    }while(boardString.find(',') != std::string::npos);
    // Das aktuelle Level lesen
    std::string levelBuffer;
    levelBuffer = Source.substr(Source.find("curLevel"));
    levelBuffer = levelBuffer.substr(levelBuffer.find("=")+1);
    levelBuffer = levelBuffer.substr(0, levelBuffer.find(";"));
    level = std::atoi(levelBuffer.c_str());
    // Die Maximale Anzahl Schritte
    std::string Moves = Source.substr(Source.find("moves"));
    Moves = Moves.substr(Moves.find("=")+1);
    maxSteps=std::atoi(Moves.substr(0,Moves.find(";")).c_str());
}


void WormMap::setTheHomeFor(const uint x, const uint y, const uint color){
    if(colorCount < color){
        colorCount = color;
    }
    world[x][y] = color;
}
/**************************************************************************************************************
                                Getter
**************************************************************************************************************/
/**
    @return Den Aktuellen Level
*/
uint WormMap::getLevel() const{
    return level;
}

uint WormMap::getColorCount() const{
    return colorCount;
}
/**
    @return Die Anzahl an Schritten, die maximal gegangen werden darf.
*/
unsigned int WormMap::getMaxSteps() const{
    return maxSteps;
}
/**  getSizeX
    gibt die Breite des Feldes zurück
    @return die Breite der Welt
*/
unsigned int WormMap::getSizeX() const{
    return world.size();
}
/**  getSizeY
    gibt die Höhe der höchsten Spalte zurück
    @return die Höhe der höchsten Spalte
*/
unsigned int WormMap::getSizeY() const{
    unsigned int result = 0;
    for(uint i=0; i< world.size(); ++i){
        if(world[i].size() > result)
            result = world[i].size();
    }
    return result;
}
/**  getSizeY
    gibt die Höhe einer Spalte zurück
    @param die Spaltenummer
    @return die Höhe der Spalte
*/
unsigned int WormMap::getSizeY(const unsigned int Line) const{
    if(Line >= world.size()) {
        std::cerr << "Line ist zugroß in getSizeY(Line)";
        return 0;
    }
    return world[Line].size();
}
/**
    @param x die x koordinate
    @param y die y koordinate
    @return 0, falls kein Wurm zuhause ist, Id des wurms sonst
*/
unsigned int WormMap::getTheHomeFor(const unsigned int x, const unsigned int y) const{
    if(x >= getSizeX()) {
    //    std::cerr << "x ist zu groß in WormMap::getTheHomeFor x="<<x;
        return 0;
    }
    if(y>= getSizeY(x)){
    //    std::cerr << "y is zu groß in WormMap::getTheHomeFor y="<<y;
        return 0;
    }
    return world[x][y];
}
/**
    @param x die x koordinate
    @param y die y koordinate
    @return true, falls es geblockt ist, ansonsten false
*/
bool WormMap::isBlocked(const unsigned int x, const unsigned int y) const{
    if(x >= world.size()) {
     //   std::cerr << "Fehler in WormMap::isBlocked x ist zu groß x="<<x;
        return true;
    }
    if(y >= world[x].size()){
    //    std::cerr << "Fehler in WormMap::isBlocked y ist zu groß y="<<y;
        return true;
    }
    return (world[x][y] == 1);
}
/**************************************************************************************************
                                    operatorn
**************************************************************************************************/
// Operatoren
WormMap& WormMap::operator=(const WormMap& old){
    sizeX = old.sizeX;
    sizeY = old.sizeY;
    world = old.world;
    return *this;
}

/**************************************************************************************************
 *                                      output
 * ***********************************************************************************************/
std::ostream& operator<<(std::ostream& stream, const WormMap& m){
    for(uint y=0; y<m.getSizeY(); ++y){
        for(uint x=0; x<m.getSizeX(); ++x){
            stream << m.getTheHomeFor(x,y);
        }
        stream << std::endl;
    }
    return stream;
}
