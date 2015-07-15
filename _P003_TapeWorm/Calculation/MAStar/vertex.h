#ifndef VERTEX_H
#define VERTEX_H
#include "../../World/world.h"

/**************************************************************************************************
 *                              Typedefs
 * ***********************************************************************************************/
typedef struct{
    uint costGoal, costToComeHere;
    uint costBest;
    uint countKids;
} costs;

/**************************************************************************************************
 *                                  Klasse
 * ***********************************************************************************************/
class Vertex
{
public:
    Vertex(const world& other, uint costToHere);
// Operator
    Vertex& operator=(const Vertex& other);
    Vertex& operator<(const Vertex& other);
// getter
    uint getCostGoal() const;
    uint getCostToComeHere() const;
    uint getCostBest() const;
    uint getCountKids() const;
    costs getCosts() const;
// expand
    costs expand();
// Delete
    void deleteVertexes();
private:
// calculate costs
    uint calcCostGoal();
    uint calcCostToComeHer();
// Kids
    WorldContainer< Vertex> kids;

    world stats;
// costs
    costs vertexCosts;
};

#endif // VERTEX_H
