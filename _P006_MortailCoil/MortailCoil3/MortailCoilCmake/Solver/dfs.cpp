#include "dfs.h"
#include <algorithm>

Solver::dfs::dfs(const World::world &w):
    numberStartPositions(0),
    numberRegions(0),
    numberBinaryRegions(0),
    counterBinarySolved(0),
    counterRegionSolverCalled(0),
    world(w)
{
    init();
}

uint Solver::dfs::getCounterRegionSolverCalled() const
{
    return counterRegionSolverCalled;
}

std::string Solver::dfs::getCaseOfStartPositions() const
{
    return caseOfStartPositions;
}

uint Solver::dfs::getCounterBinarySolved() const
{
    return counterBinarySolved;
}

uint Solver::dfs::getNumberBinaryRegions() const
{
    return numberBinaryRegions;
}

uint Solver::dfs::getNumberRegions() const
{
    return numberRegions;
}

uint Solver::dfs::getNumberStartPositions() const
{
    return numberStartPositions;
}



void Solver::dfs::init()
{
    // 1. create a solver for each region
    for(const World::Region& r: world.getRegions())
    {
        regionDfs.push_back(Solver::RegionSolverWithoutHeuristics(r));
    }
    // Find all possible start positions
    findAllPossibleStartPositions();

    // solve all binaries
    solveAllBinaryRegions();
}


/**
  find all posible start positions. It looks for a few cases:
  1. The graph has two dead ends. Then both dead ends are possible start positions.
  2. The graph has one dead end.
  2.1 All regions have an even number of connections. Then the start end the end must be in the region,
      where the dead end is.
  2.2 One region has an odd number of connections. Then the start is either the dead end or in the odd
      region.
  3. Exactly two regions have an odd number of connections. Then the start is in one of this regions.
  4. All regions have an even number of connections. It is the worst case. All position are abel to be
     the start position
* @brief findAllPossibleStartPositions
 */
void Solver::dfs::findAllPossibleStartPositions()
{
    // Search all region with an odd number of connections
    uint numberOdd(0);
    uint firstOddRegion;
    uint secondOddRegion;
    uint row, col, i;
    World::Region reg;
    Position deadEnd1;
    std::vector< Position > buffer;
    // 1st case: The graph has two dead ends. Both dead ends are possible start positions.
    if(world.getNumberDeadEnds() == 2)
    {
        caseOfStartPositions = "2 deadends";
        startPositions = world.getDeadEnds();
        return;
    }

// search the odd regions
    for(i=0; i < world.getNumberRegions(); ++i)
    {
        // the region i has an odd number of connections
        if(world.getRegion(i).getNumberPassages() % 2 == 1)
        {
            numberOdd++;
            if(numberOdd == 1)
            {
                firstOddRegion = i;
            }
            else
            {
                secondOddRegion = i;
            }
        }
    }
    // 2nd case:  The graph has one dead end.
    if(world.getNumberDeadEnds() == 1)
    {

        deadEnd1 = world.getDeadEnd(0);
        // 2.1  All regions have an even number of connections.
        if(numberOdd == 0)
        {
            caseOfStartPositions = "1 DeadEnd and All Regions even connections";
            for(const World::Region& region: world.getRegions())
            {
                if(region.isBlocked(deadEnd1))
                {
                    reg = region;
                    break;
                }
            }
            startPositions = reg.getUnblocked();
            startPositions.push_back(deadEnd1);
            return;
        }
        // 2.2 One region has an odd number of connections.
        else if(numberOdd == 1)
        {
            caseOfStartPositions = "1 DeadEnd and 1 odd";
            // All positions in the odd region
            reg = world.getRegion(firstOddRegion);
            startPositions = reg.getUnblocked();
            // The dead end
            startPositions.push_back(deadEnd1);
            return;
        }

    }
    //3. Case: two odd regions
    if(numberOdd == 2)
    {
        caseOfStartPositions = "Two Odd Regions";
        reg = world.getRegion(firstOddRegion);
        startPositions = reg.getUnblocked();

        reg = world.getRegion(secondOddRegion);
        buffer = reg.getUnblocked();
        startPositions.insert(startPositions.end(), buffer.begin(), buffer.end());

        return;
    }
    //4. Case only even region
    caseOfStartPositions = "All Even Region";
    for( col=0; col <world.getColumnCount(); ++col)
    {
        for( row=0; row < world.getRowCount(); ++row)
        {
            if(!world.isBlocked(col, row))
            {
                startPositions.push_back( Position(col, row));
            }
        }
    }

}


void Solver::dfs::solveAllBinaryRegions()
{
    uint i;
    for(i=0; i<world.getNumberRegions(); ++i)
    {
/*
        if(world.getRegion(i).getNumberPassages() == 2)
        {
            std::cout << world.getRegion(i) << std::endl;
            std::vector< Position > passages = world.getRegion(i).getPassages();
            // Solve
            Solver::Data::SavedMoveSequence seq(regionDfs[i].startSolving(passages[0]));
            seq.worldID = i;
            savedMoveSequences.push_back(seq);
            regionDfs[i].reset();
            std::cout << "1. " << seq << std::endl;
            seq = regionDfs[i].startSolving(passages[1]);
            seq.worldID = i;
            savedMoveSequences.push_back(seq);
            std::cout << "2. " << seq << std::endl;
        }
//*/
}

}

void Solver::dfs::createConnections()
{
// 1. Create the connections:
    uint i;
    uint firstRegion, secondRegion;
    bool firstFound;
    for(const auto& passage: world.getPassages())
    {
        firstFound = false;
        for(i=0; i< world.getNumberRegions(); ++i)
        {
            if(world.getRegion(i).isPassage(passage))
            {
                if(!firstFound)
                {
                    firstRegion = i;
                }
                else
                {
                    secondRegion = i;
                    break;
                }
            }
        }
        addConnection(Solver::Data::Connection(passage, passage, firstRegion, secondRegion, true));
        addConnection(Solver::Data::Connection(passage, passage, secondRegion, firstRegion, true));
    }
// 2. Overjump the solveable 2 Passage regions
    for(const auto& savedMove: savedMoveSequences)
    {
        if(!savedMove.solveable) continue;
        refreshConnection(savedMove.start, savedMove.start, savedMove.destionation, savedMove.worldID);
        
    }
}

void Solver::dfs::addConnection(const Solver::Data::Connection& con)
{
    // The key doesn't exists
    if(!connections.count(con.startPosition))
    {
        std::vector< Solver::Data::Connection > value;
        value.push_back(con);
        connections.insert(std::make_pair(con.startPosition, value));
    }
    else
// The key exists
    {

        connections[con.startPosition].push_back(con);
    }
}


void Solver::dfs::refreshConnection(Position oldStart, Position oldDest, Position newDest, uint newRegionID){
    for(Solver::Data::Connection& con: connections[oldStart])
    {
        if(con.destinationPosition == oldDest)
        {
            con.addDestination(newDest, newRegionID);
            break;
        }
    }
}

