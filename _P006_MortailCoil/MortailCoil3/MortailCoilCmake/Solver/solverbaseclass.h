/**
  Variablen:
  solved: Die Region wurde gelöst
  solveable: Die Region ist lösbar bei True,
            von dem Startpunkt aus, ist sie nicht lösbar bei false
  needToBacktrack
  holdControl: Behält die Kontrolle

Benötigte Konzepte

*/
#ifndef REGIONSOLVERBASECLASS_H
#define REGIONSOLVERBASECLASS_H

#include "../World/world.h"
#include "../World/region.h"
#include "Data/Move.h"
#include "Data/SavedMoveSequence.h"
#include <deque>

namespace Solver {
class RegionSolverBaseClass
{
public:
    RegionSolverBaseClass(const World::Region& region);

    void solve(const Position& startField);
    bool isSolveable() const;
    bool isSolved() const;
    void backtrackLastCall();

    bool giveControlBack() const;
    uint getNumberMoves() const;
    uint getNumberUnblockedPassages() const;
    uint getNumberUnblockedFields() const;
    void getMoveSequenceForLastCall( std::deque<Solver::Data::Move>& result);

    void getCurrentPosition(Position& result);
protected:
    /**
     * @brief changeDirection
     * @param currentMove
     * @return true: if the direction could changed
     *         false: otherwise
     */
    virtual bool changeDirection(Data::Move& currentMove)=0;
    /**
     * @brief getNextMove This function is called when a move is needed to calculate
     * The result is saved in curMove
     * @param currentPosition The position
     * @param lastDirection The direction, the coil was moved last
     * @param curMove The result parameter
     */
    virtual void getNextMove(const Position& currentPosition, const Data::MoveDirection& lastDirection, Data::Move& curMove) = 0;
    /**
      This function is called, when the function solve is called. The result is
      stored in currentMove. This move is done.
     * @brief getFirstMove
     * @param position
     */
    virtual void getFirstMove(const Position& position, Solver::Data::Move& currentMove) = 0;
    /**
     * @brief controlBack
     * @return true: If the cant do anything and give the controll back to world SOlver
     *         false: The solving goes on
     */
    virtual void calcControlBack(const Data::Move& curMove) =0;
    /**
     * @brief calcNextMove calc the next move
     * The result is stored in currentMove
     */

    virtual void calcIsSolveableDuringMove() = 0;
    /**
     * @brief calcIsSolveableAfterMove
     Calculate wheter the region can be solved. This function is called before the control
     is given back to the World solver. The result is stored in solveable.
    */
    virtual void calcIsSolveableAfterMove() = 0;
    /**
     * @brief calcIsSolved
     * calc whether the region is solved, i.e all fields are blocked.
     * This function is called before the control is going back to the world solver.
     * The result is stored in solveable.
     */
    virtual void calcIsSolved() = 0;
    /**
     * @brief doDuringMove
     * is called each time, the coil is moved a field
     */
    virtual void callDuringMove() = 0;
    /**
     * @brief doAfterMove
     * is called before the control is given back to the world solver.
     */
    virtual void callAfterMove() = 0;
    /**
     * @brief doBeforeMove
        is called before the coil is moved, i.e. when the solver get the control
    */
    virtual void callBeforeMove() = 0;

    bool solved, solveable, controlBack, needToBacktrack;
    World::Region region;


private:
    uint numberMoves;
    std::deque< Solver::Data::Move > moveList;
    /**
     * @brief numberMovesBetweenControllChanges
     * for each call of solve, it saves the number of Move until the control is given back
     */
    std::stack<uint> numberMovesBetweenControllChanges;
    uint numberControlChanges;

    /**
     * @brief numberUnblockedPassages The Number of passages that are not blocked
     */
    uint numberUnblockedPassages;

    /**
     * @brief numberUnblockedFields
     */
    uint numberUnblockedFields;

// Move Functions
    bool doMove(Solver::Data::Move& toDoMove);
    bool doMove(Solver::Data::Move& toDoMove, const int& colChange, const int& rowChange);
    void backtrack(const Data::Move& curMove);

};

}
#endif // SOLVERBASECLASS_H
