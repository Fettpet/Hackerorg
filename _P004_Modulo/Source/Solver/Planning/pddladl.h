#ifndef PDDLADL_H
#define PDDLADL_H
#include "../solver.h"
#include "pddlbaseclass.h"
#include <string>
#include <sstream>
namespace Solver{
class PddlADL : public Solver::PddlBaseClass
{
public:
    PddlADL(const Game::Game& t, const std::string solver);

  //  Game::Game getSolution();



protected:
    void startBFS(const std::string& problem, const std::string& domain, const std::string& output, const std::string& solver);
    void readSolutionBFS(const std::string& solutionFile);

    void createProblemFile(const std::string& outFileName);
    void createDomainFile(const std::string& outFileName);
    void readSolution(const std::string& solutionFile);
    void startSolver(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile);


private:
// Solver
    // Diverse Zeitfunktionen
    double timeProblemCreate, timeDomainCreate, timeSolver;

};

}
#endif // PDDLADL_H
