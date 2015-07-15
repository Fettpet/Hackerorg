#include "satbaseclass.h"
#include "../../Defines/compileoptions.h"
#include "../../Exceptions/stringexception.h"
#include <fstream>
#include <algorithm>
#include <sys/time.h>
#include <time.h>
/**************************************************************************************************
 *                                        Constructor
 * ***********************************************************************************************/
/**
Constructor for Solver
Set all to Zero execpt the game
 * @brief Solver::SatBaseClass::SatBaseClass
 * @param _game: The game, that will be solved
 */
Solver::SatBaseClass::SatBaseClass(const Game::Game& _game, const std::string& _satSolver):
    Solver::SolverBaseClass(_game),
    gameField(_game), trueVar(0), falseVar(0), countVars(0), countClausel(0),
    neededTimeForCreateDimacs(0), neededTimeForSatsolving(0)
{
    satSolver = _satSolver;
    createVars();
}

/**++**********************************************************************************************
 *                                          Solutioncreation
 * ***********************************************************************************************/
Game::Game Solver::SatBaseClass::getSolution(){
    std::stringstream out;

    createDimacs("./dimacs.txt");



   // satSolver = "./SparrowToRiss.sh";
    //outTmpDimacs << "./cp3 /tmp/dimacs.txt -enabled_cp3 -dimacs=/tmp/dimacsTmp.txt -enabled_cp3 -cp3_stats -up -subsimp -bve -no-bve_gates -no-bve_strength -bve_red_lits=1 -cp3_bve_heap=1 -bve_heap_updates=1 -bve_totalG -bve_cgrow_t=1000 -bve_cgrow=10";
    //std::system(outTmpDimacs.str().c_str());
    std::system("rm /tmp/model.txt");
    out << satSolver << " ./dimacs.txt >> /tmp/model.txt";
    std::cout << out.str() << std::endl;
    std::system(out.str().c_str());


    readSolution("/tmp/model.txt",satSolver);
    return gameField;
}


/**************************************************************************************************
 *                                          getter
 * ***********************************************************************************************/
/**
  * @brief getCountVarsBegin
  * @return number of Variables before Tseitin
 */
 uint Solver::SatBaseClass::getCountVarsBegin()const{
     return countVarsBegin;
 }
/**
 * @brief Solver::SatBaseClass::getCountClausel
 * @return number of clausel in the Dimacs
 */
uint Solver::SatBaseClass::getCountClausel()const{
    return countClausel;
}
/**
 * @brief Solver::SatBaseClass::getCountVars
 * @return count of Variable in the Dimacs
 */
uint Solver::SatBaseClass::getCountVars()const{
    return countVars;
}
/**
 * @brief Solver::SatBaseClass::getNeededTimeForCreateDimacs
 * @return needed Time for creating Dimacs
 */
uint Solver::SatBaseClass::getNeededTimeForCreateDimacs()const{
    return neededTimeForCreateDimacs;
}
/**
 * @brief Solver::SatBaseClass::getNeededTimeForSatSolving
 * @return needed time from Satsolver
 */
uint Solver::SatBaseClass::getNeededTimeForSatSolving()const{
    return neededTimeForSatsolving;
}
/**
 * @brief Solver::SatBaseClass::getTrueVar
 * @return The Numbe that stands for True in Dimacs
 */
uint Solver::SatBaseClass::getTrueVar(){
    return trueVar;
}
/**
 * @brief Solver::SatBaseClass::getFalseVar
 * @return The Number, that stands for false in Dimacs
 */
uint Solver::SatBaseClass::getFalseVar(){
    return falseVar;
}

/**************************************************************************************************
 *                                      create vars
 * ***********************************************************************************************/
/**
    create the Start Variables based on the gamefield given in Constructor
 * @brief Solver::SatBaseClass::createVars
*/
void Solver::SatBaseClass::createVars(){
// clear old values
    varToInt.clear();
    intToVar.clear();
// create True and falsevar
    intToVar.push_back(PosInt(0,0,21210));
    intToVar.push_back(PosInt(0,0,121210));
    trueVar = 1;
    falseVar = 2;
    result << "1 0" << std::endl;
    result << "-2 0" << std::endl;
    countClausel = 2;
// create Vars
    uint sizeGameX, sizeGameY;
    sizeGameX = gameField.getField().getSizeX();
    sizeGameY = gameField.getField().getSizeY();
    for(const Tile& t: gameField.getTiles()){
        for(uint x=0; x<=sizeGameX-t.getSizeX(); ++x){
            for(uint y=0; y<=sizeGameY-t.getSizeY(); ++y){
                intToVar.push_back(PosInt(x,y,t.getId()));
             //   std::cout << PosInt(x,y,t.getId()) << "  ->  " << intToVar.size() << std::endl;
                varToInt.insert(std::pair<PosInt, uint> (PosInt(x,y,t.getId()), intToVar.size()));

            }
        }
    }
    countVars = intToVar.size();
// true and false are not count
    countVarsBegin = countVars-2;
}

/**************************************************************************************************
                                        Exactly one Contraint
 * ***********************************************************************************************/
/**
Generates a Exactly One Constraint. it is written in the result variable
 * @brief Solver::SatBaseClass::getExactlyOne
 * @param vars: Vars for the constraint
*/

void Solver::SatBaseClass::getExactlyOne(const std::set<PosInt>& vars){
// Comment in Dimacs
#if COMMENTSDIMACS == 1
    result << "c Exactly One of ";
    for(PosInt p: vars){
        result << p << " (" << varToInt[p] << ") ";
    }
    result << std::endl;
#endif

    std::stringstream clause, clauseComment;
// pairwaise to (-x1 or -x2)
    for( std::set< PosInt >::const_iterator it1 = vars.begin() ; it1 != vars.end() ; ++it1 )
    {
        clauseComment << *it1 << " ";
        clause << varToInt[*it1] << " ";
        bool found(false);
        for( std::set< PosInt >::const_iterator it2 = it1; it2 != vars.end() ; ++it2)
        {
            if(!found){
                found = true;
                continue;
            }
            countClausel++;
#if COMMENTSDIMACS == 1
            result << "c " << "-" << *it1 << " -" << *it2 << std::endl;
#endif
            result << "-" << varToInt[*it1] << " -" << varToInt[*it2] << " 0" << std::endl;
        }
    }

// for all x1..xn: ( x1 OR x2 OR .. OR xn-1 OR xn )
    if( !clause.str().empty()){
        countClausel++;
#if COMMENTSDIMACS == 1
        result << "c " << clauseComment.str() << std::endl;
#endif
        result << clause.str() << "0" << std::endl;
    }
}

/**************************************************************************************************
 *                                      Communication with Satsolver
 * ***********************************************************************************************/
/**
Split a String in after the delim symbol in a new string
 * @brief Solver::split
 * @param s: the string
 * @param delim: The split symbol
 * @return
*/
std::vector<std::string> Solver::SatBaseClass::split( std::string &s, char delim) {
    std::vector<std::string> elems;

    std::stringstream ss(s);
    std::string item;
    while ( std::getline(ss, item, delim) ) {
        elems.push_back(item);
    }
    return elems;
}

/**
  reads the Solutionfile and add the information into the gamefield
 * @brief Solver::SatBaseClass::readSolution
 * @param solutionFile: The solutionfile
 * @param _satsolverCmd: The used satsolver
*/
void Solver::SatBaseClass::readSolution(const std::string& solutionFile, const std::string& _satsolverCmd)
{
    if((_satsolverCmd.find("minisat") != std::string::npos) || (_satsolverCmd.find("glucose") != std::string::npos)){
        readSolutionGlucose(solutionFile);
    } else{
        readSolutionOther(solutionFile);
    }
}


void Solver::SatBaseClass::readSolutionGlucose(const std::string& solutionFile){
    std::string line;
    std::ifstream fileIn ( solutionFile.c_str());
    if (fileIn.is_open()){
        getline( fileIn, line );
// if it is not solved
        if( line == "UNSAT" ){
            throw Exception::StringException("UNSAT");
            std::cout << line << std::endl;
// read solution
        }else if( line == "SAT" ){
            getline( fileIn, line );
            std::vector<std::string> solution = split( line, ' ');
            for( const std::string& s : solution ){
                if( s == "0"){
                    break;
                }

// skip negativ vars
                if( s[0] == '-' ) continue;

                uint varAsInt = std::stoi(s);
                if(varAsInt == getTrueVar()) continue;
                if(varAsInt >= intToVar.size()) break;

                PosInt varAsPos =  intToVar[varAsInt];
                gameField.getTileById(varAsPos.getValue()).setSolutionX(varAsPos.getX());
                gameField.getTileById(varAsPos.getValue()).setSolutionY(varAsPos.getY());

            }
        }

        fileIn.close();
    }else {
        std::cerr << "Solver: Unable to open file" << std::endl;
    }
}

void Solver::SatBaseClass::readSolutionOther(const std::string &solutionFile){
    std::string line;
    std::ifstream fileIn ( solutionFile );
    while(!fileIn.eof()){

        std::getline( fileIn, line );
        if(line.empty()) continue;
        if(line[0] == 'c') continue;
        if(line[0] == 's') {
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            if(line.find("unsat") != std::string::npos){
                throw Exception::StringException("UNSAT");
                std::cerr << line << std::endl;
                break;
            }
        }
        if(line[0] == 'v'){

            line = line.substr(2);
            std::vector<std::string> solution = split( line, ' ');
            for( const std::string& s : solution )
            {
                if(s.empty()) continue;
                if( s == "0")
                {
                    break;
                }

                // skip negativ vars
                if( s[0] == '-' ) continue;

                uint varAsInt = std::stoi(s)-1;
                if(varAsInt == getTrueVar()) continue;
                if(varAsInt == 0) continue;
                if(varAsInt >= intToVar.size()) break;

                PosInt varAsPos =  intToVar[varAsInt];
                std::cout << "Ausgabe der Pos " << varAsInt << ": " << varAsPos << std::endl;
                gameField.getTileById(varAsPos.getValue()).setSolutionX(varAsPos.getX());
                gameField.getTileById(varAsPos.getValue()).setSolutionY(varAsPos.getY());


            }
        }
    }
}

/**************************************************************************************************
 *                                              Tseitin
 * ***********************************************************************************************/
/**
 A = x1 or x2 or ... xn
 create cnf to create an Equility.
 * @brief Solver::SatBaseClass::createTseitinOr
 * @param vars x1, ..., xn
 * @return A
*/
uint Solver::SatBaseClass::createTseitinOr(std::multiset<literal>& vars){
    if(vars.size() == 0) return getFalseVar();
    if(vars.size() == 1) return (*vars.begin()).varId;
    uint A(++countVars);
// if A is true, then one of the xi must be true
    result << "-" << A << " ";
    for(const literal& t: vars){
        if(t.positiv){
            result << t.varId << " ";
        }else {
            result << "-" << t.varId << " ";
        }
    }
    result << "0" << std::endl;
    countClausel++;

// if one of the xi is true then A is true
    for(const literal& t: vars){
        result << A << " ";
        if(t.positiv){
            result << "-" << t.varId << " 0" << std::endl;
        } else {
            result << t.varId << " 0" << std::endl;
        }
        countClausel++;
    }
    return A;
}

/**
 A = x1 and x2 and ... xn
 create cnf to create an Equility.
 * @brief Solver::SatBaseClass::createTseitinOr
 * @param vars x1, ..., xn
 * @return A
*/
uint Solver::SatBaseClass::createTseitinAnd(std::multiset<literal>& vars){
    if(vars.size() == 0) return getTrueVar();
    uint A(++countVars);
// if A is true, then all of the xi must be true
        result << A << " ";
        for(const literal& t: vars){
            if(t.positiv){
                result << "-" << t.varId << " ";
            }else {
                result << t.varId << " ";
            }
        }
        result << "0" << std::endl;
        countClausel++;

// if one of the xi is false then A is false
        for(const literal& t: vars){
            result << "-" << A << " ";
            if(t.positiv){
                result << t.varId << " 0" << std::endl;
            } else {
                result << "-" << t.varId << " 0" << std::endl;
            }
            countClausel++;
        }
    return A;
}
