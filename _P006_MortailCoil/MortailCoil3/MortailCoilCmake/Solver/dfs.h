/**
    1. solve all region with 2 passages in both directions
    1.1 it is solvable: change the connections
    1.2 the direction is not solvable. Set it to unpassable
    2. determine where the start can be
    3. determine where the destination can be
    4. iterate over all possible start positions

Options for fast break
1. Dead ends.
1.1 2 Dead ends in the whole Graph
1.2 One Dead end in a reagion, that can't be the end

2. Bad Regions
2.1 the region is divided in two parts such that both parts have an odd number off
    passages.

3. Graph
    The graph is splittet in two parts
*/
#ifndef DFS_H
#define DFS_H
#include "../World/world.h"
#include "../Solver/Data/connection.h"
#include "../Solver/Data/SavedMoveSequence.h"
#include "../Solver/regiondfs.h"
#include "../Solver/regionsolverwithoutheuristics.h"
#include <vector>
#include <boost/container/map.hpp>

namespace Solver
{
class dfs
{
public:
    /**
     * @brief dfs
     * @param world The world you like to solve
     */
    dfs(const World::world& w);

    /**
     * @brief solve. Solve the world
     */
    void solve();
    /**
     * @brief getSolutionString
     * @return the solutionstring for hacker.org
     */
    std::string getSolutionString();


    uint getNumberStartPositions() const;
    uint getNumberRegions() const;
    uint getNumberBinaryRegions() const;
    /**
     * @brief getCounterBinarySolved
     * @return the number of solutions for Binary Regions. Should be the double of Binary Regions
     */
    uint getCounterBinarySolved() const;
    /**
     * @brief getCounterRegionSolverCalled
     * @return The number of calls of a Region solver
     */
    uint getCounterRegionSolverCalled() const;

    std::string getCaseOfStartPositions() const;

protected:
    uint numberStartPositions;
    uint numberRegions;
    uint numberBinaryRegions;
    uint counterBinarySolved;
    uint counterRegionSolverCalled;

    std::string caseOfStartPositions;
    /**
     * @brief world The world what is needed to solve
     */
    World::world world;
    /**
     * @brief regionDfs each region has an own solver.
    */
    std::vector<Solver::RegionSolverWithoutHeuristics> regionDfs;
    /**
     * @brief connections If a region solver reach a connection points it give the control back to the worlf solver.
     * The connection says, where to go on.
    */
    boost::container::map< Position, std::vector< Solver::Data::Connection> > connections;
    /**
     * @brief savedMoveSequences. If a move sequence is finish, it will be saved her.
     */
    std::vector< Solver::Data::SavedMoveSequence> savedMoveSequences;

    /**
     * @brief startPositions a vector with all possible start positions. Each start position can
     * also be a destination position
     */
    std::vector<Position > startPositions;
    /**
      find all posible start positions. It looks for a few cases:
      1. The graph has two dead ends. Then both dead ends are possible start positions.
      2. The graph has one dead end.
      2.1 All regions have an even number of connections. Then the start end the end must be in the region,
          where the dead end is.
      3. Exactly two regions have an odd number of connections. Then the start is in one of this regions.
      4. All regions have an even number of connections. It is the worst case. All position are abel to be
         the start position
    * @brief findAllPossibleStartPositions
     */
    void findAllPossibleStartPositions();
    /**
     * 1. create the regionDFS vector
     * 2. find all possible Start positions
     * 3. Solve all binary connected regions
     * 4. connect the regions
     **/
    void init();
    /**
      create the connections and save them in connections
      1. create all neighbour connections
      2. overjump the 2 passage regions. recursivly
     * @brief createConnections
     */
    void createConnections();
    /**
      Solve all regions with two connections. Save the results in savedMoveSequences
     * @brief solveAllBinaryRegions
     */
    void solveAllBinaryRegions();

    void addConnection(const Solver::Data::Connection& con);

    void refreshConnection(Position oldStart, Position oldDest, Position newDest, uint newRegionID);
};

}
#endif // DFS_H
