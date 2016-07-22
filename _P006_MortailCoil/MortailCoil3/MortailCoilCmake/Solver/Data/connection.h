/**
    A passage connects two regions. if between this region is a region with extactly two passages, we can jump over these passage
*/
#ifndef CONNECTION_H
#define CONNECTION_H
#include <algorithm>
#include <vector>
#include "Move.h"
#include "pos.h"

typedef unsigned int uint;

namespace Solver
{
namespace Data
{

class Connection
{
public:

    /**
      set all to zero.
     * @brief Connection
     */
    Connection();

    /**
      create a connection
     * @brief Connection
     * @param Start
     * @param dest
     * @param startRegion
     * @param destRegion
     */
    Connection(Position Start, Position dest,
               uint startRegion, uint destRegion, bool solveable);
    /**
      Set the destination to zero
     * @brief Connection
     * @param startCol : The start col
     * @param startRow : The start row
     * @param startRegionID : The start region
     */
    Connection(uint startCol, uint startRow, uint startRegionID);
    /**
     * @brief Connection All variables are given
     * @param startCol
     * @param startRow
     * @param startRegionID
     * @param destCol
     * @param destRow
     * @param destRegionID
     */
    Connection(uint startCol, uint startRow, uint startRegionID,
               uint destCol, uint destRow, uint destRegionID);
    /**
      add a new destination. It is useful if two neighbor regions have two passages
     * @brief addDestination
     * @param destRow
     * @param destCol
     * @param destRegionID
     */
    void addDestination(uint destRow, uint destCol, uint destRegionID);
    void addDestination(Position, uint destRegionID);

    /**
     * @brief startPosition the position of the vertex where the jump begin
     */
    Position startPosition;
    /**
      The ID of the region where the jump begin. The ID is the position in the list
     * @brief startRegionID
     */
    uint startRegionID;
    /**
     * @brief destinationPosition the position where the jump ends
     */
    Position destinationPosition;
    /**
     * @brief destinationID The id of the region where the jump ends
     */
    uint destinationID;
    /**
     * @brief overJumpedRegions All regions where the jump goes over
     *
     */
    std::vector<uint> overJumpedRegions;

    bool solveable;
};

}
}

#endif // CONNECTION_H
