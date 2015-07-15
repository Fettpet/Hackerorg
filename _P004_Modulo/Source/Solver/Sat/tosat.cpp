#include "tosat.h"
#include "../../Exceptions/stringexception.h"
#include "../../Defines/compileoptions.h"
#include <fstream>
#include <assert.h>
/**************************************************************************************************
 *                              DIMACS und Modulo n
 * ***********************************************************************************************/
void Solver::tosat::createModuloNAdder(const std::vector<Solver::literal>& inputVars, const uint Modulo){
    std::vector<Solver::literal> sortVars;
    std::stringstream streamBuffer;
// 1 Case no input, no Adder
    if(inputVars.size() == 0) return;


// Sort network
    createSortingNetwork(inputVars, sortVars);

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
create a Dimacs file from current gamefield and write it to file fileName
 * @brief Solver::tosat::createDimacs
 * @param fileName:
*/

void Solver::tosat::createDimacs(const std::string& fileName){
        result.str("");
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
                    if(!bufferOr.empty()){
                        Solver::literal lit;
                        lit.varId =createTseitinOr(bufferOr); lit.positiv = true;
                        bufferForVar.push_back(lit);
                    }
    // Set start value

                }
                if(gameField.getField().getField(fieldX,fieldY) != 0){
                    for(int i=0; i <gameField.getField().getField(fieldX,fieldY); ++i ){
                     //   std::cout << "Add: (" << fieldX << ", " << fieldY << ") " << i << " " << (int)gameField.getField().getField(fieldX, fieldY) << std::endl;
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
                createModuloNAdder(bufferForVar, gameField.getField().getMod());
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
 *                              Sorting Network
 * ***********************************************************************************************/
/**
Sort the outputvars
 * @brief Solver::tosat::createSortingNetwork
 * @param inputVars
 * @param outputVars
*/
void Solver::tosat::createSortingNetwork(const std::vector<Solver::literal>& inputVars,
                                               std::vector<Solver::literal>& outputVars){

    if(inputVars.size() == 1){
        outputVars = inputVars;
    }else if(inputVars.size() == 2){
        mergeA1B1(inputVars[0],inputVars[1], outputVars);
    } else{
#if COMMENTSDIMACS == 1
        result << "c" << std::endl;
        result << "c create Sorting Network with " << inputVars.size() << " Elements" << std::endl;
        result << "c";
        for(const Solver::literal& t: inputVars){
            result << t.varId << " ";
        }
        result << std::endl;
        result << "c" << std::endl;
#endif
        uint l=inputVars.size() / 2;
        std::vector<Solver::literal> input1, input2;
        std::vector<Solver::literal> output1, output2;
        for(uint i=0; i<l; ++i){
            input1.push_back(inputVars[i]);
        }
        for(uint i=l; i<inputVars.size(); ++i){
            input2.push_back(inputVars[i]);
        }
        createSortingNetwork(input1, output1);
        createSortingNetwork(input2, output2);
        createMergingNetwork(output1, output2, outputVars);

#if COMMENTSDIMACS == 1
        result << "c" << std::endl;
        result << "c End of creating Sorting Network with " << inputVars.size() << " Elements" << std::endl;
        result << "c input: ";
        for(const Solver::literal& t: inputVars){
            result << t.varId << " ";
        }
        result << std::endl;
        result << "c output: ";
        for(const Solver::literal& t: outputVars){
            result << t.varId << " ";
        }
        result << std::endl;
        result << "c output1: ";
        for(const Solver::literal& t: output1){
            result << t.varId << " ";
        }
        result << std::endl;
        result << "c output2: ";
        for(const Solver::literal& t: output2){
            result << t.varId << " ";
        }
        result << std::endl;
        result << "c" << std::endl;
#endif
    }
}

/**************************************************************************************************
 *                              Merging Network
 * ***********************************************************************************************/

void Solver::tosat::createMergingNetwork(const std::vector<Solver::literal>& setOne,
                          const std::vector<Solver::literal>& setTwo,
                                std::vector<Solver::literal>& resultingVars){
// 1. Case setOne == setTwo == 1
    if((setOne.size() == 1) && (setTwo.size() == 1)){
        resultingVars.clear();
        mergeA1B1(setOne, setTwo, resultingVars);

// 2 Case one of the sets is Empty
    } else if((setOne.size() == 0) || (setTwo.size() == 0)){
        resultingVars.clear();
        mergeOneEmpty(setOne, setTwo, resultingVars);
// 3. Case Both are even
    } else if((setOne.size() % 2 == 0) && (setTwo.size() % 2 == 0)){
        resultingVars.clear();
        mergeManyBothEven(setOne, setTwo, resultingVars);
// 4. Case One is even, one is odd
    } else if((setOne.size() % 2) + (setTwo.size() % 2) == 1){
        resultingVars.clear();
        mergeManyOneEven(setOne, setTwo, resultingVars);
// 5. Case both odd
    } else if((setOne.size() % 2 == 1) && (setTwo.size() % 2 == 1)){
        resultingVars.clear();
        mergeManyBothOdd(setOne, setTwo, resultingVars);
    }
#if COMMENTSDIMACS == 1
        result << "c" << std::endl;
        result << "c End of creating Merging Network with Elements" << std::endl;
        result << "c input Set 1 : ";
        for(const Solver::literal& t: setOne){
            result << t.varId << " ";
        }
        result << std::endl;
        result << "c input Set 2 : ";
        for(const Solver::literal& t: setTwo){
            result << t.varId << " ";
        }
        result << std::endl;
        result << "c input Resulting : ";
        for(const Solver::literal& t: resultingVars){
            result << t.varId << " ";
        }
        result << std::endl;
        result << "c" << std::endl;
#endif

}

/**
  Create a Merging Network. Each of the 2 Input Set has only one Element.
 * @brief Solver::tosat::mergeA1B1
 * @param setOne first set of literal. Only 1 Element
 * @param setTwo second set of literal. Only 1 Element
 * @param outputVars new vars they stand for output
 */
void Solver::tosat::mergeA1B1(const std::vector<Solver::literal>& setOne,
                              const std::vector<Solver::literal>& setTwo,
                                    std::vector<Solver::literal>& outputVars){
    assert(setOne.size() == 1);
    assert(setTwo.size() == 1);
    Solver::literal varC1;
    varC1.varId = ++countVars; varC1.positiv = true;
    Solver::literal varC2;
    varC2.varId = ++countVars; varC2.positiv = true;
#if COMMENTSDIMACS == 1
    result << "c " << std::endl;
    result << "c Create Merger with 2 set with 1 Element" << std::endl;
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
    outputVars.push_back(varC1);
    outputVars.push_back(varC2);
}

void Solver::tosat::mergeA1B1(const Solver::literal& setOne,
                              const Solver::literal& setTwo,
                                    std::vector<Solver::literal>& outputVars){
    if(setTwo.varId > countVars){
        throw Exception::StringException("HIER");

    }
    Solver::literal varC1;
    varC1.varId = ++countVars; varC1.positiv = true;
    Solver::literal varC2;
    varC2.varId = ++countVars; varC2.positiv = true;
#if COMMENTSDIMACS == 1
    result << "c " << std::endl;
    result << "c Create Merger with 2 set with 1 Element Direkt" << std::endl;
    result << "c Vars Merge: " << setOne.varId << " " << setTwo.varId << std::endl;
    result << "c New Vars: " << varC1.varId << " " << varC2.varId << std::endl;
    result << "c" << std::endl;
#endif
    result << "-" << setOne.varId << " -" << setTwo.varId << " " << varC2.varId << " 0" << std::endl;
    result << "-" << setOne.varId << " " << varC1.varId << " 0" << std::endl;
    result << "-" << setTwo.varId << " " << varC1.varId << " 0" << std::endl;
    result << ""  << setOne.varId << " -" << varC2.varId << " 0" << std::endl;
    result << ""  << setTwo.varId << " -" << varC2.varId << " 0" << std::endl;
    result << ""  << setOne.varId << " " << setTwo.varId << " -" << varC1.varId << " 0" << std::endl;
    countClausel += 6;
    outputVars.push_back(varC1);
    outputVars.push_back(varC2);

}

/**
Merge 1 set. oe of them is empty.
 * @brief Solver::tosat::mergeOneEmpty
 * @param input1
 * @param input2
 * @param outputVars
*/
void Solver::tosat::mergeOneEmpty(const std::vector<Solver::literal>& input1,
                                  const std::vector<Solver::literal>& input2,
                                        std::vector<Solver::literal>& outputVars){
#if COMMENTSDIMACS == 1
        result << "c" << std::endl;
        result << "c Merge Network with One is empty" << std::endl;
        result << "c" << std::endl;
#endif
    if(input1.empty()){
        if(input2.empty()){
            outputVars = input2;
        } else {
            throw Exception::StringException("Both Sets are empty");
        }
    }else if(input2.empty()){
        outputVars = input1;
    } else {
        throw Exception::StringException("non set is empty");
    }
}

/**
Preconditions
|input1| > 0
|input2| > 0
|input1| is even
|input2| is even

 * @brief mergeManyBothEven
 * @param input1
 * @param input2
 * @param outputVars
 */
void Solver::tosat::mergeManyBothEven(const std::vector<Solver::literal>& input1,
                       const std::vector<Solver::literal>& input2,
                             std::vector<Solver::literal>& outputVars){
// Precondition test
    assert(input1.size() > 0);
    assert(input2.size() > 0);
    assert(input1.size() % 2 == 0);
    assert(input2.size() % 2 == 0);
#if COMMENTSDIMACS == 1
        result << "c" << std::endl;
        result << "c merge Network with " << input1.size() << " " << input2.size() << " Elements both even" << std::endl;
        result << "c ";
        for(Solver::literal l: input1){
            result << l.varId << " ";
        }
        result << std::endl;
        result << "c ";
        for(Solver::literal l: input2){
            result << l.varId << " ";
        }
        result << std::endl;
        result << "c" << std::endl;
#endif
    std::vector<Solver::literal> mergeSetEven1, mergeSetEven2;
    std::vector<Solver::literal> mergeSetOdd1, mergeSetOdd2;
    std::vector<Solver::literal> mergingResult1, mergingResult2;
// Fill the 4 mergeSets
    for(uint i=0; i<input1.size(); i+=2){
        mergeSetEven1.push_back(input1[i]);
    }
    for(uint i=1; i<input1.size(); i+=2){
        mergeSetEven2.push_back(input1[i]);
    }
    for(uint i=0; i<input2.size(); i+=2){
        mergeSetOdd1.push_back(input2[i]);
    }
    for(uint i=1; i<input2.size(); i+=2){
        mergeSetOdd2.push_back(input2[i]);
    }
// merging
    createMergingNetwork(mergeSetEven1, mergeSetOdd1, mergingResult1);
    createMergingNetwork(mergeSetEven2, mergeSetOdd2, mergingResult2);

// create outputvars
    outputVars.push_back(mergingResult1[0]);
    uint minValue = mergingResult1.size();
    for(uint i=1; i<minValue; ++i){
        std::vector<Solver::literal> bufferSet;
        if(mergingResult2.size() > i-1){
            mergeA1B1(mergingResult1[i], mergingResult2[i-1], bufferSet);
            outputVars.push_back(bufferSet[0]);
            outputVars.push_back(bufferSet[1]);
        } else{
            outputVars.push_back(mergingResult1[i]);
        }

    }
    outputVars.push_back(mergingResult2[mergingResult2.size()-1]);
}

/**
Merge 2 Sets of Vars.
Precondition:
The cardinality of one of the set is even, the other is odd.
Both sets have more then 0 Elements. One Set has more than 1 Element
 * @brief Solver::tosat::mergeManyOneEven
 * @param input1
 * @param input2
 * @param outputVars
 */
void Solver::tosat::mergeManyOneEven(const std::vector<Solver::literal>& input1,
                                     const std::vector<Solver::literal>& input2,
                                           std::vector<Solver::literal>& outputVars){

// Precondition Test
    std::vector<Solver::literal> mergingSetEven, mergingSetOdd;
    if((input1.size() % 2 == 0) && (input2.size() % 2 == 1)){
        mergingSetEven = input1;
        mergingSetOdd = input2;
    } else if((input1.size() % 2 == 1) && (input2.size() % 2 == 0)){
        mergingSetEven = input2;
        mergingSetOdd = input1;
    } else {
        Exception::StringException("one is odd, one is even isnt successfull testet");
    }
    assert(input1.size() > 0);
    assert(input2.size() > 0);
#if COMMENTSDIMACS == 1
        result << "c" << std::endl;
        result << "c merge Network with " << input1.size() << " " << input2.size() << " Elements one is even, one odd" << std::endl;
        result << "c ";
        for(Solver::literal l: input1){
            result << l.varId << " ";
        }
        result << std::endl;
        result << "c ";
        for(Solver::literal l: input2){
            result << l.varId << " ";
        }
        result << std::endl;
        result << "c" << std::endl;
#endif

    std::vector<Solver::literal> mergeSetEven1, mergeSetEven2;
    std::vector<Solver::literal> mergeSetOdd1, mergeSetOdd2;
    std::vector<Solver::literal> mergingResult1, mergingResult2;
// Fill the 4 mergeSets
    for(uint i=0; i<mergingSetEven.size(); i+=2){
        mergeSetEven1.push_back(mergingSetEven[i]);
    }
    for(uint i=1; i<mergingSetEven.size(); i+=2){
        mergeSetEven2.push_back(mergingSetEven[i]);
    }

    for(uint i=0; i<mergingSetOdd.size(); i+=2){
        mergeSetOdd1.push_back(mergingSetOdd[i]);
    }
    for(uint i=1; i<mergingSetOdd.size(); i+=2){
        mergeSetOdd2.push_back(mergingSetOdd[i]);
    }
// Merging
    createMergingNetwork(mergeSetEven1, mergeSetOdd1, mergingResult1);
    createMergingNetwork(mergeSetEven2, mergeSetOdd2, mergingResult2);
// create Output vars
    uint minValue = mergingResult1.size();
    outputVars.push_back(mergingResult1[0]);
    for(uint i=1; i<minValue; ++i){
        std::vector<Solver::literal> bufferSet;
        if(mergingResult2.size() > i-1){
            mergeA1B1(mergingResult1[i], mergingResult2[i-1], bufferSet);
            outputVars.push_back(bufferSet[0]);
            outputVars.push_back(bufferSet[1]);
        }else {
            outputVars.push_back(mergingResult1[i]);
        }
    }
}

/**
  Create a Merger
Preconditions
|input1| > 0
|input2| > 0
|input1| is odd
|input2| is odd
|input1| + |input1| > 3
 * @brief Solver::tosat::mergeManyBothOdd
 * @param input1
 * @param input2
 * @param outputVars
 */
void Solver::tosat::mergeManyBothOdd(const std::vector<Solver::literal>& input1,
                      const std::vector<Solver::literal>& input2,
                            std::vector<Solver::literal>& outputVars){
// Precondition test
    assert(input1.size() > 0);
    assert(input2.size() > 0);
    assert(input1.size() % 2 == 1);
    assert(input2.size() % 2 == 1);
    assert(input1.size() + input2.size() > 3);

#if COMMENTSDIMACS == 1
    result << "c" << std::endl;
    result << "c merge Network with " << input1.size() << " " << input2.size() << " Elements mergeManyBothOdd" << std::endl;
    result << "c ";
    for(Solver::literal l: input1){
        result << l.varId << " ";
    }
    result << std::endl;
    result << "c ";
    for(Solver::literal l: input2){
        result << l.varId << " ";
    }
    result << std::endl;
    result << "c" << std::endl;
#endif

    std::vector<Solver::literal> mergeSetEven1, mergeSetEven2;
    std::vector<Solver::literal> mergeSetOdd1, mergeSetOdd2;
    std::vector<Solver::literal> mergingResult1, mergingResult2;
// Fill the 4 mergeSets
    for(uint i=0; i<input1.size(); i+=2){
        mergeSetEven1.push_back(input1[i]);
    }
    for(uint i=1; i<input1.size(); i+=2){
        mergeSetOdd1.push_back(input1[i]);
    }
    for(uint i=0; i<input2.size(); i+=2){
        mergeSetEven2.push_back(input2[i]);
    }
    for(uint i=1; i<input2.size(); i+=2){
        mergeSetOdd2.push_back(input2[i]);
    }
// merging
    createMergingNetwork(mergeSetEven1, mergeSetEven2, mergingResult1);
    createMergingNetwork(mergeSetOdd1, mergeSetOdd2, mergingResult2);

// create outputvars
    outputVars.push_back(mergingResult1[0]);
    uint minValue = mergingResult1.size();
    for(uint i=1; i<minValue; ++i){
        std::vector<Solver::literal> bufferSet;
        if(mergingResult2.size() > i-1){
            mergeA1B1(mergingResult1[i], mergingResult2[i-1], bufferSet);
            outputVars.push_back(bufferSet[0]);
            outputVars.push_back(bufferSet[1]);
        }else{
            outputVars.push_back(mergingResult1[i]);
        }
    }
    //outputVars.push_back(mergingResult2[mergingResult2.size()-1]);
}
