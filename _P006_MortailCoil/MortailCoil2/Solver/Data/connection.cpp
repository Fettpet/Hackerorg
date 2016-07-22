#include "connection.h"

Solver::Data::Connection::Connection(Position start, Position dest,
           uint startRegion, uint destRegion, bool sol):
    startPosition(start),
    startRegionID(startRegion),
    destinationPosition(dest),
    destinationID (destRegion),
    solveable(sol)
{

}


void Solver::Data::Connection::addDestination
(uint destRow, uint destCol, uint destRegionID)
{
    addDestination(Position(destCol, destRow), destRegionID);
}


void Solver::Data::Connection::addDestination(Position dest, uint destRegionID)
{
    overJumpedRegions.push_back(destRegionID);
    destinationID = destRegionID;
    destinationPosition = dest;
}
