#include "mod2tosat.h"
#include <fstream>
#include "../../Defines/compileoptions.h"
/**************************************************************************************************
 *                                          constructor
 * ***********************************************************************************************/
/**
constructor for the ggame
 * @brief Solver::Mod2ToSat::Mod2ToSat

*/

Solver::Mod2ToSat::Mod2ToSat(const Game::Game& _game, const std::string& satSolver):
    Solver::SatBaseClass(_game, satSolver)
{
}


/**************************************************************************************************
 *                              Dimacs
 * ***********************************************************************************************/
/**
  creates the dimacs
 * @brief Mod2ToSat::createDimacs
 * @param fileName: output filename for Dimacs file
*/

void Solver::Mod2ToSat::createDimacs(const std::string& fileName){
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
            std::set<uint> bufferForXor;
            for(const Tile& t: gameField.getTiles()){
        //        if(fieldX + t.getTile(i).getSizeX() > sizeX) continue;
        //        if(fieldY + t.getTile(i).getSizeY() > sizeY) continue;
                std::set<PosInt> bufferForOr;
                for(uint tileX=0; tileX <= fieldX; ++tileX){
                    if(tileX + t.getSizeX() > sizeX) break;
                    for(uint tileY=0; tileY<=fieldY; ++tileY){

                        if(tileY + t.getSizeY() > sizeY) break;
                        if(t.getField( fieldX - tileX, fieldY - tileY)){
                            bufferForOr.insert(PosInt(tileX, tileY, t.getId()));
                        }
                    }
                }
                uint var = getFormularImplication(bufferForOr);
                if(var!= 0)
                    bufferForXor.insert(var);
            }
            if(bufferForXor.empty()) continue;
            if(gameField.getField().getField(fieldX, fieldY) == 0)
                bufferForXor.insert(getTrueVar());

            getAFormulaFromXor(bufferForXor);
        }
    }
//*/
    std::fstream data;
    data.open(fileName, std::ios::out);
    data << "p cnf " << countVars << " " << countClausel << std::endl;
    data << result.str().c_str();
    data.close();

}



/**************************************************************************************************
 *                                              CNFs
 * ***********************************************************************************************/
/**
 * @brief Mod2ToSat::getFormularImplication
 * @param set
 * @param result
 * @return
 */
uint Solver::Mod2ToSat::getFormularImplication(const std::set<PosInt>& set){
#if COMMENTSDIMACS == 1
    result << "c OR ";
    for(PosInt p: set){
        result << p << " (" << varToInt[p] << ") ";
    }
    result << std::endl;
#endif
    if(set.empty()) return 0;
    if(set.size() == 1) {
        return varToInt[*set.begin()];
    }
    countVars++;
    int nr =countVars;

    result << "-" << nr << " ";
    for(PosInt z: set){
        result << varToInt[z] << " ";
    }
    result << "0" << std::endl;

    countClausel++;


    for(PosInt z: set){
        result << "-" << varToInt[z] << " " << nr << " 0" << std::endl;
        countClausel++;
    }
    return nr;
}

/**
  generate for the Variables an Xor Contraint
 * @brief Mod2ToSat::getAFormulaFromXor
 * @param vars
 * @param result
 */
void Solver::Mod2ToSat::getAFormulaFromXor(const std::set<uint>& vars){
// A Comment
#if COMMENTSDIMACS == 1
    result << "c XOR ";
    for(uint p: vars){
        if(p ==  getTrueVar()){
            result << "TRUE ";
        } else if(p == getFalseVar()){
            result << "FALSE ";
        }else {
            result << p << " ";
        }
    }
    result << std::endl;
#endif
// 1. Case no Vars in the Container -> do nothing
    if(vars.size() == 0) {
        return;
// 2. Case only one Var in Container -> build a Unit
    }else if(vars.size() == 1){
        result << *vars.begin() << " 0" << std::endl;
        countClausel++;
        return;
// 3. Case exactly 2 Vars in Container
    } else if(vars.size() == 2){
        std::set<uint>::iterator it(vars.begin());
        uint nr1 = *it;
        ++it;
        uint nr2 = *it;
        result << nr1 << " " << nr2 <<  " 0" <<std::endl;
        result << "-" << nr1 << " -" << nr2 << " 0" <<std::endl;
        countClausel += 2;
        return;
    }
// 4. Case more than 2 Vars
// Xor with Tseitin
    std::set<uint>::iterator it=vars.begin();
    uint var1= *it;
    it++;
    uint var2 = *it;
    it++;
    countVars++;
    uint var3 = countVars;
    for(; it!=vars.end(); ++it){
// 1.Step hinzufügen der Vars
        result << "-" << var1 << " -" << var2 << " -" << var3 << " 0" << std::endl;
        result << var1 << " " << var2 << " -" << var3 << " 0" << std::endl;
        result << "-" << var1 << " " << var2 << " " << var3 << " 0" << std::endl;
        result << var1 << " -" << var2 << " " << var3 << " 0" << std::endl;
        countClausel += 4;
// 2. Vars neu belegen
        var1 = var3;
        var2 = *it;
        countVars++;
        var3 = countVars;
    }
    countVars--;
    result << var1 << " " << var2 << " 0" << std::endl;
    result << "-" << var1 << " -" << var2 << " 0" << std::endl;
   // countVars++;
    countClausel += 2;
}

