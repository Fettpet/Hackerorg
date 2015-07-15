/**
  Create for the modulo 2 case a Dimacs File:
  Contraints:
    1. Exactly one
        For all variables generated for one Tile
    2. Modolo Contraint

*/

#ifndef MOD2TOSAT_H
#define MOD2TOSAT_H

#include <set>
#include <sstream>
#include <map>
#include <string>

#include "../../Modulo/pos.h"
#include "../../Modulo/game.h"
#include "satbaseclass.h"

namespace Solver{
    class Mod2ToSat : public Solver::SatBaseClass
    {
    public:
        Mod2ToSat(const Game::Game& t, const std::string& satSolver);
        void createDimacs(const std::string& fileName);

    private:
        void getFormularEquivalent(const std::set<PosInt>& set, std::stringstream& result);


        void getAFormulaFromXor(const std::set<PosInt>& vars);
        uint getFormularImplication(const std::set<PosInt>& vars);
        void getAFormulaFromXor(const std::set<uint>& vars);


    };
}
#endif // MOD2TOSAT_H
