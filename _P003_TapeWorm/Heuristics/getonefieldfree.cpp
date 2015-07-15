#include "getonefieldfree.h"
using namespace Heuristics;
/********************+++++++++++++++++*************************************************************
 *                                  Constructor
 * ***********************************************************************************************/
GetOneFieldFree::GetOneFieldFree(world w, WormPart free):
    AStarHeuristics(w), toFree(free)
{

}



/**************************************************************************************************
 *                                  Helpfunctions
 * ***********************************************************************************************/

void GetOneFieldFree::calcWormThatBlocksField(){
    for(const auto& wurm: w->getWorms()){
        if(wurm.contains(toFree)){
            WormId = wurm;
            break;
        }
    }
}

/**************************************************************************************************
 *                                  Operatoren
 * ***********************************************************************************************/
uint GetOneFieldFree:: operator ()(world* _w){
    w = _w;
    return getMovesToFreeAField(toFree, WormId);
}

/**************************************************************************************************
 *                                  Heuristic
 * ***********************************************************************************************/
uint GetOneFieldFree::getMovesToFreeAField(const WormPart& p, const Worm& wu){
    for(size_t pos=0; pos< wu.getLength(); ++pos){
// Test the Head
        if(wu.getPart(pos) == p){
            return pos;
        }
// test the tail
        if(wu.getPart(wu.getLength() - 1 - pos) == p){
            return pos;
        }
    }
    return 0;
}
