#include "astarcalculation.h"
#include "Heuristics/astarheuristics.h"
#include <boost/container/vector.hpp>
#include <fstream>
#include "Exceptions/onewormathome.h"
#define MEMORYBYTEUSAGE 40000000
AStarCalculation::AStarCalculation()
{
}


/**************************************************************************************************
 *                                          AStar Calculation
 * ***********************************************************************************************
std::string AStarCalculation::getPath(world& w, Heuristics::AStarHeuristics& hbest){
    init(w, hbest);
    uint i(0);
    worldRep best;
    uint count(0);
    bool exTest(false);
    std::stringstream sol;
    do{
        count++;
        ++i;
        uint costBest;
        best = getBestWorld(costBest);
        if(worldList.size() > (MEMORYBYTEUSAGE /sizeof(w))){
            //w = worldList[best.worldNumber];
            NoWayFound exep;
            throw exep;
        }

        try{
            if(hbest(&worldList[best.worldNumber]) == 0){
                w = worldList[best.worldNumber];
                std::cout << "Heur is 0";
                std::cout << worldList[best.worldNumber];
                break;
            }
        } catch(const OneWormAtHome& ex){
            std::deque<int> WorldPositions;
            uint cur(0);
            for(uint i=0; i<worldList.size(); ++i){
                if(worldList[i] == ex.w){
                    cur = i;
                    break;
                }
            }
            WorldPositions.push_back(cur);
            sol << getSolution(cur);
            std::cout << "Current c: " << cur;
            std::cout << "Current Solution: "<< sol.str() << std::endl;
            w = ex.w;
            w.deleteWorm(ex.wurm);
            init(w, hbest);
            continue;
        }

        PossibleMoveContainer moves;
        worldList[best.worldNumber].getAllPosibleMoves(moves);
        for(const possibleMoves& m: moves){
            uint WorldNum = (worldList[best.worldNumber].createNewWorldByMovingWorm(m, worldList));
            parents.push_back(best.worldNumber);
            worldRep n;
            n.worldNumber = WorldNum;
            n.w = &(worldList[WorldNum]);
            uint cost;
            try{
                cost =hbest(n.w);
            }catch(const OneWormAtHome& ex){
                uint cur(0);
                for(uint i=0; i<worldList.size(); ++i){
                    if(worldList[i] == ex.w){
                        cur = i;
                        break;
                    }
                }
                std::cout << "Current c: " << cur;
                sol << getSolution(cur);
                   std::cout << "Current Solution: "<< sol.str() << std::endl;
                w = ex.w;
                w.deleteWorm(ex.wurm);
                init(w, hbest);
                exTest = true;
                break;
            }

            for(uint i=openCosts.size(); i<=cost; ++i){
                InnerListContainer buffer;
                openCosts.push_back(buffer);
                closeList.push_back(buffer);
            }
            if(closeList[cost].find(n) == closeList[cost].end()){
                openCosts[cost].insert(n);
                closeList[cost].insert(n);
            }
        }
        if(exTest){
            exTest = false;
        } else {
            openCosts[costBest].erase(openCosts[costBest].find(best));
        }
    }while(true);
    uint cur(best.worldNumber);
    sol << getSolution(cur);
    std::cout << sol.str();
    return sol.str();
}

/**************************************************************************************************
 *                                                  Getter
 * ************************************************************************************************
uint AStarCalculation::getCountOpenList()const{
    uint result(0);
    for(uint i=0; i<openCosts.size(); ++i){
        result += openCosts[i].size();
    }
    return result;
}

uint AStarCalculation::getCountCloseList()const{
    uint result(0);
    for(uint i=0; i<closeList.size(); ++i){
        result += closeList[i].size();
    }
    return result;
}

/**************************************************************************************************
 *                                                      helpfunctions
 * ***********************************************************************************************
/**
    Clear old things and start
 * @brief init
 * @param w
*
void AStarCalculation::init(world& w, Heuristics::AStarHeuristics& heuristics){
    std::cout << "World in init" << std::endl;
    std::cout << w << std::endl;
    w.deleteLastMove();
    openCosts.clear();
    closeList.clear();
    worldList.clear();
    parents.clear();

    bool exeption(false);
    uint curCost(99);
    do{
        exeption = false;
        try{
            curCost = heuristics(&w);
        } catch(const OneWormAtHome& t){
            w.deleteWorm(t.wurm);
            exeption = true;
        }
    }while(exeption);
    for(uint i=0; i<= curCost; ++i){
        InnerListContainer buffer;
        openCosts.push_back(buffer);
        closeList.push_back(buffer);
    }
    worldList.push_back(w);
    worldRep rep;
    rep.w = &(worldList[0]);
    rep.worldNumber = 0;
    parents.push_back(0);
    openCosts[curCost].insert(rep);
    for(const world& wCur: worldToList){
        worldList.push_back(wCur);
        worldRep rep;
        rep.w = &(worldList[worldList.size()-1]);
        rep.worldNumber = worldList.size()-1;
        parents.push_back(worldList.size()-1);
        openCosts[curCost].insert(rep);
    }
    worldToList.clear();
}

/**
 * @brief AStarCalculation::getBestWorld
 * @return
*
const worldRep AStarCalculation::getBestWorld(uint& costBest){
    for(uint i=0; i<openCosts.size(); ++i){
        if(openCosts[i].size() != 0){
            costBest = i;
            return (*(openCosts[i].begin()));
        }
    }
    NoWayFound w;
    throw w;
    return worldRep();
}

std::string AStarCalculation::getSolution(uint wormNumber){
    std::stringstream sol;
    std::deque<int> WorldPositions;
    WorldPositions.push_back(wormNumber);
    uint cur(wormNumber);
    do{
        WorldPositions.push_front(parents[cur]);
        if(parents[cur] == cur) break;
        cur  = parents[cur];

    }while(true);
    for(uint i=1; i< WorldPositions.size(); ++i){
        sol << worldList[WorldPositions[i]].getPath();
    }
    std::cout << "SolutionString: " << sol.str() << std::endl;
    return sol.str();
}

/**************************************************************************************************
 *
 * ***********************************************************************************************/
void AStarCalculation::addToOpenList(world& w){
    worldToList.push_back(w);
}
