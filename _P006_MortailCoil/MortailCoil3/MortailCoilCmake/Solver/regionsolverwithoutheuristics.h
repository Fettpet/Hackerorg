#ifndef REGIONSOLVERWITHOUTHEURISTICS_H
#define REGIONSOLVERWITHOUTHEURISTICS_H
#include "solverbaseclass.h"

namespace Solver
{
class RegionSolverWithoutHeuristics: public Solver::RegionSolverBaseClass
{
public:
    RegionSolverWithoutHeuristics(const World::Region& region);


protected:
    /**
     * @brief changeDirection
     * @param currentMove
     * @return true: if the direction could changed
     *         false: otherwise
     */
    virtual bool changeDirection(Data::Move& currentMove);
    /**
     * @brief getNextMove This function is called when a move is needed to calculate
     * The result is saved in curMove
     * @param currentPosition The position
     * @param lastDirection The direction, the coil was moved last
     * @param curMove The result parameter
     */
    virtual void getNextMove(const Position& currentPosition, const Data::MoveDirection& lastDirection, Data::Move& curMove);
    /**
      This function is called, when the function solve is called. The result is
      stored in currentMove. This move is done.
     * @brief getFirstMove
     * @param position
     */
    virtual void getFirstMove(const Position& position, Solver::Data::Move& currentMove);
    /**
     * @brief controlBack
     * @return true: If the cant do anything and give the controll back to world SOlver
     *         false: The solving goes on
     */
    virtual void  calcControlBack(const Data::Move& curMove);
    /**
     * @brief calcIsSolveableDuringMove
        calculate wheter the region can be solved. This function is called
        after the coil is moved one field. The result is stored in solveable.
    */
    virtual void calcIsSolveableDuringMove();
    /**
     * @brief calcIsSolveableAfterMove
     Calculate wheter the region can be solved. This function is called before the control
     is given back to the World solver. The result is stored in solveable.
    */
    virtual void calcIsSolveableAfterMove();
    /**
     * @brief calcIsSolved
     * calc whether the region is solved, i.e all fields are blocked.
     * This function is called before the control is going back to the world solver.
     * The result is stored in solveable.
     */
    virtual void calcIsSolved();
    /**
     * @brief doDuringMove
     * is called each time, the coil is moved a field
     */
    virtual void callDuringMove();
    /**
     * @brief doAfterMove
     * is called before the control is given back to the world solver.
     */
    virtual void callAfterMove();
    /**
     * @brief doBeforeMove
        is called before the coil is moved, i.e. when the solver get the control
    */
    virtual void callBeforeMove();
};
}
#endif // REGIONSOLVERWITHOUTHEURISTICS_H
