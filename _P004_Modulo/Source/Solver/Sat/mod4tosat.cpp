#include "mod4tosat.h"
#include <fstream>
#include "../../Defines/compileoptions.h"
/**************************************************************************************************
 *                                          Constructor
 * ***********************************************************************************************/
/**
constructor for the ggame
 * @brief Solver::Mod3ToSat::Mod3ToSat

*/

Solver::Mod4ToSat::Mod4ToSat(const Game::Game& _game, const std::string& satSolver):
    Solver::SatBaseClass(_game, satSolver)
{
}

/**************************************************************************************************
 *                                          Dimacs
 * ***********************************************************************************************/

void Solver::Mod4ToSat::createDimacs(const std::string& fileName){
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
                                lit.positiv = true;
                                lit.varId = varToInt[PosInt(tileX, tileY, t.getId())];
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
                // generierung der Ausgänge
                result << "c" << std::endl;
                result << "c Decode at Position (" << fieldX << ", " << fieldY << ")" << std::endl;
                result << "c" << std::endl;
                generateMod4Adder(bufferForVar);
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
void Solver::Mod4ToSat::generateMod4Adder(std::multiset<uint> vars){
    uint var1(0), var2(0), var3(0), var4(0);
    uint bufferVar1(0), bufferVar2(0), bufferVar3(0);
// 1. Case empty
    if(vars.empty()) return;
// Add vars so there are at least 4
    for(uint count=vars.size(); count < 4; ++count){
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
        } else if(var3 == 0){
            var3 = curVar;
            continue;
        }
// generate the Mod 3 Adder
        var4 = curVar;
        bufferVar1 = getFormularMod4FirstOut(var1, var2, var3, var4);
        bufferVar2 = getFormularMod4SecondOut(var1, var2, var3, var4);
        bufferVar3 = getFormularMod4ThirdOut(var1, var2, var3, var4);
        var1 = bufferVar1;
        var2 = bufferVar2;
        var3 = bufferVar3;

    }
// The last output must bb Zero
    result << "-" << var1 << " 0" << std::endl;
    result << "-" << var2 << " 0" << std::endl;
    result << "-" << var3 << " 0" << std::endl;
    countClausel += 3;
}

/**
A1 = <-x1, -x2, -x3, -x4>, <x1, x2, x3, x4>
* @brief Solver::Mod4ToSat::getFormularMod4FirstOut
 * @param x1
 * @param x2
 * @param x3
 * @param x4
 * @return ID of A1
 */
uint Solver::Mod4ToSat::getFormularMod4FirstOut(uint x1, uint x2, uint x3, uint x4){
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
    // N1 = -x1 -x2 -x3 -x4
    #if COMMENTSDIMACS == 1
        result << "c N1(" <<  N1 << ") = -x1(" << x1 << ")" <<" -x2(" << x2 << ") -x3(" << x3 << ") -x4(" << x4 << ")" << std::endl;
    #endif
        result << "-" << N1 << " -" << x1 << " -" << x2 << " -" << x3 << " -" << x4 <<" 0" << std::endl;
        result << N1 << " " << x1 << " 0" << std::endl;
        result << N1 << " " << x2 << " 0" << std::endl;
        result << N1 << " " << x3 << " 0" << std::endl;
        result << N1 << " " << x4 << " 0" << std::endl;

    // right
    // N2 = x1 x2 x3
    #if COMMENTSDIMACS == 1
        result << "c N2(" <<  N2 << ") = x1(" << x1 << ")" <<" x2(" << x2 << ") x3(" << x3 << ") x4(" << x4 << ")" << std::endl;
    #endif
        result << "-" << N2 << " " << x1 << " " << x2 << " " << x3 << " " << x4 << " 0" << std::endl;
        result << N2 << " -" << x1 << " 0" << std::endl;
        result << N2 << " -" << x2 << " 0" << std::endl;
        result << N2 << " -" << x3 << " 0" << std::endl;
        result << N2 << " -" << x4 << " 0" << std::endl;

        countClausel += 13;
        return A1;
}

/**
  A2 = <-x1, -x2, -x3, -x4>, <x1, x2, x3>, <x1, x2, x4>, <x1, x3, x4>,  <x2, x3, x4>
 * @brief Solver::Mod4ToSat::getFormularMod4SecondOut
 * @param x1
 * @param x2
 * @param x3
 * @param x4
 * @return A2
*/
uint Solver::Mod4ToSat::getFormularMod4SecondOut(uint x1, uint x2, uint x3, uint x4){
    // gen new Vars
        uint A2(++countVars);
        uint N1(++countVars);
        uint N2(++countVars);
        uint N3(++countVars);
        uint N4(++countVars);
        uint N5(++countVars);

    #if COMMENTSDIMACS == 1
        result << "c generate a Formular for second output" << std::endl;
    #endif


    // Maybe right
    // A1 = N1 and N2 and N3 and N4 and N5
    #if COMMENTSDIMACS == 1
        result << "c A2(" << A2 <<") = N1(" << N1 << ") and N2(" << N2 << ") and N3("
               << N3 << ") and N4(" << N4 << ") and N4(" << N4 << ") N5(" << N5 << ")" << std::endl;
    #endif
        result << A2 << " -" << N1 << " -" << N2 << " -" << N3 << " -" << N4 << " -" << N5 <<" 0" << std::endl;
        result << "-" << A2 << " " << N1 << " 0" << std::endl;
        result << "-" << A2 << " " << N2 << " 0" << std::endl;
        result << "-" << A2 << " " << N3 << " 0" << std::endl;
        result << "-" << A2 << " " << N4 << " 0" << std::endl;
        result << "-" << A2 << " " << N5 << " 0" << std::endl;


    // N1 = -x1 -x2 -x3 -x4
    #if COMMENTSDIMACS == 1
        result << "c N1(" << N1 << ") = -x1(" << x1 << ") -x2(" << x2 << ") -x3(" << x3 << ") -x4(" << x4 << ")" << std::endl;
    #endif
        result << "-" << N1 << " -" << x1 << " -" << x2 << " -" << x3 << " -" << x4 << " 0" << std::endl;
        result << N1 << " " << x1 << " 0" << std::endl;
        result << N1 << " " << x2 << " 0" << std::endl;
        result << N1 << " " << x3 << " 0" << std::endl;
        result << N1 << " " << x4 << " 0" << std::endl;
    // N2 = x1 x2 x3
    #if COMMENTSDIMACS == 1
        result << "c N2(" << N2 << ") = x1(" << x1 << ") x2(" << x2 << ")" << " x3(" << x3  << ")" << std::endl;
    #endif
        result << "-" << N2 << " " << x1 << " " << x2 << " " << x3 << " 0" << std::endl;
        result << N2 << " -" << x1 << " 0" << std::endl;
        result << N2 << " -" << x2 << " 0" << std::endl;
        result << N2 << " -" << x3 << " 0" << std::endl;

    // N3 = x1 x2 x4
    #if COMMENTSDIMACS == 1
        result << "c N3(" << N3 << ") = x1(" << x1 << ") x2(" << x2 << ") x4(" << x4 << ")" << std::endl;
    #endif
        result << "-" << N3 << " " << x1 << " " << x2 << " " << x4 << " 0" << std::endl;
        result << N3 << " -" << x1 << " 0" << std::endl;
        result << N3 << " -" << x2 << " 0" << std::endl;
        result << N3 << " -" << x4 << " 0" << std::endl;

    // N4 = x1 x3 x4
    #if COMMENTSDIMACS == 1
        result << "c N4(" << N4 << ") = x1(" << x1 << ") x3(" << x3 << ") x4(" << x4 << ")" << std::endl;
    #endif
        result << "-" << N4 << " " << x1 << " " << x3 << " "<< x4 << " 0" << std::endl;
        result << N4 << " -" << x1 << " 0" << std::endl;
        result << N4 << " -" << x3 << " 0" << std::endl;
        result << N4 << " -" << x4 << " 0" << std::endl;

    // N5 = x2 x3 x4
    #if COMMENTSDIMACS == 1
        result << "c N5(" << N5 << ") = x2(" << x2 << ") x3(" << x3 << ") x4(" << x4 << ")" << std::endl;
    #endif
        result << "-" << N5 << " " << x2 << " " << x3 << " "<< x4 << " 0" << std::endl;
        result << N5 << " -" << x2 << " 0" << std::endl;
        result << N5 << " -" << x3 << " 0" << std::endl;
        result << N5 << " -" << x4 << " 0" << std::endl;

        countClausel += 27;
        return A2;
}
/**
  A3 = <-x1, -x2, -x3, -x4>, <x1, x2>, <x1, x3>, <x1, x4>, <x2, x3>, <x2, x4>, <x3, x4>
 * @brief Solver::Mod4ToSat::getFormularMod4ThirdOut
 * @param x1
 * @param x2
 * @param x3
 * @param x4
 * @return
 */
uint Solver::Mod4ToSat::getFormularMod4ThirdOut(uint x1, uint x2, uint x3, uint x4){
// gen new Vars
    uint A3(++countVars);
    uint N1(++countVars);
    uint N2(++countVars);
    uint N3(++countVars);
    uint N4(++countVars);
    uint N5(++countVars);
    uint N6(++countVars);
    uint N7(++countVars);

#if COMMENTSDIMACS == 1
    result << "c generate a Formular for second output" << std::endl;
#endif


    // Maybe right
    // A1 = N1 and N2 and N3 and N4 and N5 and N6 and N7
#if COMMENTSDIMACS == 1
    result  << "c A2(" << A3 <<") = N1(" << N1 << ") and N2(" << N2 << ") and N3("
            << N3 << ") and N4(" << N4 << ") and N4(" << N4 << ") N5(" << N5 << ") N6("
            << N6 << ") N7(" << N7 << ")" << std::endl;
#endif
    result << A3 << " -" << N1 << " -" << N2 << " -" << N3 << " -" << N4 << " -" << N5 << " -" << N6 << " -" << N7 << " 0" << std::endl;
    result << "-" << A3 << " " << N1 << " 0" << std::endl;
    result << "-" << A3 << " " << N2 << " 0" << std::endl;
    result << "-" << A3 << " " << N3 << " 0" << std::endl;
    result << "-" << A3 << " " << N4 << " 0" << std::endl;
    result << "-" << A3 << " " << N5 << " 0" << std::endl;
    result << "-" << A3 << " " << N6 << " 0" << std::endl;
    result << "-" << A3 << " " << N7 << " 0" << std::endl;

    countClausel += 8;
// N1 = -x1 -x2 -x3 -x4
#if COMMENTSDIMACS == 1
    result << "c N1(" << N1 << ") = -x1(" << x1 << ") -x2(" << x2 << ") -x3(" << x3 << ") -x4(" << x4 << ")" << std::endl;
#endif
    result << "-" << N1 << " -" << x1 << " -" << x2 << " -" << x3 << " -" << x4 << " 0" << std::endl;
    result << N1 << " " << x1 << " 0" << std::endl;
    result << N1 << " " << x2 << " 0" << std::endl;
    result << N1 << " " << x3 << " 0" << std::endl;
    result << N1 << " " << x4 << " 0" << std::endl;

    countClausel += 5;
// N2 = x1 x2
#if COMMENTSDIMACS == 1
    result << "c N2(" << N2 << ") = x1(" << x1 << ") x2(" << x2 << ")" << std::endl;
#endif
    result << "-" << N2 << " " << x1 << " " << x2 << " 0" << std::endl;
    result << N2 << " -" << x1 << " 0" << std::endl;
    result << N2 << " -" << x2 << " 0" << std::endl;

    countClausel +=3;
// N3 = x1 x3
#if COMMENTSDIMACS == 1
    result << "c N3(" << N3 << ") = x1(" << x1 << ") x3(" << x3 << ")" << std::endl;
#endif
    result << "-" << N3 << " " << x1 << " " << x3 << " 0" << std::endl;
    result << N3 << " -" << x1 << " 0" << std::endl;
    result << N3 << " -" << x3 << " 0" << std::endl;

    countClausel += 3;
// N4 = x1 x4
#if COMMENTSDIMACS == 1
    result << "c N4(" << N4 << ") = x1(" << x1 << ") x4(" << x4 << ")" << std::endl;
#endif
    result << "-" << N4 << " " << x1 << " " << x4 << " 0" << std::endl;
    result << N4 << " -" << x1 << " 0" << std::endl;
    result << N4 << " -" << x4 << " 0" << std::endl;

    countClausel += 3;
// N5 = x2 x3
#if COMMENTSDIMACS == 1
    result << "c N5(" << N5 << ") = x2(" << x2 << ") x3(" << x3 << ")" << std::endl;
#endif
    result << "-" << N5 << " " << x2 << " " << x3 << " 0" << std::endl;
    result << N5 << " -" << x2 << " 0" << std::endl;
    result << N5 << " -" << x3 << " 0" << std::endl;

    countClausel+=3;
// N6 = x2 x4
#if COMMENTSDIMACS == 1
    result << "c N6(" << N6 << ") = x2(" << x2 << ") x4(" << x4 << ")" << std::endl;
#endif
    result << "-" << N6 << " " << x2 << " " << x4 << " 0" << std::endl;
    result << N6 << " -" << x2 << " 0" << std::endl;
    result << N6 << " -" << x4 << " 0" << std::endl;
    countClausel += 3;

// N7 = x3 x4
#if COMMENTSDIMACS == 1
    result << "c N7(" << N7 << ") = x3(" << x3 << ") x4(" << x4 << ")" << std::endl;
#endif
    result << "-" << N7 << " " << x3 << " " << x4 << " 0" << std::endl;
    result << N7 << " -" << x3 << " 0" << std::endl;
    result << N7 << " -" << x4 << " 0" << std::endl;
    countClausel += 3;

    return A3;
}
