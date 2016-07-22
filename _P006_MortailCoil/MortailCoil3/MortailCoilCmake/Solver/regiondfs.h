/**
the region dfs start in passage (or the start) and solve a region until it reach a passage
*/

#ifndef REGIONDFS_H
#define REGIONDFS_H
#include "../World/region.h"
#include "../Solver/Data/SavedMoveSequence.h"

#include <deque>
#include <stack>

namespace Solver {

class regionDFS
{
public:
// Constructor
    regionDFS(const regionDFS& other);
    regionDFS(const World::Region& reg);
// Solving Procedure
    /**
     * @brief startSolving the algorithm starts at (col, row) and solve until passage, solved or
     * unsolveable
     * @param row The start row
     * @param col The start col
     * @return A savedmoveSequence please read it there
     */
    Solver::Data::SavedMoveSequence
    startSolving(const uint col,
                 const uint row);

    Solver::Data::SavedMoveSequence
    startSolving(const Position pos);

    void reset();
    uint getNumberDeadEnds() const;
    uint getNumberUnblockedFields() const;

    inline uint getCurrentRow() const;

    inline uint getCurrentCol() const;


    bool doMove();

    bool doMoveUp();
    bool doMoveDown();
    bool doMoveLeft();
    bool doMoveRight();
    void backTrack();

protected:
    /**
     * both represent the current position of the coil
     */
    uint currentRow, currentCol;
    /**
     * @brief curMove The move that we like to do
     */
    Solver::Data::Move curMove;
    /**
     * @brief currentConditions Moveslist from the last call of the function
     */
    std::vector<Solver::Data::Move> currentConditions;
    /**
     * @brief movelist The moves from the current function call.
     */
    std::deque<Solver::Data::Move> movelist;
    /**
     * @brief numberDeadEnds The number of dead ends in the region
     */
    uint numberDeadEnds;
    /**
     * @brief numberUnblockedFields number of fields, that must be visited
     */
    uint numberUnblockedFields;

    /**
     * @brief region an object for the region
     */
    World::Region region;

    /**
        @brief the number of passages, that can be entered
    */
    uint numberUnblockedPassages;
};

}
#endif // REGIONDFS_H
