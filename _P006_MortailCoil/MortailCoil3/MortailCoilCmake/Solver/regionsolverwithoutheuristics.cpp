#include "regionsolverwithoutheuristics.h"

using namespace Solver;

RegionSolverWithoutHeuristics::RegionSolverWithoutHeuristics(const World::Region& r):
    RegionSolverBaseClass(r)
{

}

bool RegionSolverWithoutHeuristics::changeDirection(Data::Move& currentMove)
{
    if(currentMove.direction == Data::Down)
    {
        currentMove.direction = Data::Up;
        return true;
    }
    else if(currentMove.direction == Data::Up)
    {
        currentMove.direction = Data::Left;
        return true;
    }
    else if(currentMove.direction == Data::Left)
    {
        currentMove.direction = Data::Right;
        return true;
    }
    return false;
}

void RegionSolverWithoutHeuristics::getNextMove(const Position &currentPosition, const Data::MoveDirection &lastDirection, Data::Move &curMove)
{
    curMove.start = currentPosition;
    if(lastDirection == Data::Down || lastDirection == Data::Up)
    {
        curMove.direction = Data::Left;
    } else
    {
        curMove.direction = Data::Down;
    }
}

void RegionSolverWithoutHeuristics::getFirstMove(const Position& position, Solver::Data::Move& currentMove)
{
    currentMove = Data::Move(position, Data::Down);
}

void RegionSolverWithoutHeuristics::calcIsSolved()
{
    solved = (getNumberUnblockedFields() == 0);
}

void RegionSolverWithoutHeuristics::calcIsSolveableAfterMove()
{}

void RegionSolverWithoutHeuristics::calcIsSolveableDuringMove()
{}

void RegionSolverWithoutHeuristics::callAfterMove()
{}

void RegionSolverWithoutHeuristics::callBeforeMove()
{}

void RegionSolverWithoutHeuristics::callDuringMove()
{}

void RegionSolverWithoutHeuristics::calcControlBack(const Data::Move& curMove)
{
    if(region.isPassage(curMove.destination))
    {
        controlBack = true;
    }
}
