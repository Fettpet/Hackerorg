#include "easywormheuristics.h"
#include <cmath>
using namespace Heuristics;
/**************************************************************************************************
 *                                      Constructor
 * ***********************************************************************************************/
EasyWormHeuristics::EasyWormHeuristics(world& _w, uint _wormNumber):
    AStarHeuristics(_w), wormNumber(_wormNumber)
{
    std::cout << "Hello World";
    findHomeFields();
    createDistanceGraph();
    createInverseDistances(_w,w->getWorm( _wormNumber));
    createCost(_w, w->getWorm(_wormNumber));
}

/**************************************************************************************************
 *                                  Heuristics
 * ***********************************************************************************************/


uint EasyWormHeuristics::operator() (world* _w){
    w = _w;
    uint result = getDistancePlusCountOfFirstOutsider(wormNumber);

    if(result == 0) return 0;
    return result  + getBlockedFields();
}

uint EasyWormHeuristics::getMaxDist(){
    uint result(0);
    for(const Worm& wurm: w->getWorms()){
        //if(wurm.getId() == wormNumber.getId()) continue;
        uint buffer(0);
        register uint cut = wurm.getLength()/2;
        for(uint i=0; i<cut; ++i){
            register WormPart part(wurm.getPart(i));
            buffer = std::max(buffer, distancesInverse[part.getPosX()][part.getPosX()] + i);
        }
        for(uint i=cut; i<wurm.getLength(); ++i){
            register WormPart part(wurm.getPart(i));
            buffer = std::max(buffer, distancesInverse[part.getPosX()][part.getPosX()] + wurm.getLength()-i);
        }
        result += buffer;
    }
    return result;
}

uint EasyWormHeuristics::getBlockedFields(){
//init
    Worm wurm;
    uint wormCount = w->getWormCount();
    std::vector<uint> valueForResultHead, valueForResultTail;
    uint resultHead(0), resultTail(0);

    for(uint i=0; i< wormCount; ++i){
        valueForResultTail.push_back(0);
        valueForResultHead.push_back(0);
    }
// Homefields
    for(const auto& p: homeFields){

        for(uint worm=0; worm < wormCount; ++worm){
            bool found(false);
            if (worm == wormNumber) continue;
            wurm = w->getWorm(worm);
            for(uint pos=0; pos < wurm.getLength(); ++pos){
                if(wurm.getPart(pos) == p){
                    if(valueForResultHead[worm] <= pos){
                       valueForResultHead[worm] = pos + 1;
                    }
                    found = true;
                }
                if(wurm.getPart(wurm.getLength() - pos - 1) == p){
                    if(valueForResultHead[worm] <= wurm.getLength() - pos - 1){
                       valueForResultHead[worm] = wurm.getLength() - pos;
                    }
                    found = true;
                }
            }
            if(found) break;
        }
    }
// PathFields
    findHomePath(true);
    for(const auto& p: homePath){
        for(uint worm=0; worm< wormCount; ++worm){
            bool found(false);
            if (worm == wormNumber) continue;
            wurm = w->getWorm(worm);
            for(uint pos=0; pos < wurm.getLength(); ++pos){
                if(wurm.getPart(pos) == p){
                    if(valueForResultHead[worm] <= pos){
                       valueForResultHead[worm] = pos + 1;
                    }
                    found = true;
                    break;
                }
            }
            if(found) break;
        }
    }
    // PathFields
    findHomePath(false);
    for(const auto& p: homePath){
        for(uint worm=0; worm < wormCount; ++worm){
            bool found(false);
            if (worm == wormNumber) continue;
            wurm = w->getWorm(worm);
            for(uint pos=0; pos < wurm.getLength(); ++pos){
                if(wurm.getPart(pos) == p){
                    if(valueForResultTail[worm] <= pos){
                        valueForResultTail[worm] = pos + 1;
                    }
                    found = true;
                    break;
                }
            }
            if(found) break;
        }
    }
// Find the minimum
    for(uint value: valueForResultHead){
        resultHead += value;
    }
    for(uint value: valueForResultTail){
        resultTail += value;
    }
    return std::min(resultHead, resultTail);
}

uint EasyWormHeuristics::getBlockedHomeFields(){
    uint result(0);
    for(const auto& p: homeFields){
        for(const auto& Wurm: w->getWorms()){
            if(w->getWormMap().getTheHomeFor(p.getX(), p.getY()) == Wurm.getColor()){
                continue;
            }
            bool found(false);
            for(uint pos=0; pos<Wurm.getLength(); ++pos){
        // Test the Head
                if(Wurm.getPart(pos) == p){
                    result += pos+1;
                    found = true;
                    break;
                }
        // test the tail
                if(Wurm.getPart(Wurm.getLength() - 1 - pos) == p){
                    result +=pos+1;
                    found = true;
                    break;
                }
            }
            if(found)break;
        }
    }
    return result;
}

uint EasyWormHeuristics::getBlockedPathFields(){
    std::deque<int> returnValuesForWormsHead, returnValuesForWormsTail;
    for(uint i=0; i< w->getWormCount(); ++i){
        returnValuesForWormsHead.push_back(0);
        returnValuesForWormsTail.push_back(0);
    }
    findHomePath(true);
    uint resultHead(0);
    for(const auto& p: homePath){
        for(const Worm& wurm: w->getWorms()){
//            if(wurm.getId() == wormNumber.getId()) continue;
            if(wurm.contains(p)){
                resultHead += 1;
                break;
            }
        }
    }
    findHomePath(false);
    uint resultTail(0);
    for(const auto& p: homePath){
        for(const Worm& wurm: w->getWorms()){
//            if(wurm.getId() == wormNumber.getId()) continue;
            if(wurm.contains(p)){
                resultTail += 1;
                break;
            }
        }
    }

    return std::min(resultHead, resultTail);
}

/**************************************************************************************************
 *                                  init functions
 * ***********************************************************************************************/

void EasyWormHeuristics::findHomeFields(){
    homeFields.clear();
    const uint wormColor(w->getWorm(wormNumber).getColor());
    for(uint x=0; x<w->getSizeX(); ++x){
        for(uint y=0; y<w->getSizeY(); ++y){
            if(w->getWormMap().getTheHomeFor(x,y) == wormColor){
                homeFields.push_back(WormPart(x,y));
            }
        }
    }
}

void EasyWormHeuristics::findHomePath(bool headUsing){
    homePath.clear();
    uint wormColor(w->getWorm(wormNumber).getColor());
    WormPart start;
    if(headUsing){
        start = w->getWorm(wormNumber).getHead();
    } else{
        start = w->getWorm(wormNumber).getTail();
    }

    homePath.push_back(start);
    WormPart cur(start);

    while(distances[wormColor][cur.getX()][cur.getY()] != 0){
        cur = getMinNeighbor(cur, wormColor);
        homePath.push_back(cur);
    };
    homePath.pop_front();
}


void EasyWormHeuristics::createCost(world& _w, Worm _worm){
    costgetBlockedFields.clear();
// Generate a zero field
    for(uint sizeX=0; sizeX < _w.getSizeX(); ++sizeX){
        std::vector< std::vector< std::vector< uint > > > sizeXContainer;
        for(uint sizeY=0; sizeY < _w.getSizeY(); ++sizeY){
            std::vector< std::vector< uint > >  sizeYContainer;
            for(const Worm& worm: _w.getWorms()){
// First case Worm = dest Worm
                if(worm.getId() == _worm.getId()){
                    std::vector< uint >  WormContainer;
                    for(uint wP=0; wP < worm.getLength(); ++wP){
                        WormContainer.push_back(0);
                    }
                    sizeYContainer.push_back(WormContainer);
                }
// second case another worm
                for(uint wP=0; wP < worm.getLength(); ++wP){
                    std::vector< uint >  WormContainer;
                    if(_w.getWormMap().getTheHomeFor(sizeX, sizeY) == _worm.getColor()){
                        WormContainer.push_back(std::min(wP+1, worm.getLength()-wP));
                    }else {
                        WormContainer.push_back(distancesInverse[sizeX][sizeY]);
                    }
                }
            }
        }
    }
}

void EasyWormHeuristics::createInverseDistances(world& _w, Worm _wormNumber){

// init the Color distances with 0
    int max(0);
    for(uint x=0; x<w->getSizeX(); ++x){
        std::vector<uint> buffer;
        for(uint y=0; y<w->getSizeY(); ++y){
            buffer.push_back(0);
            if(distances[_wormNumber.getColor()][x][y] > max){
                max = distances[_wormNumber.getColor()][x][y];
            }
        }
        distancesInverse.push_back(buffer);
    }
    std::cout << "MAX:" << max << std::endl;
    for(uint x=0; x<w->getSizeX(); ++x){
        for(uint y=0; y<w->getSizeY(); ++y){
            distancesInverse[x][y] = max - distances[_wormNumber.getColor()][x][y];
        }
    }
    for(uint x=0; x<w->getSizeY(); ++x){
        for(uint y=0; y<w->getSizeX(); ++y){
            std::cout << distancesInverse[y][x];
        }
        std::cout << std::endl;
    }
}

/**************************************************************************************************
 *                                          helpfunction
 * ***********************************************************************************************/

WormPart EasyWormHeuristics::getMinNeighbor(const WormPart& p, const Color color){

    WormPart result;
    int min(99999);
    for(const auto& p2: *(w->getNeighborFields(p))){
        if( distances[color][p2.getX()][p2.getY()] == -1) continue;
        if(min > distances[color][p2.getX()][p2.getY()]){
            min = distances[color][p2.getX()][p2.getY()];
            result = p2;
        }
    }

    return result;
}


/**************************************************************************************************
 *
 * ***********************************************************************************************/
HomePathContainer& EasyWormHeuristics::getHomePath(bool head){//
    findHomePath(head);
    return homePath;
}
//*/
