#ifndef MOVE_H
#define MOVE_H
#include "MoveDirection.h"
#include <algorithm>
#include "pos.h"

namespace Solver{
namespace Data{

/**
 * @brief The Move struct consists of a start positon (start), a destination position (destion),
 * a move direction (direction) and wheter it is jumped ( its lands on a passagefield)
 */
struct Move{
    Position start;
    Position destination;
    MoveDirection direction;
    bool jump;

    Move(){}
    Move(const Move& other);
    /**
     * @brief Move Specify the start position and the direction
     * @param col
     * @param row
     * @param dir
     */
    Move(uint col, uint row, MoveDirection dir);
    /**
     * @brief Move Specify the start position and the direction
     * @param col
     * @param row
     * @param dir
     */
    Move(const Position& Pos , MoveDirection dir);
    /**
     * @brief Solver::Data::Move::operator == compare two moves, wheter they are equal
     * @param other : The second elemt to compare
     * @return true if the elements are equal, false if they are different
     */
    bool operator==(const Move& other) const;
    /**
      copy operator
     * @brief operator =
     * @param other
     * @return
     */
    Move& operator=(const Move& other);
};

}
}

std::ostream& operator<<(std::ostream& out, const Solver::Data::Move& m);

#endif // MOVE_H
