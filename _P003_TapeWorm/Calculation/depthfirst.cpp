#include "depthfirst.h"
#include "Exceptions/depthwayfound.h"
DepthFirst::DepthFirst()

{
}

std::string DepthFirst::calc(world &w, Heuristics::AStarHeuristics &heur){
    notExpand = 0;
    timer.restart();
    //createdStates = 0;
    visitedStates = 0;
    uint deep(heur(&w));
    startDepth = deep;
    wo = w;
    while(true){
        std::cout << "current Depth: " <<deep << std::endl;
        try{
            if(doDepth(heur, deep)){
                maxDepth=deep;
                w.setWorms(wo.getWorms());
                return wo.getPath();
            }
            deep += DEEPENING;
        }catch(...){

        }
    };
}

bool DepthFirst::doDepth(Heuristics::AStarHeuristics& heur, int maxDeep){
    visitedStates++;
    if(visitedStates % 100000 == 0){
        std::cout << "visited Nodes: " << visitedStates << " CurrentDeep: " << maxDeep << " Steps to home:" << heur(&wo) << "time " << timer.elapsed() << std::endl;
    }
    if((maxDeep >0) || (maxDeep == -1)){
        MoveInDeepContainer MoveDeep;
        wo.getAllMoves(MoveDeep, heur, maxDeep);
        for(const MoveInDeep& t: MoveDeep){
            wo.move(t);
            if(t.costToDest == 0){
                return true;
            }
            if(doDepth(heur, maxDeep-1)) return true;
            wo.oneMoveBack();
        }
    }
    return false;
}


/**************************************************************************************************
 *                      Getter
 * ***********************************************************************************************/
long unsigned int DepthFirst::getNotExpand(){
    return notExpand;
}

long long int DepthFirst::getCreatedStates(){
    return visitedStates;
}

uint DepthFirst::getMaxDepth(){
    return maxDepth;
}

uint DepthFirst::getStartDepth(){
    return startDepth;
}

uint DepthFirst::getSolutionLength(){
    return wo.getSolutionLength();
}
