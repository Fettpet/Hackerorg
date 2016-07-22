#include "solverbaseclass.h"

using namespace Solver;
RegionSolverBaseClass::RegionSolverBaseClass(const World::Region& r):
    region(r), solveable(true), solved(false), controlBack(false),
    numberControlChanges(0), numberMoves(0)
{
    numberUnblockedPassages = region.getNumberPassages();
    numberUnblockedFields = region.getNumberUnblocked();
}

/**************************************************************************************************
 *                  Getter
 * ***********************************************************************************************/
bool RegionSolverBaseClass::isSolveable() const
{
    return solveable;
}
bool RegionSolverBaseClass::isSolved() const
{
    return solved;
}


uint RegionSolverBaseClass::getNumberMoves() const
{
    return numberMoves;
}

uint RegionSolverBaseClass::getNumberUnblockedPassages() const
{
    return numberUnblockedPassages;
}

uint RegionSolverBaseClass::getNumberUnblockedFields() const
{
    return region.getNumberUnblocked();
}

bool RegionSolverBaseClass::giveControlBack() const
{
    return controlBack;
}

void RegionSolverBaseClass::getCurrentPosition(Position& result)
{
    if(numberMoves > 0)
        result = moveList.back().destination;
}


void RegionSolverBaseClass::getMoveSequenceForLastCall(std::deque<Solver::Data::Move>& result)
{
    uint startPosition = numberMoves - numberMovesBetweenControllChanges.top();
    std::copy(moveList.begin() + startPosition, moveList.end(), result.begin());
}

/**************************************************************************************************
 *              Solver
**************************************************************************************************/
/**
doMove(Move): führt den Move aus
    true: konnt bewegen,
    false: konnte nicht bewegen
 getFirstMove(Position, MOve): speichert die erste bewegung in Move.
 getNextMove(Position, direction, Move): get a new move
 changeDirection(): Ändert die richtung des Moves
    true: Falls die Richtung geändert werden konnte
    false: Richtung konnte nicht geändert werden
 backTrack: Macht den letzten Zug rückgängig
 * @brief RegionSolverBaseClass::solve
 * @param position
 */
void RegionSolverBaseClass::solve(const Position& position)
{
    needToBacktrack = false;
    Data::Move curMove;
    uint numberMovesInThisCall(0);

    getFirstMove(position, curMove);
// main loop
    while(!giveControlBack() && !isSolved() && isSolveable())
    {
// Can move in this direction
       // std::cout << "Current Move" << curMove << " Size: " << moveList.size() << std::endl;
        if(!needToBacktrack)
        {
            if(doMove(curMove))
            {
                std::cout << "Erfolgreich" << curMove << std::endl;
                numberMovesInThisCall++;
                numberMoves++;
                moveList.push_back(curMove);
                getNextMove(curMove.destination, curMove.direction, curMove);
                calcIsSolved();
            }
            else
            {
                needToBacktrack = true;
            }
        }

// something like backtrack
        else
        {
            while(true)
            {
                if(changeDirection(curMove))
                {
                    needToBacktrack = false;
                    break;
                }
                else if(numberMovesInThisCall == 0)
                {
                    solveable = false;
                    return;
                }
                else
                {
                    numberMoves--;
                    numberMovesInThisCall--;
                    backtrack(moveList.back());
                    curMove = moveList.back();
                    moveList.pop_back();

                }
            }
        }
        calcControlBack(curMove);

    }
    numberMovesBetweenControllChanges.push(numberMovesInThisCall);

}

/**************************************************************************************************
 *                      Moves
 * ***********************************************************************************************/


bool RegionSolverBaseClass::doMove(Data::Move& curMove,
                                   const int& colChange,
                                   const int& rowChange)
{

    if(moveList.size() == 5)
    {
        std::cout << "The Region" << getNumberUnblockedFields() << std::endl;
        std::cout << region << std::endl;
        std::cout << std::endl;
    }
    bool result(false);
    uint col(curMove.start.getCol());
    uint row(curMove.start.getRow());
    //go until blocked
    callBeforeMove();

    while(!region.isBlocked(col+colChange, row+rowChange))
    {
        calcIsSolveableDuringMove();
        callDuringMove();
        result = true;
        --numberUnblockedFields;
        region.setBlocked(col+colChange, row+rowChange);
        row += rowChange;
        col += colChange;
    }
    if(result == true){
        if(region.isPassage(col, row))
        {
            numberUnblockedPassages--;
        }
        curMove.destination = Position(col, row);
    }
    callAfterMove();
    calcIsSolveableAfterMove();
    std::cout << "Move after Move " << curMove << " Size: " << moveList.size() << std::endl;
    return result;
}

/**
    testet nicht ob die aktuelle Position blockiert ist.
    Bewegt den coil in eine Richtung, welche in curMove spezifiziert ist.
 * @brief Solver::regionDFS::doMove
*/
bool RegionSolverBaseClass::doMove(Data::Move& curMove)
{
    uint row(curMove.start.getRow()), col(curMove.start.getCol());
    region.setBlocked(col, row);
    if(region.isPassage(col, row))
    {
        numberUnblockedPassages--;
    }
    switch(curMove.direction){
        case Solver::Data::Down:
            return doMove(curMove, 0, 1);
        case Solver::Data::Up:
            return doMove(curMove, 0, -1);
        case Solver::Data::Right:
            return doMove(curMove, 1, 0);
        case Solver::Data::Left:
            return doMove(curMove, -1, 0);
    }


    return false;
}


void RegionSolverBaseClass::backtrack(const Data::Move& curMove)
{
    std::cout << "Backtrack: " << curMove << " Size: " << moveList.size() << std::endl;
    uint i;
    if(curMove.destination == curMove.start)
        return;
    if(region.isPassage(curMove.destination))
    {
        numberUnblockedPassages++;
    }
// first case: Right undo
    if(curMove.direction == Solver::Data::Right)
    {
        for(i=curMove.start.getCol(); i<curMove.destination.getCol(); ++i)
        {
            region.setUnblocked(i,curMove.start.getRow());
            ++numberUnblockedFields;
        }
    }else
// second case: Left undo
    if(curMove.direction == Solver::Data::Left)
    {
        for(i=curMove.start.getCol(); i>curMove.destination.getCol(); --i)
        {
            region.setUnblocked(i, curMove.start.getRow());
            ++numberUnblockedFields;
        }
    }else
// first case: Up undo
    if(curMove.direction == Solver::Data::Up)
    {
        for(i=curMove.start.getRow(); i>curMove.destination.getRow(); --i)
        {
            region.setUnblocked(curMove.start.getCol(), i);
            ++numberUnblockedFields;
        }
    }else
// first case: Right undo
    if(curMove.direction == Solver::Data::Down)
    {
        for(i=curMove.start.getRow(); i<curMove.destination.getRow(); ++i)
        {
            region.setUnblocked(curMove.start.getCol(), i);
            ++numberUnblockedFields;
        }
    }
    region.setUnblocked(curMove.destination.getCol(), curMove.destination.getRow());
    ++numberUnblockedFields;
}

void Solver::RegionSolverBaseClass::backtrackLastCall()
{
    uint numberCall = numberMovesBetweenControllChanges.top();
    for(int i=0; i< numberCall; ++i)
    {
        backtrack(moveList.back());
        moveList.pop_back();
    }
    numberMovesBetweenControllChanges.pop();
}
