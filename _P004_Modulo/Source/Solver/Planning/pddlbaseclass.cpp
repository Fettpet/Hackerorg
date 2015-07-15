#include "pddlbaseclass.h"
#include <fstream>


/**************************************************************************************************
 *                                          Constructor
 * ***********************************************************************************************/
/**
Create a PDDL Baseclass Set Solver and gameField
 * @brief Solver::PddlBaseClass::PddlBaseClass
 * @param t
 * @param _solver
*/
Solver::PddlBaseClass::PddlBaseClass(const Game::Game& t, const std::string _solver):
    Solver::SolverBaseClass(t), solver(_solver)
{
}

/**************************************************************************************************
 *                              Solution creation
 * ***********************************************************************************************/
Game::Game Solver::PddlBaseClass::getSolution(){


    createDomainFile("/tmp/domain.txt");


    createProblemFile("/tmp/problem.txt");



    startSolver("/tmp/domain.txt", "/tmp/problem.txt", "/tmp/ausg.txt");

    readSolution("/tmp/ausg.txt");
    return gameField;
}

/**************************************************************************************************
 *                              Getter
 * ***********************************************************************************************/
double Solver::PddlBaseClass::getNeededTimeProblemCreate() const{
    return timeProblemCreate;
}
double Solver::PddlBaseClass::getNeededTimeDomainCreate() const{
    return timeDomainCreate;
}
double Solver::PddlBaseClass::getNeededTimeSolverCreate() const{
    return timeSolver;
}
