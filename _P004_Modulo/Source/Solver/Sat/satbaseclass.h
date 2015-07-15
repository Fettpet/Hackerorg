/**
  Baseclass for Sat. Has functions for varibles and has variables
  @author: Sebastian Hahn
  @e-mail: s6303743@mail.zih.tu-dresden.de
*/
#ifndef SATBASECLASS_H
#define SATBASECLASS_H
#include <vector>
#include "../../Modulo/game.h"
#include "../../Modulo/pos.h"
#include "../solver.h"
#include <map>
#include <sstream>
#include <set>
#include <string>

namespace Solver{

/**************************************************************************************************
 *                                          Typedefs
 * ***********************************************************************************************/
class literal{
public:
    literal(): varId(0), positiv(true){}
    literal(const literal& other): varId(other.varId), positiv(other.positiv){}
    literal& operator=(const literal& other){
        this->varId = other.varId;
        this->positiv = other.positiv;
        return *this;
    }

    uint varId;
    bool positiv;
    bool operator<(const literal& t1) const{
        return varId <t1.varId;
    }

};

/**************************************************************************************************
 *                                          class
 * ***********************************************************************************************/
class SatBaseClass: public Solver::SolverBaseClass
{
public:
// Constructor
    SatBaseClass(const Game::Game& game, const std::string& satSolver);

// Getter
    uint getCountClausel()const;
    uint getCountVars()const;
    uint getCountVarsBegin()const;
    uint getNeededTimeForCreateDimacs()const;
    uint getNeededTimeForSatSolving()const;


    Game::Game getSolution();

protected:
// Functions
    void createVars();
    uint getTrueVar();
    uint getFalseVar();

//
    virtual void createDimacs(const std::string&) = 0;
    void readSolution(const std::string& solutionFile, const std::string& satSolver);
    void readSolutionGlucose(const std::string& solutionFile);
    void readSolutionOther(const std::string& solutionFile);
    void startSatSolver(const std::string& solver, const std::string dimacs);

// tseitin Formeln
    uint createTseitinOr(std::multiset<literal>& vars);

    uint createTseitinAnd(std::multiset<literal>& vars);

// exactly one
    void getExactlyOne(const std::set<PosInt>& vars);
// vars
// stringstream for creating DImacs
    std::stringstream result;
// Container for Mapping from Integer to readable Variables
    std::vector<PosInt> intToVar;
// Container fpr Mapping form Variables to Integer
    std::map<PosInt, int> varToInt;
// The Game what will be solved
    Game::Game gameField;
// Variables that stands for true and false
    uint trueVar, falseVar;
//  count of Vars and clausels in Dimacs
    uint countVars, countVarsBegin;
    uint countClausel;
// needed Times
    uint neededTimeForCreateDimacs;
    uint neededTimeForSatsolving;
    std::string satSolver;
private:

    std::vector<std::string> split( std::string &s, char delim);
};
}
#endif // SATBASECLASS_H
