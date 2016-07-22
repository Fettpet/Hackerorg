#ifndef SAVEDMOVESEQUENCE_H
#define SAVEDMOVESEQUENCE_H
#include <algorithm>
#include "Move.h"
#include <deque>
#include <stack>

namespace Solver
{
namespace Data
{

/**
 * @brief The SavedMoveSequence struct saves a sequence of moves.
 *
 */
struct SavedMoveSequence{
    SavedMoveSequence(const uint row, const uint col, const bool solve, const uint worldID);
    SavedMoveSequence(const uint row, const uint col, const bool solve);
    SavedMoveSequence(const SavedMoveSequence& other);
    /**
     * @brief worldID saves the number of the world in the datastructure
     */
    uint worldID;
    /**
     * @brief solveable saves if the Saved moves are solveable.
     * This variable is false if it is not solveable
     * Is this variable true, it is not known wheter it is solvable
     */
    bool solveable;
    /**
     * @brief start. The start position of the sequence. It must be a passage field.
     */
    Position start;
    /**
     * @brief dest the destination of the sequence. It must be a passage field. If it is
     * not solvable this must not be determined.
     */
    Position destionation;
    /**
     * @brief Moves The moves to go from start to end.
     */
    std::deque<Move> Moves;
    /**
     * @brief conditions. If the region has more than two passages, the conditions save the done moves.
     */
    std::deque<Move> conditions;

    /**
      copy operator
     * @brief operator =
     * @param other
     * @return
     */
    SavedMoveSequence& operator=(const SavedMoveSequence& other);
};
}
}

std::ostream& operator<<(std::ostream& out, const Solver::Data::SavedMoveSequence& Saved);
#endif // SAVEDMOVESEQUENCE_H
