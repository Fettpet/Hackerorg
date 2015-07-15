#include "getwormtofield.h"
using namespace Heuristics;

/**************************************************************************************************
 *                                  Constructor
 * ***********************************************************************************************/
GetWormToField::GetWormToField(world& _w, const WormPart& dest, const Worm& wu):
    GetOneFieldFree(_w, dest), WormDest(wu)
{
    w = &_w;
    calcWormThatBlocksField();
    createDistanceGraphForOne();
}


/**************************************************************************************************
 *                                          Operator
 * ***********************************************************************************************/
uint GetWormToField::operator ()(world* _w){
    w = _w;

    return getMovesToFreeAField(toFree, WormId) + getCostToField(WormDest);
}

/**************************************************************************************************
 *                                          Heuristics
 * ***********************************************************************************************/
uint GetWormToField::getCostToField(const Worm& _wormId){
    uint costHead, costTail;
    costHead = distances[0][_wormId.getHead().getPosX()][_wormId.getHead().getPosY()];
    costTail = distances[0][_wormId.getTail().getPosX()][_wormId.getTail().getPosY()];
    return std::min(costHead, costTail);
}

/**************************************************************************************************
 *                                          Helpfunctions
 * ***********************************************************************************************/

void GetWormToField::createDistanceGraphForOne(){
    distances.clear();
    std::vector < std::vector < int > > distBuf;
    std::deque<WormPart> founded;
// init the Color distances with 0
    for(uint x=0; x<w->getSizeX(); ++x){
        std::vector<int> buffer;
        for(uint y=0; y<w->getSizeY(); ++y){
             buffer.push_back(-1);
        }
        distBuf.push_back(buffer);
    }
    NeighborContainer* b = w->getNeighborFields(toFree);
    for(uint i=0; i<b->size(); ++i){
        founded.push_back(b->operator [](i));
    }
    distBuf[toFree.getX()][toFree.getY()] = 0;
    // Set the distances

    while(!founded.empty()){
        WormPart cur = founded[0];
        founded.pop_front();
        if(w->getWormMap().getTheHomeFor(cur.getX(), cur.getY()) == 1) continue;
        if(distBuf[cur.getX()][cur.getY()] != -1) continue;
        NeighborContainer* neighbor;
        int min(__INT32_MAX__);
        neighbor = w->getNeighborFields(cur);

        for(const auto& p: *neighbor){
            if(distBuf[p.getX()][p.getY()] != -1){
                if(distBuf[p.getX()][p.getY()] < min)
                    min = distBuf[p.getX()][p.getY()];
            } else{
                founded.push_back(p);
            }
        }
        distBuf[cur.getX()][cur.getY()] = min + 1;
    }

    distances.push_back(distBuf);

}
