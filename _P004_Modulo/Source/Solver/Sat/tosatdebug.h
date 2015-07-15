#ifndef TOSATDEBUG_H
#define TOSATDEBUG_H
#include "tosat.h"
#include "tosathalf.h"

namespace Solver{

class toSatDebug: public Solver::tosat, public Solver::toSatHalf
{
public:
   toSatDebug( const Game::Game& t, const std::string& satSolver): tosat(t, satSolver), toSatHalf(t, satSolver){}

   virtual void createDimacs(const std::string&);
   Game::Game etSolution();
};

}
#endif // TOSATDEBUG_H
