#include "easywormstohome.h"
#include "Heuristics/easywormheuristics.h"
#include "Heuristics/getonefieldfree.h"
#include "Calculation/astarcalculation.h"
#include "Heuristics/getwormtofield.h"
#include "Tools/worldparts.h"
#include "Calculation/depthfirst.h"
#include "Calculation/dfs_astar.h"

std::string EasyWormsToHome::getEasyWormsToHome(world& w, uint& countOpen, uint& countClose){
    std::stringstream result;
    SeperatedWormContainer seperWorms;
    seperateWorms(w, seperWorms);

    while(getWurm(seperWorms) != -1){
        Worm wormId(w.getWorm(getWurm(seperWorms)));
        WormPositionContainer con;
        findHomeForWorms(w.getWormMap(), wormId.getColor(), con);
        if(canBringTheWormHome(w, con, wormId)){
            std::cout << "This Worm can be bring home: " << wormId << std::endl;
            uint cO(0), cC(0);
            result << bringTheWormHomeDeep(w, wormId);


            //w.getTheWorldAsImage().save("/home/fettpet/image.png");
            countOpen += cO;
            countClose += cC;
         //   w.deleteWorm(wormId);
         //   for(const auto& p: con){
         //       w.getWormMap().setTheHomeFor(p.getX(), p.getY(), 1);
         //   }
            std::cout << "done" << std::endl;
        //    seperWorms.clear();
        //    seperateWorms(w, seperWorms);
        }
        for(uint i=0; i< seperWorms.size(); ++i){
            if(seperWorms[i].empty()) continue;
            if(seperWorms[i][0] == getWurm(seperWorms)){
                seperWorms[i].clear();
                break;
            }
        }
    }

    return result.str();
}

/**************************************************************************************************
 *                                      Worm finding functions
 * ***********************************************************************************************/
void EasyWormsToHome::seperateWorms(world& w, SeperatedWormContainer& result){
    const uint colorCount(w.getWormMap().getColorCount());
    for(uint curColor=result.size(); curColor<=colorCount; ++curColor){
        result.push_back( std::vector< uint>());
    }
    for(uint i=0; i<w.getWormCount(); ++i){
        result[w.getWorm(i).getColor()].push_back(i);
    }
}


int EasyWormsToHome::getWurm(const SeperatedWormContainer& sep){
    for(uint i=0; i<sep.size(); ++i){
        if(sep[i].size() == 1){
            return sep[i][0];
        }
    }
    return -1;
}


void EasyWormsToHome::findHomeForWorms(const WormMap& w, const uint color, WormPositionContainer& container){
    for(uint x=0; x<w.getSizeX(); ++x){
        for(uint y=0; y<w.getSizeY(); ++y){
            if(w.getTheHomeFor(x,y) == color){
                container.insert(WormPart(x,y));
            }
        }
    }
}

bool EasyWormsToHome::canBringTheWormHome(const world& w, const WormPositionContainer& container, const Worm& wormId){
    std::vector< NeighborContainer* > neigCont;
    world worldBuffer(w);
    uint numberOfParts(Tools::countWorldParts(w, WormContainer()));
    uint wurmColor = w.getWormMap().getTheHomeFor((*(container.begin())).getX(), (*(container.begin())).getY());
// Die ganzen Nachbarfelder
    for(const auto& p: container){
        neigCont.push_back(w.getNeighborFields(p));
    }
// Nachschauen ob alle Nachbarfelder mindestens 2 Freie Nachbarfelder haben.
    for(NeighborContainer* con1: neigCont){
        for(auto& p1: *con1){
            if(container.find(p1) != container.end()) continue;
            if(w.getWormMap().getTheHomeFor(p1.getX(), p1.getY()) == 1) continue;
            NeighborContainer* neighborOfWurm;
            neighborOfWurm = w.getNeighborFields(p1);
            uint count(0);
            for(const auto& p2: *neighborOfWurm){
                if(container.find(p2) != container.end()) continue;
                if(w.getWormMap().getTheHomeFor(p2.getX(), p2.getY()) == 1) continue;
                count++;
            }
            if(count <= 1) return false;
        //if(count == 2){
            bool result(true);
            for(const auto& point: container){
                worldBuffer.getWormMap().setTheHomeFor(point.getX(), point.getY(), 1);
            }
            uint field = worldBuffer.getWormMap().getTheHomeFor(p1.getX(), p1.getY());
            worldBuffer.getWormMap().setTheHomeFor(p1.getX(), p1.getY(), 1);
            Worm wormBuffer(container);
            if(Tools::countWorldParts(w, wormBuffer) != numberOfParts){
                result = false;
            }
            worldBuffer.getWormMap().setTheHomeFor(p1.getX(), p1.getY(), field);
            for(const auto& point: container){
                worldBuffer.getWormMap().setTheHomeFor(point.getX(), point.getY(), wurmColor);
            }
            if(!result) return false;
        }
    }
// Nachschauen ob man die Karte aufsplittet
    for(const auto& p: container){
        worldBuffer.getWormMap().setTheHomeFor(p.getX(), p.getY(), 1);
    }
    bool result(false);
    Worm wormBuffer(container);
    if(Tools::countWorldParts(worldBuffer, wormBuffer) == numberOfParts){
        result = true;
    } else {
        std::cout   << "Value: " << Tools::countWorldParts(worldBuffer, wormId)
                    << "NumberOfParts: "<< numberOfParts << std::endl;
    }
    for(const auto& p: container){
        worldBuffer.getWormMap().setTheHomeFor(p.getX(), p.getY(), wurmColor);
    }
    return result;
}


/**************************************************************************************************
 *                          Worm Homebringer
 * ***********************************************************************************************
std::string EasyWormsToHome::bringTheWormHome(world& w, const Worm& wurm, uint& countOpen, uint& countClose){
    Heuristics::EasyWormHeuristics Heur(w, 0);
    Heur.createDistanceGraph();
    AStarCalculation calc;
    std::string result(calc.getPath(w, Heur));
    countOpen = calc.getCountOpenList();
    countClose = calc.getCountCloseList() - calc.getCountOpenList();
    return result;
}

/**************************************************************************************************
 *                                          WormHomeBringerDeep
 * ***********************************************************************************************/

std::string EasyWormsToHome::bringTheWormHomeDeep(world& w, const Worm& wurm){
    WorldDeep wo(w);
    uint id;
    for(int i=0; i<w.getWormCount(); ++i){
        if(w.getWorm(i).getId() == wurm.getId()){
            id = i;
            break;
        }
    }
    Heuristics::EasyWormHeuristics Heur(wo, id);
    Heur.createDistanceGraph();

    DFS_AStar calc;
    std::string result(calc.calc(w, Heur));
    wo.clearMoves();
    std::cout << "world after copy" << wo;
    return result;
}
