#include "mod3tosat.h"
#include <fstream>
#include "../../Defines/compileoptions.h"
/**************************************************************************************************
 *                                          Constructor
 * ***********************************************************************************************/
/**
constructor for the ggame
 * @brief Solver::Mod3ToSat::Mod3ToSat

*/

Solver::Mod3ToSat::Mod3ToSat(const Game::Game& _game, const std::string& satSolver):
    Solver::SatBaseClass(_game, satSolver)
{
}


/***********************************************************************************++++++++*******
 *                                  Dimacs
 * ***********************************************************************************************/
/**
  creates the dimacs
 * @brief Mod3ToSat::createDimacs
 * @param fileName: output filename for Dimacs file
*/

void Solver::Mod3ToSat::createDimacs(const std::string& fileName){
// Exactly One
    uint sizeX(gameField.getField().getSizeX());
    uint sizeY(gameField.getField().getSizeY());
    for(const Tile& t: gameField.getTiles()){
        std::set<PosInt> buffer;
        for(uint x=0; x<=sizeX - t.getSizeX(); ++x){
            for(uint y=0; y<=sizeY - t.getSizeY(); ++y){
                buffer.insert(PosInt(x,y,t.getId()));

            }
        }
        getExactlyOne(buffer);
    }
// Modulo Contraint

    for(uint fieldX = 0; fieldX < sizeX; ++fieldX){
        for(uint fieldY = 0; fieldY < sizeY; ++fieldY){
            std::multiset<uint> bufferForVar;
            for(const Tile& t: gameField.getTiles()){
// Variablen in Buffer Set schreiben
                std::multiset<literal> bufferOr;
                for(uint tileX=0; tileX <= fieldX; ++tileX){
                    if(tileX + t.getSizeX() > sizeX) break;
                    for(uint tileY=0; tileY<=fieldY; ++tileY){
                        if(tileY + t.getSizeY() > sizeY) break;
                        if(t.getField( fieldX - tileX, fieldY - tileY)){
                            literal lit;
                            lit.varId = varToInt[PosInt(tileX, tileY, t.getId())];
                            lit.positiv = true;
                            bufferOr.insert(lit);
                        }
                    }
                }
                bufferForVar.insert(createTseitinOr(bufferOr));
// Set start value
            }
            if(gameField.getField().getField(fieldX,fieldY) != 0){
                for(int i=0; i <gameField.getField().getField(fieldX,fieldY); ++i ){
                    std::cout << "Add: (" << fieldX << ", " << fieldY << ") " << i << " " << (int)gameField.getField().getField(fieldX, fieldY) << std::endl;
                    bufferForVar.insert(getTrueVar());
                }
            }
#if COMMENTSDIMACS == 1
            // generierung der Ausgänge
            result << "c" << std::endl;
            result << "c Decode at Position (" << fieldX << ", " << fieldY << ")" << std::endl;
            result << "c";

            for(uint c: bufferForVar){
                if(c == getTrueVar()){
                    result << "TRUE" << " " ;
                } else {
                    result << intToVar[c-1] << "(" << c <<")";
                }
            }
            result << std::endl;
            result << "c" << std::endl;
#endif
            generateMod3Adder(bufferForVar);
        }
    }
//*/
// write it into file
    std::fstream data;
    data.open(fileName, std::ios::out);
    data << "p cnf " << countVars << " " << countClausel << std::endl;
    data << result.str().c_str();
    data.close();

}

/**************************************************************************************************
 *                              function for creating cnf
 * ***********************************************************************************************/
/**
  Not Debbuged yet
 generate a series of Mod 3 adder for all input variable. The last mod 3 Adder output vars
 are set to false
 * @brief Solver::Mod3ToSat::generateMod3Adder
 * @param vars input vars

*/
void Solver::Mod3ToSat::generateMod3Adder(std::multiset<uint> vars){
    uint var1(0), var2(0), var3(0);
    uint bufferVar1(0), bufferVar2(0);
// 1. Case empty
    if(vars.empty()) return;
// Add vars so there are at least 3
    for(uint count=vars.size(); count < 3; ++count){
        result << "c " << count << " " << vars.size() << std::endl;
        result << "c add Var" << std::endl;
        vars.insert(getFalseVar());
    }

    for(uint curVar : vars){
// Fill the vars
        if(var1 == 0) {
            var1 = curVar;
            continue;
        } else if(var2 == 0){
            var2 = curVar;
            continue;
        }
// generate the Mod 3 Adder
        var3 = curVar;
        bufferVar1 = getFormularMod3FirstOut(var1, var2, var3);
        bufferVar2 = getFormularMod3SecondOut(var1, var2, var3);
        var1 = bufferVar1;
        var2 = bufferVar2;

    }
// The last output must bb Zero
    result << "-" << var1 << " 0" << std::endl;
    result << "-" << var2 << " 0" << std::endl;
    countClausel += 2;
}

/**
  generate the first outputvar: The Formular is
  A1 = (-x1 -x2 -x3) and (x1 x2 x3)
 * @brief Solver::Mod3ToSat::getFormularMod3FirstOut
 * @param x1
 * @param x2
 * @param x3
 * @return Id of A1
 */

uint Solver::Mod3ToSat::getFormularMod3FirstOut(uint x1, uint x2, uint x3){
// gen new Vars
    uint A1(++ countVars);
    uint N1(++countVars);
    uint N2(++countVars);
#if COMMENTSDIMACS == 1
    result << "c generate a Formular for first output" << std::endl;
#endif


// Right
// A1 = N1 and N2
#if COMMENTSDIMACS == 1
    result << "c A1(" << A1 << ") = N1(" << N1 << ") and N2(" << N2 << ")" << std::endl;
#endif
    result << A1 << " -" << N1 << " -" << N2 << " 0" << std::endl;
    result << "-" << A1 << " " << N1 << " 0" << std::endl;
    result << "-" << A1 << " " << N2 << " 0" << std::endl;

//right
// N1 = -x1 -x2 -x3
#if COMMENTSDIMACS == 1
    result << "c N1(" <<  N1 << ") = -x1(" << x1 << ")" <<" -x2(" << x2 << ") -x3(" << x3 << ")" << std::endl;
#endif
    result << "-" << N1 << " -" << x1 << " -" << x2 << " -" << x3 << " 0" << std::endl;
    result << N1 << " " << x1 << " 0" << std::endl;
    result << N1 << " " << x2 << " 0" << std::endl;
    result << N1 << " " << x3 << " 0" << std::endl;

// right
// N2 = x1 x2 x3
#if COMMENTSDIMACS == 1
    result << "c N2(" <<  N2 << ") = x1(" << x1 << ")" <<" x2(" << x2 << ") x3(" << x3 << ")" << std::endl;
#endif
    result << "-" << N2 << " " << x1 << " " << x2 << " " << x3 << " 0" << std::endl;
    result << N2 << " -" << x1 << " 0" << std::endl;
    result << N2 << " -" << x2 << " 0" << std::endl;
    result << N2 << " -" << x3 << " 0" << std::endl;

    countClausel += 11;
    return A1;
}

/**
  generate the second outputvar: The Formular is
  A2 = (-x1 -x2 -x3) and (x1 x3) and (x1 x2) and (x2 x3)
 * @brief Solver::Mod3ToSat::getFormularMod3FirstOut
 * @param x1
 * @param x2
 * @param x3
 * @return Id of A2
 */
uint Solver::Mod3ToSat::getFormularMod3SecondOut(uint x1, uint x2, uint x3){
// gen new Vars
    uint A2(++countVars);
    uint N1(++countVars);
    uint N2(++countVars);
    uint N3(++countVars);
    uint N4(++countVars);

#if COMMENTSDIMACS == 1
    result << "c generate a Formular for second output" << std::endl;
#endif


// Maybe right
// A1 = N1 and N2 and N3 and N4
#if COMMENTSDIMACS == 1
    result << "c A2(" << A2 <<") = N1(" << N1 << ") and N2(" << N2 << ") and N3(" << N3 << ") and N4(" << N4 << ")" << std::endl;
#endif
    result << A2 << " -" << N1 << " -" << N2 << " -" << N3 << " -" << N4 << " 0" << std::endl;
    result << "-" << A2 << " " << N1 << " 0" << std::endl;
    result << "-" << A2 << " " << N2 << " 0" << std::endl;
    result << "-" << A2 << " " << N3 << " 0" << std::endl;
    result << "-" << A2 << " " << N4 << " 0" << std::endl;


// N1 = -x1 -x2 -x3
#if COMMENTSDIMACS == 1
    result << "c N1(" << N1 << ") = -x1(" << x1 << ") -x2(" << x2 << ") -x3(" << x3 << ")" << std::endl;
#endif
    result << "-" << N1 << " -" << x1 << " -" << x2 << " -" << x3 << " 0" << std::endl;
    result << N1 << " " << x1 << " 0" << std::endl;
    result << N1 << " " << x2 << " 0" << std::endl;
    result << N1 << " " << x3 << " 0" << std::endl;

// N2 = x1 x2
#if COMMENTSDIMACS == 1
    result << "c N2(" << N2 << ") = x1(" << x1 << ") x2(" << x2 << ")" << std::endl;
#endif
    result << "-" << N2 << " " << x1 << " " << x2 << " 0" << std::endl;
    result << N2 << " -" << x1 << " 0" << std::endl;
    result << N2 << " -" << x2 << " 0" << std::endl;

// N3 = x1 x3
#if COMMENTSDIMACS == 1
    result << "c N3(" << N3 << ") = x1(" << x1 << ") x3(" << x3 << ")" << std::endl;
#endif
    result << "-" << N3 << " " << x1 << " " << x3 << " 0" << std::endl;
    result << N3 << " -" << x1 << " 0" << std::endl;
    result << N3 << " -" << x3 << " 0" << std::endl;

// N3 = x1 x3
#if COMMENTSDIMACS == 1
    result << "c N4(" << N4 << ") = x2(" << x2 << ") x3(" << x3 << ")" << std::endl;
#endif
    result << "-" << N4 << " " << x2 << " " << x3 << " 0" << std::endl;
    result << N4 << " -" << x2 << " 0" << std::endl;
    result << N4 << " -" << x3 << " 0" << std::endl;

    countClausel += 18;
    return A2;
}
