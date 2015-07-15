/**
    use the half merger descripted in

http://link.springer.com/chapter/10.1007/978-3-642-02777-2_18
*/
#ifndef TOSATHALF_H
#define TOSATHALF_H
#include "satbaseclass.h"
#include "../solver.h"
#include "../../Modulo/pos.h"
#include <set>
#include <string>
#include <map>
#include <vector>

namespace Solver{

class toSatHalf: public Solver::SatBaseClass{
public:
// Standard Constructor
    toSatHalf( const Game::Game& t, const std::string& satSolver): SatBaseClass(t, satSolver){}

protected:
    void createHalfMergingNetwork(const std::vector<Solver::literal>& setOne,
                                  const std::vector<Solver::literal>& setTwo,
                                        std::vector<Solver::literal>& resultingVars);
    void createHalfSortingNetwork(const std::vector<Solver::literal>& inputVars,
                                        std::vector<Solver::literal>& outputVars);

    void createModuloNAdderHalf(std::vector<Solver::literal>& inputVars, const uint Modulo);
    virtual void createDimacs(const std::string&);

};


}

#endif // TOSAT_H
