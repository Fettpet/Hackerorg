#ifndef MOD3TOSAT_H
#define MOD3TOSAT_H
#include "satbaseclass.h"


namespace Solver{
class Mod3ToSat  : public Solver::SatBaseClass
{
public:
    Mod3ToSat(const Game::Game& t, const std::string& satSolver);
    void createDimacs(const std::string& fileName);

private:
    void generateMod3Adder(std::multiset<uint> vars);
    uint getFormularMod3FirstOut(uint var1, uint var2, uint var3);
    uint getFormularMod3SecondOut(uint var1, uint var2, uint var3);
};
}
#endif // MOD3TOSAT_H
