#include "Move.h"

/*
 * copy constructor
 */
Solver::Data::Move::Move
(const Solver::Data::Move& other):
    start(other.start),
    destination(other.destination),
    direction(other.direction),
    jump(other.jump)
{}

Solver::Data::Move::Move(uint col, uint row, MoveDirection dir):
    start(Pos<uint>(col, row)),
    destination(Pos<uint>(col, row)),
    direction(dir)
{}

Solver::Data::Move::Move(const Position& pos, MoveDirection dir):
    start(pos),
    destination(pos),
    direction(dir)
{}


/*
 Operator
*/
bool
Solver::Data::Move::operator==
(const Move& other) const
{
// compare the destination
    if(other.destination != destination)
        return false;
// compare the start
    if(other.start != start)
        return false;

    return true;
}

Solver::Data::Move&
Solver::Data::Move::operator=
(const Solver::Data::Move& other)
{
    destination = other.destination;
    start = other.start;
    direction = other.direction;
    jump = other.jump;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Solver::Data::Move& m)
{
    out << "(" << m.start.getX() << ", " << m.start.getPosY() << ") -> (" << m.destination.getPosX() << ", " << m.destination.getPosY() << ")";
    switch(m.direction){
    case(Solver::Data::Left):
        out << " Left";
        break;
    case(Solver::Data::Down):
        out << "Down";
        break;
    case(Solver::Data::Right):
        out << "Right";
        break;
    case(Solver::Data::Up):
        out << "up";
        break;
    }
}
