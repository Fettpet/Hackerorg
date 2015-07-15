

#include "dfs_astar.h"
#include "Exceptions/depthrestart.h"
DFS_AStar::DFS_AStar()
{
    std::set<world> buffer;
    for(int i=0; i<10000; ++i){
        visitedList.push_back(buffer);
    }
}


 bool DFS_AStar::doDepth(Heuristics::AStarHeuristics& heur, int maxDeep){
     visitedStates++;
     if(visitedStates % 100000 == 0){
         std::cout << "visited Nodes: " << visitedStates << "  not expand" << notExpand << " time " << timer.elapsed() << "Blocked List: ";
         int sum = 0;
         for(uint i=0; i< visitedList.size(); ++i){
             if(!visitedList[i].empty()){
                 sum += visitedList[i].size();
             }
         }
         std::cout << sum << std::endl;
         for(uint i=0; i< visitedList.size(); ++i){
             if(!visitedList[i].empty()){
                 std::cout << "\t\tSize of Set " << i << " : " << visitedList[i].size() << std::endl;
             }
         }
         throw DepthRestart();
     }
     if((maxDeep >0) || (maxDeep == -1)){
         MoveInDeepContainer MoveDeep;
         wo.getAllMoves(MoveDeep, heur, maxDeep);
         for(const MoveInDeep& t: MoveDeep){
             if(t.costToDest < maxDepth){
                 maxDepth = t.costToDest;
             }
             wo.move(t);
             bool found(false);
             for(int i=maxDeep+1; i<visitedList.size(); ++i){
                 if(visitedList[i].find(wo) != visitedList[i].end()){
                     notExpand++;
                     wo.oneMoveBack();
                     found = true;
                     break;
                 }
             }
             if(found) continue;
             if(t.costToDest == 0){
                 return true;
             }
             if(doDepth(heur, maxDeep-1)) return true;
             wo.oneMoveBack();
         }
         for(int i=0; i<maxDeep; ++i){
             visitedList[i].erase(wo);
         }
         visitedList[maxDeep].insert(wo);
     }
     return false;
 }

uint DFS_AStar::getBlockListSize(){
     uint result(0);

     for(uint i=0; i<visitedList.size(); ++i)
         result += visitedList[i].size();
     return result;
}
