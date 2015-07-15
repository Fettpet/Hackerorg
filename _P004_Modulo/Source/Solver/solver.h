/* Base Class for all Solvers
@Author: Sebastian Hahn
@Email: s6303743@mail.zih.tu-dresden.de

*/
#ifndef SOLVER_H
#define SOLVER_H
#include <string>
#include <vector>
#include "../Modulo/pos.h"
#include "../Modulo/game.h"
namespace Solver{
    class SolverBaseClass
    {
    public:
// Standard Constructor
        SolverBaseClass( const Game::Game& t): gameField(t){}
       // SolverBaseClass( const Game::Game& t, const std::string& a): gameField(t){}
// functioon for  Solution generation
        virtual Game::Game getSolution() = 0;

    protected:
        Game::Game gameField;

    };
}
#endif // SOLVER_H
