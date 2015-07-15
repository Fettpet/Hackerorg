clude "vertex.h"
#include "../../Heuristics/astarheuristics.h"
#define EDGEWEIGHT 1
Vertex::Vertex()
{
}



/**************************************************************************************************
 *
 * ***********************************************************************************************
// Erstellt einen neuen Knoten
costs Vertex::expand(){
    if(kids.empty()){
        PossibleMoveContainer moves;

        stats.getAllPosibleMoves(moves);
// Erstellen aller Kinder
        vertexCosts.countKids = moves.size();
        for(auto m: moves){
            stats.createNewWorldByMovingWorm(m, kids);
            Heuristics::AStarHeuristics heur(kids[kids.size()-1]);
            heur.createDistanceGraph();
            costs buffer;
            buffer.costGoal = heur.getDistancePlusCountOfFirstOutsider();
            buffer.costToComeHere = getCostToComeHere() + EDGEWEIGHT;
            buffer.costBest = buffer.costGoal + buffer.costToComeHere;
            buffer.countKids = 0;

        }
    }
}



/**************************************************************************************************
 *                                      Getter
 * ***********************************************************************************************/
/**
 * @brief Vertex::getCostGoal
 * @return Die Heristischen kosten zum Ziel
 */
uint Vertex::getCostGoal() const{
    return vertexCosts.costGoal;
}

/**
 * @brief Vertex::getCostToComeHere
 * @return Die Kosten um zu diesen Zustand zu kommen
 */
uint Vertex::getCostToComeHere() const{
    return vertexCosts.costToComeHere;
}

/**
 * @brief Vertex::getCostBestGoal
 * @return die niedrigsten kosten zum Ziel
 */
uint Vertex::getCostBest() const{
    return vertexCosts.costBest;
}

/**
 * @brief Vertex::getCountKids
 * @return
 */
uint Vertex::getCountKids() const{
    return vertexCosts.countKids;
}
/**
 * @brief Vertex::getCosts
 * @return Alle Kosten als Struct
 */
costs Vertex::getCosts() const {
    return vertexCosts;
}
/**************************************************************************************************
 *                          expand and Delete Vertex
 * ***********************************************************************************************/
/**
1. Schritt
        Denn besten Knoten suchen
2. Schritt
        Nachfolger Knoten erstellen
3. Schritt
        Die Kosten nach oben durchgeben
 * @brief expand
 * @return
 */
costs Vertex::expand(){
/// 1. Find best Vertex
/// if the kids are empty, the best Vertex is find
    costs bufferCosts;

    if(!kids.empty()){
        PossibleMoveContainer moves;

        for(possibleMoves m: moves){
            stats.createNewWorldByMovingWorm(m, )
        }
        stats.getAllPosibleMoves();
    } else {

    }
}
// Delete
void Vertex::deleteVertexes(){

}
private:
// calculate costs
uint calcCostGoal();
uint calcCostToComeHer();
