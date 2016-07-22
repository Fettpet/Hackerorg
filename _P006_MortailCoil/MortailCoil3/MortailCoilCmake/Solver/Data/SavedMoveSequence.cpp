#include "SavedMoveSequence.h"

Solver::Data::SavedMoveSequence::SavedMoveSequence
(const uint row, const uint col, const bool solve, const uint _worldID):
    worldID(_worldID),
    solveable(solve),
    start(Position(col, row))
{

}

Solver::Data::SavedMoveSequence::SavedMoveSequence
(const uint row, const uint col, const bool solve):
    solveable(solve),
    start(Position(col, row))
{
}

Solver::Data::SavedMoveSequence::SavedMoveSequence(const SavedMoveSequence &other)
{
    *this = other;
}

Solver::Data::SavedMoveSequence&
Solver::Data::SavedMoveSequence::operator =(const Solver::Data::SavedMoveSequence& other)
{
    start = other.start;
    destionation = other.destionation;
    Moves = other.Moves;
    conditions = other.conditions;
    solveable = other.solveable;
    worldID = other.worldID;
    return *this;
}

std::ostream& operator<<(std::ostream& out, const Solver::Data::SavedMoveSequence& saved)
{
    //1 first case it is not solveable
    if(!saved.solveable)
    {
        out << "No solution candidate found from start " << saved.start;
    }
    else
    {

        for(const Solver::Data::Move& m: saved.Moves)
        {
            out << m << " -> ";
        }
    }
    return out;
}
