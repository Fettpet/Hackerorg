#include "tosathalf.h"
#include <sstream>
#include "../../Defines/compileoptions.h"
#include <cassert>
#include <fstream>
/**************************************************************************************************
 *                                          Dimacs
 * ***********************************************************************************************/

void Solver::toSatHalf::createDimacs(const std::string& fileName){
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
                std::vector<Solver::literal> bufferForVar;
                for(const Tile& t: gameField.getTiles()){
    // Variablen in Buffer Set schreiben
                    std::multiset<Solver::literal> bufferOr;
                    for(uint tileX=0; tileX <= fieldX; ++tileX){
                        if(tileX + t.getSizeX() > sizeX) break;
                        for(uint tileY=0; tileY<=fieldY; ++tileY){

                            if(tileY + t.getSizeY() > sizeY) break;
                            if(t.getField( fieldX - tileX, fieldY - tileY)){
                                Solver::literal lit;
                                lit.positiv = true;
                                lit.varId = varToInt[PosInt(tileX, tileY, t.getId())];
                                bufferOr.insert(lit);
                            }
                        }
                    }
                    if(bufferOr.size() > 0){
                        Solver::literal lit;
                        lit.varId =createTseitinOr(bufferOr); lit.positiv = true;
                        bufferForVar.push_back(lit);
                    }
    // Set start value

                }
                if(gameField.getField().getField(fieldX,fieldY) != 0){
                    for(int i=0; i <gameField.getField().getField(fieldX,fieldY); ++i ){
                        std::cout << "Add: (" << fieldX << ", " << fieldY << ") " << i << " " << (int)gameField.getField().getField(fieldX, fieldY) << std::endl;
                        Solver::literal lit;
                        lit.varId =getTrueVar(); lit.positiv = true;
                        bufferForVar.push_back(lit);
                    }
                }
                // generierung der Ausgänge
#if COMMENTSDIMACS == 1
                result << "c" << std::endl;
                result << "c Decode at Position (" << fieldX << ", " << fieldY << ")" << std::endl;
                result << "c" << std::endl;
#endif
                createModuloNAdderHalf(bufferForVar, gameField.getField().getMod());
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
 *                          Modulo N
 * ***********************************************************************************************/

/**
  create a Modulo N Adder
 * @brief Solver::toSatHalf::createModuloNAdder
 * @param inputVars
 * @param Modulo
 */
void Solver::toSatHalf::createModuloNAdderHalf(std::vector<Solver::literal>& inputVars, const uint Modulo){
    std::vector<Solver::literal> sortVars;
    uint bound = std::pow(2, std::floor(std::log2(inputVars.size())) + 1);
    std::stringstream streamBuffer;
// 1 Case no input, no Adder
    if(inputVars.size() == 0) return;

// 2. Case: more then 0 vars
// Add Vars to get 2^n size
    std::cerr << "Bound : "<< bound << " " << inputVars.size() << std::endl;
    if(bound != 2*inputVars.size())
    for(uint i=inputVars.size(); i< bound; ++i){
        Solver::literal buffer;
        buffer.varId = getFalseVar(); buffer.positiv = true;
        inputVars.push_back(buffer);
    }
// Sort network

    createHalfSortingNetwork(inputVars, sortVars);

// create cnfs
    streamBuffer << "-" << sortVars[0].varId; // << " " << createTseitinOr()
    for(uint i=Modulo; i < sortVars.size(); i+=Modulo){
        sortVars[i].positiv = false;
        sortVars[i-1].positiv = true;
        std::multiset<Solver::literal> bufferSet;
        bufferSet.insert(sortVars[i]);
        bufferSet.insert(sortVars[i-1]);
        streamBuffer << " " << createTseitinAnd(bufferSet);
    }
    if(sortVars.size() % Modulo == 0){
        streamBuffer << " " << sortVars[sortVars.size() -1].varId;
    }
    streamBuffer << " 0" << std::endl;
#if COMMENTSDIMACS == 1
    result << "c" << std::endl;
    result << "c Add Constraint that one is true" << std::endl;
    result << "c" << std::endl;
#endif
    result << streamBuffer.str();
    countClausel++;
}

/**
Preconditions:
    1. setOne.size() == setTwo.size()
    2. setOne.size() = 2^n  n is element N

Didnt test the Preconditions
 * @brief createHalfMergingNetwork
 * @param setOne: First Var Set
 * @param setTwo: Second Var Set
 * @param resultingVars: New output vars
*/
void Solver::toSatHalf::createHalfMergingNetwork(const std::vector<Solver::literal>& setOne,
                              const std::vector<Solver::literal>& setTwo,
                                    std::vector<Solver::literal>& resultingVars){
    assert(setOne.size() == setTwo.size());
// 1. Case: setOne.size == 1
    if(setOne.size() == 0) return;
    if(setOne.size() == 1){
        Solver::literal varC1;
        varC1.varId = ++countVars; varC1.positiv = true;
        Solver::literal varC2;
        varC2.varId = ++countVars; varC2.positiv = true;
#if COMMENTSDIMACS == 1
        result << "c " << std::endl;
        result << "c Create Half Merger with 2 set with 1 Element" << std::endl;
        result << "c Vars Merge: " << setOne[0].varId << " " << setTwo[0].varId << std::endl;
        result << "c New Vars: " << varC1.varId << " " << varC2.varId << std::endl;
        result << "c" << std::endl;
#endif
        result << "-" << setOne[0].varId << " -" << setTwo[0].varId << " " << varC2.varId << " 0" << std::endl;
        result << "-" << setOne[0].varId << " " << varC1.varId << " 0" << std::endl;
        result << "-" << setTwo[0].varId << " " << varC1.varId << " 0" << std::endl;
        result << ""  << setOne[0].varId << " -" << varC2.varId << " 0" << std::endl;
        result << ""  << setTwo[0].varId << " -" << varC2.varId << " 0" << std::endl;
        result << ""  << setOne[0].varId << " " << setTwo[0].varId << " -" << varC1.varId << " 0" << std::endl;
        countClausel += 6;
        resultingVars.push_back(varC1);
        resultingVars.push_back(varC2);
        return;
    }
// 2. Case setOne.size() > 1
// Create Vars for input
    std::vector< Solver::literal > Merge1First, Merge1Second;
    std::vector< Solver::literal > Merge2First, Merge2Second;
    std::vector< Solver::literal > Merge1Result, Merge2Result;
// recursiv Merging
    for(uint i=0; i<setOne.size(); i+=2){
        Merge1First.push_back(setOne[i]);
        Merge1Second.push_back(setTwo[i]);

        Merge2First.push_back(setOne[i+1]);
        Merge2Second.push_back(setTwo[i+1]);
    }
    createHalfMergingNetwork(Merge1First, Merge1Second, Merge1Result);
    createHalfMergingNetwork(Merge2First, Merge2Second, Merge2Result);
// Add new clausels
    resultingVars.push_back(Merge1Result[0]);
#if COMMENTSDIMACS == 1
        result << "c " << std::endl;
        result << "c Create Half Merger with 2 set with " <<setOne.size() << " Elements" << std::endl;
        result << "c VarsSet One: ";
        for(uint i=0; i< setOne.size(); ++i){
            result << setOne[i].varId << " ";
        }
        result << std::endl;
        result << "c VarsSet Two: ";
        for(uint i=0; i< setTwo.size(); ++i){
            result << setTwo[i].varId << " ";
        }
        result << std::endl;
        result << "c" << std::endl;
#endif
    for(uint i=0; i<setOne.size()-1; ++i){
        Solver::literal c1, c2;
        c1.varId = ++countVars; c1.positiv = true;
        c2.varId = ++countVars; c2.positiv = true;

        result << "-" << Merge1Result[i+1].varId << " -" << Merge2Result[i].varId << " " << c2.varId << " 0" << std::endl;
        result << "-" << Merge1Result[i+1].varId << " " << c1.varId << " 0" << std::endl;
        result << "-" << Merge2Result[i].varId   << " " << c1.varId << " 0" << std::endl;
        result << "" << Merge1Result[i+1].varId << " -" << c2.varId << " 0" << std::endl;
        result << "" << Merge2Result[i].varId   << " -" << c2.varId << " 0" << std::endl;
        result << ""  << Merge2Result[i].varId << " " << Merge1Result[i+1].varId << " -" << c1.varId << " 0" << std::endl;
        resultingVars.push_back(c1);
        resultingVars.push_back(c2);
        countClausel += 6;
    }
    resultingVars.push_back(Merge2Result[Merge2Result.size()-1]);
}

/**
 create the half Sorting Network
 Size of input var is 2^n. In Paper stands 2n, but it is wrong I think
 * @brief Solver::toSatHalf::createHalfSortingNetwork
 * @param inputVars
 * @param outputVars
 */

void Solver::toSatHalf::createHalfSortingNetwork(const std::vector<Solver::literal>& inputVars,
                                                       std::vector<Solver::literal>& outputVars){
#if COMMENTSDIMACS == 1
    result << "c" << std::endl;
    result << "c Create Half Sorting Network Size: " << inputVars.size() << std::endl;
    result << "c Vars: ";
    for(uint i=0; i< inputVars.size(); ++i){
        result << inputVars[i].varId << " ";
    }
    result << std::endl;
    result << "c" << std::endl;
#endif
//    assert(inputVars.size() == 0);

// 3. Case 2 Elements
    if(inputVars.size() == 2){
        std::vector<Solver::literal> set1, set2;
        set1.push_back(inputVars[0]);
        set2.push_back(inputVars[1]);
        createHalfMergingNetwork(set1, set2, outputVars);
        return;
    }
// 4. Case > 2 Elements
    std::vector<Solver::literal> set1, set2;
    std::vector<Solver::literal> resultSet1, resultSet2;
    for(uint i=0;i< inputVars.size()/2; ++i){
        set1.push_back(inputVars[i]);
    }
    for(uint i=inputVars.size()/2; i<inputVars.size(); ++i){
        set2.push_back(inputVars[i]);
    }
    createHalfSortingNetwork(set1, resultSet1);
    createHalfSortingNetwork(set2, resultSet2);
    createHalfMergingNetwork(resultSet1, resultSet2, outputVars);
}
