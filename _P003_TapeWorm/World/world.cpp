#include "world.h"
#include <fstream>


std::vector< std::vector < NeighborContainer > > world::NeighborHood;
WormMap world::map;
/**************************************************************************************************
                            Konstruktoren
**************************************************************************************************/

world::world():
    Hash(0),
    CurrentSteps(0),
    lastMove(false)
{
}

world::world(const world& old):
    Hash(old.Hash),
    worms(old.worms),
    lastMoveStart(old.lastMoveStart),
    lastMoveDestiny(old.lastMoveDestiny),
    CurrentSteps(old.CurrentSteps),
    lastMove(old.lastMove)
{
}

world::~world()
{    }

/********************************************************************************************************
                                                Ausgabe als Bild
********************************************************************************************************/
/*
#include <QImage>
#include <QVector>
QImage world::getTheWorldAsImage(){
    const unsigned int BlockLengthX = 30;
    const unsigned int BlockLengthY = 30;
    unsigned int sizeX, sizeY;
    sizeX = map.getSizeX() *BlockLengthX;
    sizeY = map.getSizeY() * BlockLengthY+BlockLengthY/2;
    QImage result(sizeX, sizeY, QImage::Format_RGB32);
    // Das Feld
    for(unsigned int x=0; x< map.getSizeX(); ++x){
        for(unsigned int y=0; y<map.getSizeY(); ++y){
            unsigned long int Color = getColor(map.getTheHomeFor(x,y));
            bool up = (x %2);
            for(unsigned int i=0; i< BlockLengthX; ++i){
                for(unsigned int j=0; j<BlockLengthY; ++j){
                    result.setPixel(x*BlockLengthX+i,y*BlockLengthY+j + up*BlockLengthY/2, Color);
                }
                result.setPixel(x*BlockLengthX+i,y*BlockLengthY+ up*BlockLengthY/2, 0);
            }
        }
        for(int i=0; i< result.height(); ++i){
            result.setPixel(x*BlockLengthX,i,0);
        }
    }
    // Die Würmer
    for(Worm wurm : worms){
         unsigned long int Color = getColor(wurm.getColor());
         for(WormPart part: wurm.getWormParts()){
             unsigned int x = part.getPosX();
             unsigned int y = part.getPosY();
             unsigned int up = x % 2;
             for(unsigned int i=BlockLengthX*0.25; i< BlockLengthX*0.75; ++i){
                 for(unsigned int j=BlockLengthY*0.25; j<BlockLengthY*0.75; ++j){
                     result.setPixel(x*BlockLengthX+i,y*BlockLengthY+j + up*BlockLengthY/2, Color);
                 }
                 result.setPixel(x*BlockLengthX+i,y*BlockLengthY+ up*(BlockLengthY)/2 + BlockLengthY*0.25, 0);
                 result.setPixel(x*BlockLengthX+i,y*BlockLengthY+ up*(BlockLengthY)/2 + BlockLengthY*0.75, 0);
                 result.setPixel(x*BlockLengthX + BlockLengthX*0.25, y*BlockLengthY+ up*(BlockLengthY)/2 + i, 0);
                 result.setPixel(x*BlockLengthX + BlockLengthX*0.77, y*BlockLengthY+ up*(BlockLengthY)/2 + i, 0);
                 result.setPixel(x*BlockLengthX + i, y*BlockLengthY+ up*(BlockLengthY)/2+ i, 0);
                 result.setPixel((x+1)*BlockLengthX - i, y*BlockLengthY+ up*(BlockLengthY)/2+ i, 0);
             }
         }
    }

    return result;
}

unsigned long int world::getColor(unsigned int Id){
    QVector<unsigned long int> Farben;
;
    Farben.push_back(0xffffff);
    Farben.push_back(0);
    Farben.push_back(0xff0000);
    Farben.push_back(0x0000ff);
    Farben.push_back(0xffFF00);
    Farben.push_back(0x00ffff);
    Farben.push_back(0xff00ff);
     Farben.push_back(0xff00ff);
    return Farben[Id];
}


//*/
/*******************************************************************************************************
                                        Laden der Umgebung
*********************************************************************************************************/


/* Ließt den Source aus einer Html Datei aus, welche bei Hacker.org gedownloaded wurde
  @param filename Der Dateiname
*/

void world::ReadFromHtmlData(const std::string& filename){
    std::fstream datei;
    datei.open(filename.c_str(), std::ios::in);
    if(!datei.is_open()){
        std::cerr << "Datei konnte in ReadFromHtmlData nicht geöffnet werden Dateiname:" << filename.c_str() << std::endl;
        return;
    }
    std::string buffer;
    std::string Source;
    do{
        datei >> buffer;
        Source.append(buffer.c_str());
    }while(!datei.eof());

    createWorld(Source);
}

void world::createWorld(const std::string& Source){
    worms.clear();

// DIe Karte setzen
    map.setWormMap(Source);
// Die Würmer auslesen

    std::string WormString;

    WormString = Source.substr(Source.find("wormString"));
    WormString = WormString.substr(WormString.find("\"")+1);
    WormString = WormString.substr(0, WormString.find("\""));
    do{
        if(WormString.at(0) =='@') WormString.erase(0,1);

        std::string buffer;
        if(WormString.find('@') != std::string::npos) {
            buffer = WormString.substr(0,WormString.find('@'));
        } else {
            buffer = WormString;
        }

        // Auf das Feld übertragen
        worms.push_back(Worm(buffer));
        if(WormString.find('@') != std::string::npos)
            WormString = WormString.substr(WormString.find('@'));
    }while(WormString.find('@') != std::string::npos);
    createNeighood();
}

/**************************************************************************************************
                            moves
**************************************************************************************************/

/**
   bewegt einen Wurm in die angegebene Richtung
   WICHTIG: Testet nicht auf VALIDÄT
   @param Move, Die Bewegung, die man mittels getAllPossibleMoves erhält
*/

void world::move(const possibleMoves& Move){
    move(Move.wormNumber, Move.direction, Move.head);
}

/**
   bewegt einen Wurm in die angegebene Richtung
   WICHTIG: Testet nicht auf VALIDÄT
   @param WormNumber
         der Wurm, der Bewegt werden soll
   @param direction
         Die Richtung, in der sich der Wurm bewegen soll
   @param head
        Bei True wird der Kopf bewegt
        Bei false wird der Schwanz bewegt
*/
void world::move(const uint wormNumber, const MoveDircetion direction, const bool headMoving){
// Set the tail to not blocked
    std::cout << "Old move";
    if(headMoving){
        WormPart p(worms[wormNumber].getTail());
    } else {
        WormPart p(worms[wormNumber].getHead());
    }
// move
    worms[wormNumber].move(direction, headMoving);
// Set the Head to blocked
    if(headMoving){
        WormPart p(worms[wormNumber].getHead());
    } else {
        WormPart p(worms[wormNumber].getTail());
    }
}

bool world::move(const WormPart& start, const WormPart& destiny){
    std::cout << "old move" << std::endl;
    if(start == destiny) return false;
    for(uint i=0; i<worms.size(); ++i){
        if(worms[i].contains(destiny)){
            if(isBlocked(destiny, i)) break;
            if(worms[i].move(start, destiny)){
                lastMoveStart = start;
                lastMoveDestiny = destiny;
                return true;
            } else{
                return false;
            }
        }
    }
    return false;
}

/*************************************************************************************************
                                Getter
**************************************************************************************************/
uint world::getSolutionLength() const{
    std::string path = getPath();
    uint result(0);
    for(char c: path){
        if(c == 'm') result++;
    }
    return result;
}

const WormContainer& world::getWorms() const{
    return worms;
}

WormContainer world::getWorms(){
    return worms;
}


WormMap& world::getWormMap(){
    return map;
}

const WormMap& world::getWormMap() const{
    return map;
}

uint world::getSizeX() const{
    return map.getSizeX();
}

uint world::getSizeY() const{
    return map.getSizeY();
}


/**
    @return die Anzahl der existierenden Würmer
*/
unsigned int world::getWormCount()const{
    return worms.size();
}

/**
  @return path, den Weg zum Ziel
*/
std::string world::getPath()const{
    std::stringstream str;

    if(lastMoveStart == lastMoveDestiny){
            std::cerr << "LastmoveStart == lastmoveDEst";

    } else if(lastMoveStart.getPosX() >= getSizeX() || lastMoveDestiny.getPosX() >= getSizeX()){

    } else if(lastMoveStart.getPosY() >= getSizeY() || lastMoveDestiny.getPosY() >= getSizeY()){

    } else{
    str << "m" << (int)lastMoveStart.getPosX() <<"." << (int)lastMoveStart.getPosY()
        << "-" << (int)lastMoveDestiny.getPosX() << "." << (int)lastMoveDestiny.getPosY();
    }
    return str.str();
}

/**
    @param nr die Nummer des Wurms
    @return einen Pointer auf einen Wurm
*/
const Worm& world::getWorm(const unsigned int nr)const {
    return (worms[nr]);
}

Worm& world::getWorm(const unsigned int nr) {
    return (worms[nr]);
}

/**
    Berechnet die Anzahl an Feldern die von den Passend gefärbten Würmern bedeckt sind
    @return Die Anzahl
*/
uint world::getCountWormPartsAtHome()const{
    uint result(0);
    for(const Worm& wurm: worms){
        register const uint wormColor(wurm.getColor());
        for(const WormPart& part: wurm.getWormParts()){
            uint mapColor(map.getTheHomeFor(part.getPosX(), part.getPosY()));
            result += (mapColor != wormColor);
        }
    }
    return result;
}

/**
    @return die Anzahl der Wurmteile
*/
unsigned int world::getCountWormParts()const{
    unsigned int result = 0;
    for(uint i=0; i< worms.size(); ++i){
        result += worms[i].getLength();
    }
    return result;
}

/**
  how many times was was a wurm moving
 * @brief world::getCurrentSteps
 * @return
 */
uint world::getCurrentSteps()const{
    return CurrentSteps;
}


/**
 * @brief world::getAllPosibleMoves
 * @param result
 * Haut alle Möglichen bewegungen in einen Vector
 */
void world::getAllPosibleMoves(PossibleMoveContainer& result) const{
    for(uint i=0; i<worms.size(); ++i){

// Den Kopf anschauen
        WormPart current, buffer;
        current = worms[i].getHead();
        // Nach Oben
        buffer = Worm::getNewPos(Up,current);

        if(!isBlocked(buffer, i)){
            possibleMoves newMove;
            newMove.direction = Up;
            newMove.head = true;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Nach Unten
        buffer = Worm::getNewPos(Down,current);
        if(!isBlocked(buffer, i)){
            possibleMoves newMove;
            newMove.direction = Down;
            newMove.head = true;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Links Unten
        buffer = Worm::getNewPos(LeftDown,current);
        if(!isBlocked(buffer,i )){
            possibleMoves newMove;
            newMove.direction = LeftDown;
            newMove.head = true;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Nach LinksOben
        buffer = Worm::getNewPos(LeftUp,current);
        if(!isBlocked(buffer,i)){
            possibleMoves newMove;
            newMove.direction = LeftUp;
            newMove.head = true;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Nach RechtsUnten
        buffer = Worm::getNewPos(RightDown,current);
        if(!isBlocked(buffer, i)){

            possibleMoves newMove;
            newMove.direction = RightDown;
            newMove.head = true;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Nach RechtsOben
        buffer = Worm::getNewPos(RightUp,current);
        if(!isBlocked(buffer, i)){
            possibleMoves newMove;
            newMove.direction = RightUp;
            newMove.head = true;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }
// Den Schwanz anschauen

        current = worms[i].getTail();
        // Nach Oben
        buffer = Worm::getNewPos(Up,current);
        if(!isBlocked(buffer,i)){
            possibleMoves newMove;
            newMove.direction = Up;
            newMove.head = false;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Nach Unten
        buffer = Worm::getNewPos(Down,current);
        if(!isBlocked(buffer, i)){
            possibleMoves newMove;
            newMove.direction = Down;
            newMove.head = false;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Links Unten
        buffer = Worm::getNewPos(LeftDown,current);
        if(!isBlocked(buffer, i)){
            possibleMoves newMove;
            newMove.direction = LeftDown;
            newMove.head = false;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Nach LinksOben
        buffer = Worm::getNewPos(LeftUp,current);
        if(!isBlocked(buffer,i)){
            possibleMoves newMove;
            newMove.direction = LeftUp;
            newMove.head = false;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Nach RechtsUnten
        buffer = Worm::getNewPos(RightDown,current);
        if(!isBlocked(buffer,i)){
            possibleMoves newMove;
            newMove.direction = RightDown;
            newMove.head = false;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }

        // Nach RechtsOben
        buffer = Worm::getNewPos(RightUp,current);
        if(!isBlocked(buffer,i)){
            possibleMoves newMove;
            newMove.direction = RightUp;
            newMove.head = false;
            newMove.wormNumber = i;
            result.push_back(newMove);
        }
    }
}

/**
 * Schaut nach ob auf das Feld blockiert ist
  @return
        true das Feld ist blockiert
        false das Feld ist nicht blockiert
   WICHITG überprüft den aktuellen Zustand, beachtet den Schwanz nicht des Wurmes wurmNr nicht
*/
bool world::isBlocked(const WormPart& part, uint wormNr)const{
    std::cout << "Old block methode" << std::endl;
    if(part.getPosX() >= map.getSizeX()) return true;
    if(part.getPosY() >= map.getSizeY(part.getPosX())) return true;
    if(map.getTheHomeFor(part.getPosX(), part.getPosY()) == 1) return true;
    if(worms[wormNr].getHead() == part) return false;
    if(worms[wormNr].getTail() == part) return false;

    for(const Worm w:worms){
        if(w.contains(part)) return true;
    }

    return false;
}

void world::setWorms(const WormContainer& w){
    worms = w;
}

long world::getHash() const {
    return Hash;
}


WormPart world::getLastMoveStart() const{
    return lastMoveStart;
}

WormPart world::getLastMoveDestiny() const{
    return lastMoveDestiny;
}

bool world::getLastMove() const{
    return lastMove;
}

/**************************************************************************************************
 *                                  Get all neighbor fields
 *********************************++*************************************************************/
NeighborContainer* world::getNeighborFields(const WormPart& t) const{
    return getNeighborFields(t.getX(), t.getY());
}

// optimierbar
NeighborContainer* world::getNeighborFields(const uint x, const uint y) const{
    return &(NeighborHood[x][y]);
}


void world::createNeighood() {
    NeighborHood.clear();
    for(uint x = 0; x< getSizeX(); ++x){
        std::vector< NeighborContainer > buffer;
        for(uint y = 0; y<getSizeY(); ++y){
            NeighborContainer result;
            // Up and Down
            if(y>0) result.push_back(WormPart(x, y-1));
            if(y<getSizeY()-1) result.push_back(WormPart(x, y+1));
            // Left and Right
            if(x>0) result.push_back(WormPart(x-1, y));
            if(x<getSizeX()-1) result.push_back(WormPart(x+1, y));
            // (Left or Right) Down
            if((x & 1) == 1){
                if((x>0) && (y<getSizeY()-1))
                    result.push_back(WormPart(x-1, y+1));
                if((x<getSizeX()-1) && (y<getSizeY()-1))
                    result.push_back(WormPart(x+1, y+1));
            }else{
                if((x>0) && (y> 0))
                    result.push_back(WormPart(x-1, y-1));
                if((x<getSizeX()-1) && (y > 0))
                    result.push_back(WormPart(x+1, y-1));
            }
            buffer.push_back(result);
        }
        NeighborHood.push_back(buffer);
    }
}

/**************************************************************************************************
                                    Create
**********************++++************************************************************************/

uint world::createNewWorldByMovingWorm(const possibleMoves &t, WorldContainer& container) const{
    uint result(container.size());
    container.push_back(*this);
    if(t.head){
        container[result].lastMoveStart = container[result].getWorm(t.wormNumber).getHead();
    }else{
        container[result].lastMoveStart = container[result].getWorm(t.wormNumber).getTail();
    }
    container[result].getWorm(t.wormNumber).move(t.direction, t.head);
    if(t.head){
        container[result].lastMoveDestiny = container[result].getWorm(t.wormNumber).getHead();
    }else{
        container[result].lastMoveDestiny = container[result].getWorm(t.wormNumber).getTail();
    }

    container[result].CurrentSteps = this->CurrentSteps+1;
    return result;
}

/**************************************************************************************************
 *                                          delete
 * ***********************************************************************************************/
void world::deleteWorm(const uint id){
    worms.erase(worms.begin()+id);
}

void world::deleteWorm(const Worm& id){
    for(WormContainer::iterator it=worms.begin(); it!=worms.end(); ++it){
        if((*it).getId() == id.getId()){
            std::cout << "Before Erase: " << std::endl;
            std::cout << *this << std::endl;
            for(const WormPart& p: (*it).getWormParts()){
                map.setTheHomeFor(p.getPosX(), p.getPosY(), 1);
            }


            worms.erase(it);
            std::cout << "After Erase: " << std::endl;
            std::cout << *this << std::endl;
            break;
        }
    }
}

void world::deleteLastMove(){
    lastMoveDestiny = WormPart(0,0);
    lastMoveStart = WormPart(0,0);
}

/**************************************************************************************************
                            operatoren
**************************************************************************************************/

world& world::operator =( const world& old){
    Hash = old.Hash;
    worms = old.worms;
    lastMoveStart = old.lastMoveStart;
    lastMoveDestiny = old.lastMoveDestiny;
    CurrentSteps = old.CurrentSteps;
    lastMove = old.lastMove;
    return *this;
}

bool world::operator ==(const world& other)const {
   // if(Hash != other.Hash) return false;
    //if(worms.size() != other.worms.size()) return false;

    for(const Worm& w: worms){
        bool found(false);
        for(const Worm& _w: other.worms){
            if(w==_w){
                found = true;
                break;
            }
        }
        if(!found) return false;
    }
    return true;
}

bool world::operator<(const world& old) const{
    for(uint i=0; i<worms.size(); ++i){
        if(worms[i] < old.worms[i])return true;
        if(old.worms[i] < worms[i]) return false;
    }
    return false;
}

/**************************************************************************************************
                                Ausgabe
**************************************************************************************************/

std::ostream& operator<<(std::ostream& stream, const possibleMoves& Move){
    stream << "WurmNummer: " << Move.wormNumber << " ";
    if(Move.head){
        stream << "Head ";
    } else{
        stream << "Tail ";
    }
    stream << " Direction: ";
    if(Move.direction == Up){
        stream << "up";
    } else if(Move.direction == Down){
        stream << "down";
    } else if(Move.direction == LeftUp){
        stream << "LeftUp";
    } else if(Move.direction == LeftDown){
        stream << "LeftDown";
    } else if(Move.direction == RightUp){
        stream << "RightUp";
    } else if(Move.direction == RightDown){
        stream << "rightDown";
    } else{
        stream << "error";
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const world& w){
    stream <<  "World" << std::endl;
    stream << w.map << std::endl;
    stream << "Würmer" << std::endl;
    for(uint i=0; i<w.worms.size(); ++i){
        stream << "Wurm " << i << ":" <<w.worms[i] << std::endl;
    }
    return stream;
}
