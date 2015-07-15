#ifndef PddlStripsConst_H
#define PddlStripsConst_H
#include "pddlbaseclass.h"

namespace Solver {


class PddlStripsConst: public Solver::PddlBaseClass
{
public:
    PddlStripsConst(const Game::Game& t, const std::string& solver);
    //PddlStripsConst(const Game::Game t, const std::string& sol);
    void createProblemFile(const std::string& outFileName);
    void createDomainFile(const std::string& outFileName);
protected:

    void readSolution(const std::string& solutionFile);
    void startSolver(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile);

private:
    void startSolverMadagascar(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile);
    void readSolutionMadagascar(const std::string &solutionFile);
};
}
#endif // PddlStripsConst_H
