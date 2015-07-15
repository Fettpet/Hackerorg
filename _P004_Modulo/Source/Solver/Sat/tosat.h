#ifndef TOSAT_H
#define TOSAT_H
#include "satbaseclass.h"
#include <vector>
#include <string>

namespace Solver{


class tosat: public Solver::SatBaseClass
{
public:
// Standard Constructor
    tosat( const Game::Game& t, const std::string& satSolver): SatBaseClass(t, satSolver){}

    virtual void createDimacs(const std::string&);
protected:

// functions for case analysis
    void createMergingNetwork(const std::vector<Solver::literal>& setOne,
                                  const std::vector<Solver::literal>& setTwo,
                                        std::vector<Solver::literal>& resultingVars);
    void createSortingNetwork(const std::vector<Solver::literal>& inputVars,
                                  const std::vector<Solver::literal>& outputVars);

    void createModuloNAdder(const std::vector<Solver::literal>& inputVars, const uint Modulo);
// Dimacs

// sorting
    void createSortingNetwork(const std::vector<Solver::literal>& input2,
                                    std::vector<Solver::literal>& outputVars);
// functions for Merge
    void mergeA1B1(const std::vector<Solver::literal>& input1,
                   const std::vector<Solver::literal>& input2,
                         std::vector<Solver::literal>& outputVars);
    void mergeA1B1(const Solver::literal& setOne,
                   const Solver::literal& setTwo,
                         std::vector<Solver::literal>& outputVars);
    void mergeOneEmpty(const std::vector<Solver::literal>& input1,
                       const std::vector<Solver::literal>& input2,
                             std::vector<Solver::literal>& outputVars);
    void mergeManyBothEven(const std::vector<Solver::literal>& input1,
                           const std::vector<Solver::literal>& input2,
                                 std::vector<Solver::literal>& outputVars);
    void mergeManyOneEven(const std::vector<Solver::literal>& input1,
                          const std::vector<Solver::literal>& input2,
                                std::vector<Solver::literal>& outputVars);
    void mergeManyBothOdd(const std::vector<Solver::literal>& input1,
                          const std::vector<Solver::literal>& input2,
                                std::vector<Solver::literal>& outputVars);
};

}

#endif // TOSAT_H
