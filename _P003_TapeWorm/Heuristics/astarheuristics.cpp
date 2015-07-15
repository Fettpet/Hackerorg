#include "astarheuristics.h"
#include <deque>
#include "Praprozessor/easywormstohome.h"
#include "World/pos.h"
#include "Exceptions/onewormathome.h"
#include <Tools/worldparts.h>
#define USESETTING 0
using namespace Heuristics;

std::vector< std::vector< std::vector < int > > > AStarHeuristics::distances;

/**************************************************************************************************
 *                                      Constructor
 * ***********************************************************************************************/
AStarHeuristics::AStarHeuristics(world &_w):
    w(&_w)
{
}

/**************************************************************************************************
 *                                          Wormparts are not at Home
 * unneaded because getWormPartsAreNotAtHome <= getTheCountOfTheFirstOutsider
 * ***********************************************************************************************/
/**
calculate how many worm parts are not at home. This is the minimum number of steps to come home.
This is for all Worms
 * @brief getWormPartsAreNotAtHome
 * @param w: The world
 * @return The number of parts they are noch at home
 */
uint AStarHeuristics::getWormPartsAreNotAtHome(){
    return w->getCountWormPartsAtHome();
}

/**
 * @brief AStarHeuristics::getWormPartsAreNotAtHome
 * @param w: The World
 * @param wurm: The Wurm
 * @return
 */
uint AStarHeuristics::getWormPartsAreNotAtHome(const Worm& wurm){
    uint result(0);
    register const uint wormColor(wurm.getColor());
    for(const auto& p: wurm.getWormParts()){
        uint mapColor(w->getWormMap().getTheHomeFor(p.getX(), p.getY()));
        result += (mapColor == wormColor);
    }
    return result;
}

/**************************************************************************************************
 *                          Count of first out
 * ***********************************************************************************************/


uint AStarHeuristics::getTheCountOfTheFirstOutsider(const Worm& wurm){
    int posHead(0), posTail(0);
    const register uint wormColor(wurm.getColor());
    for(uint pos=0; pos<wurm.getLength(); ++pos){
// Test the Head
        const WormPart HeadPart(wurm.getPart(pos));
        if(w->getWormMap().getTheHomeFor(HeadPart.getPosX(), HeadPart.getPosY()) != wormColor){
            posHead = pos;
        }
// test the tail
        const WormPart tailPart(wurm.getPart(wurm.getLength()-1-pos));
        if(w->getWormMap().getTheHomeFor(tailPart.getPosX(), tailPart.getPosY()) != wormColor){
            posTail = pos;
        }
    }
    return std::min(posHead, posTail);
}
uint AStarHeuristics::getTheCountOfTheFirstOutsider(){
    uint result(0);
    for(const auto& wurm: w->getWorms()){
        result += getTheCountOfTheFirstOutsider(wurm);
    }
    return result;
}

/**************************************************************************************************
 *
 * ***********************************************************************************************/
/**
 * @brief AStarHeuristics::getDistancePlusCountOfFirstOutsider
 * @param wurm: Der Wurm
 * @return: Der Heuristic Wert
*/
uint AStarHeuristics::getDistancePlusCountOfFirstOutsider(const Worm& wur){
   Worm wurm;
   for(const Worm& wo: w->getWorms()){
        if(wo.getId() == wur.getId()){
            wurm = wo;
            break;
        }
    }
   // std::cout << "Worm 1: " << wur << std::endl;
   // std::cout << "Worm 2: " << wurm << std::endl;
    //*/
    uint posHead(0), posTail(0);
    const register uint wormColor(wurm.getColor());
    const uint headCost(distances[wurm.getColor()][wurm.getHead().getPosX()][wurm.getHead().getPosY()]);
    const uint tailCost(distances[wurm.getColor()][wurm.getTail().getPosX()][wurm.getTail().getPosY()]);
    for(uint pos=0; pos<wurm.getLength(); ++pos){
// Test the Head
        const WormPart HeadPart(wurm.getPart(pos));
        if(w->getWormMap().getTheHomeFor(HeadPart.getPosX(), HeadPart.getPosY()) != wormColor){
            posHead = pos+ (tailCost > 0 ? tailCost : 1);
        }
// test the tail
        const WormPart tailPart(wurm.getPart(wurm.getLength()-1-pos));
        if(w->getWormMap().getTheHomeFor(tailPart.getPosX(), tailPart.getPosY()) != wormColor){
            posTail = pos + (headCost > 0 ? headCost : 1);
        }
    }
    return std::min(posHead, posTail);
}

uint AStarHeuristics::getDistancePlusCountOfFirstOutsider(uint wormNumber){
    return getDistancePlusCountOfFirstOutsider(w->getWorm(wormNumber));
}

uint AStarHeuristics::getDistancePlusCountOfFirstOutsider(){
    uint result(0);
    for(const auto& wurm: w->getWorms()){
        uint buffer(getDistancePlusCountOfFirstOutsider(wurm));
        result += buffer;
    }
    return result;
}

/**************************************************************************************************
 *                              dijkstra shortest paths heuristic
 * ***********************************************************************************************/
void AStarHeuristics::createDistanceGraph(){
    uint countColors(w->getWormMap().getColorCount());
    distances.clear();
    for(uint curColor=0; curColor<=countColors; ++curColor){

        std::vector < std::vector < int > > distBuf;
        NeighborContainer toLook;
// init the Color distances with 0
        for(uint x=0; x<w->getSizeX(); ++x){


            std::vector<int> buffer;
            for(uint y=0; y<w->getSizeY(); ++y){
                if(w->getWormMap().getTheHomeFor(x, y) == curColor){
                    NeighborContainer *founded = w->getNeighborFields(x, y);
                    for(uint i=0; i<founded->size(); ++i){
                        toLook.push_back( (*founded)[i]);
                    }
                    toLook.push_back(WormPart(x, y));
                    buffer.push_back(0);
                } else {
                    buffer.push_back(-1);
                }
            }
            distBuf.push_back(buffer);
        }
// Set the distances

        while(!toLook.empty()){
            WormPart cur = toLook[0];
            toLook.erase(toLook.begin());
            if(w->getWormMap().getTheHomeFor(cur.getX(), cur.getY()) == 1) continue;
            if(distBuf[cur.getX()][cur.getY()] != -1) continue;
            NeighborContainer *neighbor;
            int min(__INT32_MAX__);
            neighbor = w->getNeighborFields(cur);
            for(const auto& p: (*neighbor)){
                if(distBuf[p.getX()][p.getY()] != -1){ ;
                    if(distBuf[p.getX()][p.getY()] < min)
                        min = distBuf[p.getX()][p.getY()];
                } else{
                    toLook.push_back(p);
                }
            }
            distBuf[cur.getX()][cur.getY()] = min + 1;
        }

        distances.push_back(distBuf);
    }
}

void AStarHeuristics::printDistanceGraph(){
    std::cout << "Color Count: " << distances.size() << std::endl;
    for(uint color=0; color<distances.size(); ++color){
        std::cout << "Color: " << color << std::endl;
        for(uint x=0; x < distances[color][0].size(); ++x){
            for(uint y=0; y < distances[color].size(); ++y){
                std::cout << distances[color][y][x] << " ";
            }
            std::cout << std::endl;
        }
    }
}

/**************************************************************************************************
 *                              help functions
 * ***********************************************************************************************/
bool AStarHeuristics::canSetToUnused(const Worm& wurm){
    WormMap wormMapBuffer(w->getWormMap());
    WormPartContainer conNothing;
    for(uint x=0; x<w->getWormMap().getSizeX(); ++x){
        for(uint y=0; y<w->getWormMap().getSizeY(); ++y){
            if(w->getWormMap().getTheHomeFor(x,y) != wurm.getColor()){
                conNothing.push_back(WormPart(x,y));
                w->getWormMap().setTheHomeFor(x, y, 1);
            }
        }
    }
    WormContainer conBuf;
    world wo(*w);
    uint count = Tools::countWorldParts(wo, conBuf);
    for(const WormPart& p: wurm.getWormParts()){
        wo.getWormMap().setTheHomeFor(p.getPosX(), p.getPosY(), 1);
    }
    bool result(count == Tools::countWorldParts(wo, conBuf));
    w->getWormMap() = wormMapBuffer;
    return result;

}
