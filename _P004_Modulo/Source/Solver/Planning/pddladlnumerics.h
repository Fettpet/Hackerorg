#ifndef PDDLADLNUMERICS2_H
#define PDDLADLNUMERICS2_H
#include "pddlbaseclass.h"
#include <string>

namespace Solver{
class PddlAdlNumerics : public Solver::PddlBaseClass
{
public:
    PddlAdlNumerics(const Game::Game& t, const std::string solver);

    void createProblemFile(const std::string& outFileName);
    void createDomainFile(const std::string& outFileName);

protected:
    virtual void startSolver(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile);
    virtual void readSolution(const std::string& solutionFile);

};

}

#endif
