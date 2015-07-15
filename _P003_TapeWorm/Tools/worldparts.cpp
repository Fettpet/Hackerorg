#include "worldparts.h"

typedef std::vector<bool> visitedMapInnerContainer;
typedef std::vector< visitedMapInnerContainer> visitedMap;

/**************************************************************************************************
 *                                      create visited Map
 * ***********************************************************************************************/
/**
 * @brief createVisitedMap
 * @param w: The Current World: needed for size and blocked fields
 * @param map: The Result Map
 * @param worms: Worms that block fields
 */
void createVisitedMap(const world& w, visitedMap& map, const WormContainer& worms){
    map.clear();
    for(uint x=0; x<w.getSizeX(); ++x){
        visitedMapInnerContainer innerContainer;
        for(uint y=0; y<w.getSizeY(); ++y){
            if(w.getWormMap().getTheHomeFor(x,y) == 1){
                innerContainer.push_back(true);
            } else{
                bool found(false);
                for(const Worm& wurm: worms){
                    if(wurm.contains(WormPart(x,y))) {
                        innerContainer.push_back(true);
                        found = true;
                        break;
                    }
                }
                if(!found)
                    innerContainer.push_back(false);
            }
        }
        map.push_back(innerContainer);
    }
}

/**************************************************************************************************
 *                                  getLength
 * ***********************************************************************************************/
uint getWorldPartSize(const world& w, visitedMap& map, const WormPart& p){
    NeighborContainer mustVisited;
    mustVisited.push_back(p);
    uint result(0);

    while(!mustVisited.empty()){
        WormPart cur = mustVisited[mustVisited.size()-1];
        mustVisited.pop_back();
        if((w.getWormMap().getTheHomeFor(cur.getX(), cur.getY()) != 1) && (!map[cur.getX()][cur.getY()])){
            result++;
            map[cur.getX()][cur.getY()] = true;
            NeighborContainer* buffer = w.getNeighborFields(cur);
            for(auto p: *buffer){
                mustVisited.push_back(p);
            }
        }
    }

    return result;
}

uint getWorldPartSize(const world &w, visitedMap& map, const Worm &worms, const WormPart& p){
    return getWorldPartSize(w, map, p);
}

uint getWorldPartSize(const world &w, visitedMap& map, const uint &worms, const WormPart& p){
    return getWorldPartSize(w, map, p);
}

uint Tools::getWorldPartSize(const world& w, const WormContainer& worms, const WormPart& p){
    visitedMap map;
    createVisitedMap(w, map, worms);
    return getWorldPartSize(w, map, p);
}

uint Tools::getWorldPartSize(const world &w, const Worm &worms, const WormPart& p){
    visitedMap map;
    WormContainer wCon;
    wCon.push_back(worms);
    createVisitedMap(w, map, wCon);

    return getWorldPartSize(w, map, p);

}

uint Tools::getWorldPartSize(const world &w, const uint &worms, const WormPart& p){
    visitedMap map;
    WormContainer wCon;
    wCon.push_back(w.getWorm(worms));
    createVisitedMap(w, map, wCon);

    return getWorldPartSize(w, map, p);
}


/**+***********************************************************************************************
 *                                  countWorldParts
 * ***********************************************************************************************/
/**
 * @brief countWorldParts: This function count how many parts are in the World
 * @param w: The World
 * @param blockedWorms: The Worms, that block fields
 * @return The Count of seperated Parts in the Map
*/
uint Tools::countWorldParts(const world& w, const WormContainer& blockedWorms){
    visitedMap map;
    uint result(0);
    createVisitedMap(w, map, blockedWorms);

    for(uint x=0; x< w.getSizeX(); ++x){
        for(uint y=0; y < w.getSizeY(); ++y){
            if(map[x][y] == false){
                result++;
                getWorldPartSize(w,map, WormPart(x,y));
            }
        }
    }
    return result;
}
uint Tools::countWorldParts(const world& w, const Worm& blockedWorms){
    WormContainer con;
    con.push_back(blockedWorms);
    return countWorldParts(w, con);
}

uint Tools::countWorldParts(const world &w, const uint &blockedWorm){
    WormContainer con;
    con.push_back(w.getWorm(blockedWorm));
    return countWorldParts(w, con);
}
