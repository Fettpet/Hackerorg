#include "worm.h"
#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>
#include "World/pos.h"
unsigned int Worm::globalId(1);
/**************************************************************************************************
                                        Konstruktoren
***************************************************************************************************/

/* Standartkonstruktor
    Setzt alles auf 0
*/
Worm::Worm():
    id(0), posHead(0), color(0), length(0)
{}

/**
  Testfunction
 * @brief Worm::Worm
 * @param parts
*/

Worm::Worm(const std::set<WormPart> parts):
    id(0), posHead(0), color(0), length(parts.size())
{
    for(const WormPart& p: parts){
        wormparts.push_back(p);
    }
}

/* Konstruktor
  @param WormString erwartet den String, den es im Internet gibt

*/
Worm::Worm(std::string WormString):
    posHead(0)
{
    color = std::atoi(WormString.substr(WormString.find("team")+4, WormString.find("_")-4).c_str())+2;
    WormString = WormString.substr(6);
    do{
        if(WormString[0] =='_') WormString.erase(0,1);

        std::string buffer;
        if(WormString.find('_') != std::string::npos) {
            buffer = WormString.substr(0,WormString.find('_'));
        } else {
            buffer = WormString;
        }

        uint posX, posY;
        posX = std::atoi(buffer.substr(0, buffer.find(',')).c_str());
        posY = std::atoi(buffer.substr(buffer.find(',')+1).c_str());

        wormparts.push_back(WormPart(posX, posY));
        if(WormString.find('_') != std::string::npos)
            WormString = WormString.substr(WormString.find('_'));
    }while(WormString.find('_') != std::string::npos);
    id = globalId++;
    length = wormparts.size();
}

Worm::Worm(const Worm& old):
    hash(old.hash), id(old.id), posHead(old.posHead), color(old.color), wormparts(old.wormparts), length(old.length)
{}

Worm::~Worm(){

}

/**************************************************************************************************
                                            Getter
**************************************************************************************************/

uint Worm::getId()const{
    return id;
}
/**
ignoring head and Tail
 * @brief Worm::contains
 * @param p
 * @return
*/

bool Worm::contains(const WormPart& p) const{
    for(const auto& wormpart: wormparts){
        if(wormpart == p) return true;
    }

    return false;
}
/* Returnt das erste Element des Wurms, wird zweckmässig als Kopf bezeichnet
  @return Den Kopf des Wurms
*/
WormPart Worm::getHead()const{
    return wormparts[posHead];
}

/* Returnt das letzte Element des Wurms, wird zweckmässig als Schwanz bezeichnet
  @return Den Schwanz des Wurms
*/
WormPart Worm::getTail()const{
    if(posHead == 0)
        return wormparts[length-1];
    else
        return wormparts[posHead-1];
}
/*
    @param Welcher Teil des Wurms returnt werden soll
    @return den Teil des Wurms
*/
const WormPart& Worm::getPart(const uint number) const{
    return wormparts[(number+ posHead) % length];
}
/*
   @return Wie lang der Wurm ist
*/
uint Worm::getLength() const{
    return length;
}

/*
    @return Die Farbe des Wurms
*/
Color Worm::getColor() const{
    return color;
}

/**
    @return Gibt alle Wurmparts zurück als Set
*/
WormPartContainer& Worm::getWormParts(){
    return wormparts;
}
const WormPartContainer& Worm::getWormParts()const{
    return wormparts;
}

WormPart Worm::getNewPos(MoveDircetion direction, WormPart part){
    int x, y;
    bool Upper;
    Upper = part.getPosX() % 2;
    if(direction == Up){
        x = 0;
        y = -1;
    }else
    if(direction == Down){
        x = 0;
        y = 1;
    }else
    if(direction == LeftUp){
        x = -1;
        if(Upper){
            y = 0;
        } else{
            y= -1;
        }
    }else
    if(direction == LeftDown){
        x = -1;
        if(Upper){
            y = 1;
        } else{
            y= 0;
        }
    }else
    if(direction == RightUp){
        x = 1;
        if(Upper){
            y = 0;
        } else{
            y= -1;
        }
    } else{
        x = 1;
        if(Upper){
            y = 1;
        } else{
            y= 0;
        }
    }
    return WormPart(part.getPosX() + x, part.getPosY() +y);
}
/**************************************************************************************************
                                    Setter
**************************************************************************************************/

void Worm::setColor(const Color c){
    this->color = c;
}

/**************************************************************************************************
                                        move
**************************************************************************************************/
/*
  bewegt den Wurm in die angegebene Richtung
  ES FINDET KEIN TEST AUF VALIDIERUNG STATT
  @param direction die Richtung in die er sich bewegen soll
  @param head true falls der sich bewegen soll, false, falls der Schwanz sich bewegen soll
*/
void Worm::move(MoveDircetion direction, bool moveHead){
    int x, y;
    bool Upper;
    if(moveHead){
        Upper = getHead().getPosX() %2;
    }else{
         Upper = getTail().getPosX() %2;
    }
    if(direction == Up){
        x = 0;
        y = -1;
    }else
    if(direction == Down){
        x = 0;
        y = 1;
    }else
    if(direction == LeftUp){
        x = -1;
        if(Upper){
            y = 0;
        } else{
            y= -1;
        }
    }else
    if(direction == LeftDown){
        x = -1;
        if(Upper){
            y = 1;
        } else{
            y= 0;
        }
    }else
    if(direction == RightUp){
        x = 1;
        if(Upper){
            y = 0;
        } else{
            y= -1;
        }
    } else{
        x = 1;
        if(Upper){
            y = 1;
        } else{
            y= 0;
        }
    }
    if(moveHead){
        WormPosIntern posX = getHead().getPosX()+x;
        WormPosIntern posY = getHead().getPosY()+y;
        if(--posHead < 0) {
            posHead = length -1;
        }
        std::cout << "Before: " << *this << std::endl;
        wormparts[posHead] = WormPart(posX, posY);
        std::cout << "After: " << *this << std::endl;
    } else{
        WormPosIntern posX = getTail().getPosX()+x;
        WormPosIntern posY = getTail().getPosY()+y;
        std::cout << "Before:" << *this << std::endl;
        wormparts[posHead] = WormPart(posX, posY);
        posHead++;
        posHead %= length;
        std::cout << "After: " << *this << std::endl;
    }
    calcHash();
}

/**
  bewegt den Wurm von Start nach Destiny
 * @brief Worm::move
 * @param start
 * @param destiny
 */
bool Worm::move(const WormPart& start, const WormPart& destiny){
    int x = start.getX() - destiny.getX();
    int y = start.getY() - destiny.getY();
    bool Upper(false);
    bool moveHead(false);
    if(x > 1) return false;
    if(y > 1) return false;
    if(y < -1) return false;
    if(x < -1) return false;
    if(getHead() == start){
        moveHead = true;
        Upper = getHead().getPosX() %2;
    } else if(getTail() == start){
        moveHead = false;
        Upper = getTail().getPosX() %2;
    } else{
        return false;
    }
    if(((x==0) && (y==-1))
        || ((x==0) && (y==1))
        || ((x==-1) && ((Upper && ((y==0) || (y==1))) || ((!Upper) && ((y==-1) || (y==0)))))
        || ((x==1)  && ((Upper && ((y==0) || (y==1))) || ((!Upper) && ((y==-1) || (y==0)))))){
        if(moveHead){
            posHead++;
            posHead %= length;
            wormparts[posHead] = destiny;
        } else{
            wormparts[posHead] = destiny;
            if(--posHead < 0) {
                posHead = length-1;
            }
        }
        calcHash();
        return true;
    }
    return false;
}

void Worm::move(const WormPart& destiny, const bool& moveHead){
    if(moveHead){
        if(--posHead < 0) {
            posHead = length -1;
        }

        wormparts[posHead] = destiny;
    } else{
        wormparts[posHead] = destiny;
        posHead++;
        posHead %= length;
    }
    calcHash();
}

/**************************************************************************************************
 *                          Hash
 * ***********************************************************************************************/
void Worm::calcHash(){
    hash = 0;
    for(uint i=0; i<length; ++i){
        hash ^= ((getPart(i).getPosX() | (getPart(i).getPosY() << 16)) << i);
    }
 //   std::cout << hash << std::endl;

}

/**************************************************************************************************
                            Operatoren
******************+*******************************************************************************/
Worm& Worm::operator =(const Worm& old){
    id = old.id;
    color = old.color;
    posHead = old.posHead;
    wormparts = old.wormparts;
    length = old.length;
    return *this;
}

bool Worm::operator==(const Worm& other) const{
    if(length != other.length) return false;
    if(color != other.color) return false;
    // Testen von Kopf zu  Kopf
    for(uint i=0; i< length; ++i){
        if((wormparts[i] != other.wormparts[i]) && (wormparts[i] != other.wormparts[length-1-i])){
            return false;
        }
    }
    return true;
}

bool Worm::operator<(const Worm& other) const{
    if(hash < other.hash) return true;
    if(hash > other.hash) return false;
    for(uint i=0; i<length; ++i){
        if(getPart(i) < other.getPart(i)) return true;
        if(other.getPart(i) < getPart(i) ) return false;
    }
    return false;
}

std::ostream& operator<<(std::ostream& str, const Worm& t){
    str << "ID: " << t.getId() << "Farbe: " << t.getColor() << " Länge: " << t.getLength();
    str << "Kopf: "<< t.getHead();
    str << "Schwanz: " << t.getTail();
    str << "Körper:";
    for(uint i=0; i< t.getLength(); ++i){
        str << t.getPart(i) << " ";
    }
    return str;
}
