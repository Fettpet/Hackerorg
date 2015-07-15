#include "tosatdebug.h"


/**************************************************************************************************
 *                                          Dimacs
 * ***********************************************************************************************/

void Solver::toSatDebug::createDimacs(const std::string& fileName){
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
                    Solver::literal lit;
                    lit.varId =createTseitinOr(bufferOr); lit.positiv = true;
                    bufferForVar.push_back(lit);
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
#if COMMENTSDEBUG == 1
                result << "c" << std::endl;
                result << "c Decode at Position (" << fieldX << ", " << fieldY << ")" << std::endl;
                result << "c" << std::endl;
#endif
                createModuloNAdderHalf(bufferForVar, gameField.getField().getMod());
            }
        }





/**************************************************************************************************
 *                          Debug
 *
 * ***********************************************************************************************/
    uint fieldX = 0;
    uint fieldY = 0;
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
        Solver::literal lit;
        lit.varId =createTseitinOr(bufferOr); lit.positiv = true;
        bufferForVar.push_back(lit);
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
#if COMMENTSDEBUG == 1
                result << "c" << std::endl;
                result << "c Decode at Position (" << fieldX << ", " << fieldY << ")" << std::endl;
                result << "c" << std::endl;
#endif
                createModuloNAdderHalf(bufferForVar, gameField.getField().getMod());

//*/
    // write it into file
        std::fstream data;
        data.open(fileName, std::ios::out);
        data << "p cnf " << countVars << " " << countClausel << std::endl;
        data << result.str().c_str();
        data.close();

}


Game::Game Solver::toSatDebug::getSolution(){
    std::stringstream out;
    boost::posix_time::ptime start, end;
    boost::posix_time::time_duration diff;
    start = boost::posix_time::microsec_clock::local_time();
    createDimacs("/tmp/dimacs.txt");
    end  = boost::posix_time::microsec_clock::local_time();
    diff = end-start;
    neededTimeForCreateDimacs = diff.total_milliseconds();

   // satSolver = "./SparrowToRiss.sh";
    out << satSolver << " /tmp/dimacs.txt >> /tmp/ausg.txt";
    std::system("rm /tmp/ausg.txt");

    start = boost::posix_time::microsec_clock::local_time();
    std::system(out.str().c_str());
    end  = boost::posix_time::microsec_clock::local_time();
    diff = end-start;
    neededTimeForSatsolving = diff.total_milliseconds();
    readSolution("/tmp/ausg.txt",satSolver);
    return gameField;
}
