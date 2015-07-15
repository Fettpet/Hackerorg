#ifndef MOD4TOSAT_H
#define MOD4TOSAT_H
#include "satbaseclass.h"

namespace Solver{
class Mod4ToSat: public Solver::SatBaseClass
{
public:
    Mod4ToSat(const Game::Game& t, const std::string& satSolver);
    void createDimacs(const std::string& fileName);

private:
    void generateMod4Adder(std::multiset<uint> vars);
    uint getFormularMod4FirstOut(uint var1, uint var2, uint var3,  uint var4);
    uint getFormularMod4SecondOut(uint var1, uint var2, uint var3, uint var4);
    uint getFormularMod4ThirdOut(uint var1, uint var2, uint var3, uint var4);

};

}

#endif // MOD4TOSAT_H
