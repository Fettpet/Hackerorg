#ifndef PDDLBASECLASS_H
#define PDDLBASECLASS_H
#include "../solver.h"
#include <string>

namespace Solver{
class PddlBaseClass : public Solver::SolverBaseClass
{
public:
    PddlBaseClass(const Game::Game& t, const std::string solver);

    Game::Game getSolution();
// Getter
    double getNeededTimeProblemCreate() const;
    double getNeededTimeDomainCreate() const;
    double getNeededTimeSolverCreate() const;

protected:
// function
    virtual void createProblemFile(const std::string& outFileName) = 0;
    virtual void createDomainFile(const std::string& outFileName) = 0;
    virtual void startSolver(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile)=0;
    virtual void readSolution(const std::string& solutionFile) = 0;
// vars
    std::string solver;
private:
    double timeProblemCreate, timeDomainCreate, timeSolver;
};
}
#endif // PDDLBASECLASS_H
