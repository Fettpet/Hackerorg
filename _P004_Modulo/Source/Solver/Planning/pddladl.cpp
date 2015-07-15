#include "pddladl.h"
#include <fstream>

/**************************************************************************************************
 *                                      Constructor
 * ***********************************************************************************************/
/**
  Constructor
 * @brief Solver::PddlADL::PddlADL
 * @param _game The game what you like to solve
 * @param _solver The solver you like to work
 */
Solver::PddlADL::PddlADL(const Game::Game& _game, const std::string _solver):
    Solver::PddlBaseClass(_game, _solver)
{
}

/**************************************************************************************************
 *                                      Communication with bfs
 * ***********************************************************************************************/
/**
 * @brief Solver::PddlADL::startBFS
 * @param problem: Filename for Problem
 * @param domain: Filename for Domain
 * @param output: Filename for calculatet plan
 * @param solver: Path to the solver
 */
void Solver::PddlADL::startBFS(const std::string& problem, const std::string& domain, const std::string& output, const std::string& solver){
    std::stringstream str;
    str << solver << " --domain " << domain << " --problem " << problem << " --output " << output << " > /tmp/solverTalk.txt";
    std::cout << str.str() << std::endl;
    std::system(str.str().c_str());
}


void Solver::PddlADL::readSolutionBFS(const std::string& solutionFile){
    std::fstream data;
    data.open(solutionFile, std::ios::in);
    while(!data.eof()){
        std::string buffer, tile, posX, posY;
        std::getline(data, buffer);
        if(buffer.empty()) break;
        tile = buffer.substr(buffer.find("_T")+2, buffer.find("_X")- buffer.find("_T")-2);
        posX = buffer.substr(buffer.find("_X")+2, buffer.find("_Y")- buffer.find("_X")-2);
        posY = buffer.substr(buffer.find("_Y")+2, buffer.find(" ")- buffer.find("_Y")-2);
        gameField.getTileById(std::atoi(tile.c_str())).setSolutionX(std::atoi(posX.c_str()));
        gameField.getTileById(std::atoi(tile.c_str())).setSolutionY(std::atoi(posY.c_str()));
    }
}

/**************************************************************************************************
                            virutal functions
 * ***********************************************************************************************/

void Solver::PddlADL::startSolver(const std::string &domainFile, const std::string &problemFile, const std::string &solutionFile){
    startBFS(problemFile, domainFile, solutionFile, solver);
}

void Solver::PddlADL::readSolution(const std::string &solutionFile){
    readSolutionBFS(solutionFile);
}

/**
  create a domain file for the Problem
 * @brief Solver::PddlADL::createDomainFile
 * @param domainFileName: Name of the domainfile
 */
void Solver::PddlADL::createDomainFile(const std::string &domainFileName){
    std::fstream data;
    uint sizeX(gameField.getField().getSizeX());
    uint sizeY(gameField.getField().getSizeY());
    data.open(domainFileName, std::ios::out);
    data << "(define (domain Modulo-adl)" << std::endl;
// requirements
    data << "   (:requirements :adl :typing)" << std::endl;
// Types
    data << "   (:types Cell Tile)" << std::endl;
// Prädikate
    data << "   (:predicates" << std::endl;
// Prädikate für Farben
    for(uint i=0; i< gameField.getField().getMod(); ++i){
        data << "       (hasColor" << i << " ?c - Cell)" << std::endl;
    }
     for(const Tile& t: gameField.getTiles()){
        data << "       (isTile" << t.getId() << " ?c - Tile)" << std::endl;
    }
// Prädikate für Positionen
    for(uint posX=0; posX < gameField.getField().getSizeX(); ++posX){
        for(uint posY=0; posY < gameField.getField().getSizeY(); ++posY){
            data << "       (isPosition_X" << posX << "_Y" << posY << " ?c - Cell)" << std::endl;
        }
    }
    data << "       (wasUsed ?t - Tile)" << std::endl;
    data << "   )"<< std::endl;

// Actions
    for(const Tile& t: gameField.getTiles()){

        for(uint fieldX = 0; fieldX < sizeX; ++fieldX){
            for(uint fieldY = 0; fieldY < sizeY; ++fieldY){
                if(t.getSizeX() + fieldX > sizeX) break;
                if(t.getSizeY() + fieldY > sizeY) break;
                data << "   (:action setTile_T" << t.getId() << "_X" << fieldX << "_Y" << fieldY << std::endl;
// Create Parameter List
                data << "       :parameters (?used - Tile ";
                for(uint tileX=0; tileX < t.getSizeX(); ++tileX){
                  //  if(fieldX + tileX > sizeX) break;
                    for(uint tileY=0; tileY<t.getSizeY(); ++tileY){
                        if(!t.getField(tileX, tileY)) continue;
                    //    if(tileY + t.getSizeY() > sizeY) break;
                        data << "?Cell_X" << fieldX + tileX << "_Y" << fieldY + tileY << " - Cell ";
                    }
                }
                data << ")" << std::endl;
// Create Precondition List
                data << "       :precondition (and (not (wasUsed ?used))"<< std::endl;
                data << "                          (isTile" << t.getId() << " ?used)" << std::endl;
                for(uint tileX=0; tileX < t.getSizeX(); ++tileX){
                  //  if(fieldX + tileX > sizeX) break;
                    for(uint tileY=0; tileY<t.getSizeY(); ++tileY){
                        if(t.getField(tileX,tileY)){
                            data << "                          (isPosition_X" << fieldX + tileX << "_Y" << fieldY + tileY << " "
                                 << "?Cell_X" << fieldX + tileX << "_Y" << fieldY + tileY << ")" << std::endl;
                        }
                    }
                }
                data <<"        )" << std::endl;

// Create Effect liste
                data << "       :effect (and (wasUsed ?used)" << std::endl;
// Für jede Farbe

// Für jede Zelle
                for(uint tileX=0; tileX < t.getSizeX(); ++tileX){
                    for(uint tileY=0; tileY<t.getSizeY(); ++tileY){
                        if(!t.getField(tileX, tileY)) continue;
                        for(int color=0; color<gameField.getField().getMod()-1; ++color){
                            data << "           (when (hasColor" << color << " ?Cell_X" << fieldX + tileX << "_Y" << fieldY + tileY << ")" << std::endl;
                            data << "               (and (not (hasColor" << color << " ?Cell_X" << fieldX + tileX << "_Y" << fieldY + tileY << "))" << std::endl;
                            data << "                    (hasColor" << color+1 << " ?Cell_X" << fieldX + tileX << "_Y" << fieldY + tileY << ")" << std::endl;
                            data << "               )" << std::endl;
                            data << "           )" << std::endl;
                        }
// Zum schließen des Rings
                        data << "           (when (hasColor" << gameField.getField().getMod() -1 << " ?Cell_X" << fieldX + tileX << "_Y" << fieldY + tileY << ")" << std::endl;
                        data << "               (and (not (hasColor" <<  gameField.getField().getMod() -1 << " ?Cell_X" << fieldX + tileX << "_Y" << fieldY + tileY << "))" << std::endl;
                        data << "                    (hasColor" << 0 << " ?Cell_X" << fieldX + tileX << "_Y" << fieldY + tileY << ")" << std::endl;
                        data << "               )" << std::endl;
                        data << "           )" << std::endl;
                    }
                }
                data << "       )" << std::endl;
                data << "   )" <<std::endl;
            }
        }

    }
    data << ")";
    data.close();
}


/**
  create a  PDDL Problem File for the current  game
 * @brief createProblemFile
 * @param outFileName: Name of the file
*/
void Solver::PddlADL::createProblemFile(const std::string& outFileName){
    std::fstream data;
    data.open(outFileName, std::ios::out);
    data << "(define (problem modulo-adl)" << std::endl;
    data << "   (:domain Modulo-adl)" << std::endl;
    data << "   (:objects" << std::endl;
    data << "       ";
    for(const Tile& t: gameField.getTiles()){
        data << "Tile" << t.getId() << " ";
    }
    data << " - Tile" << std::endl;

    data << "       ";
    for(uint x=0; x<gameField.getField().getSizeX(); ++x){
        for(uint y=0; y<gameField.getField().getSizeY(); ++y){
            data << "Cell_X" << x << "_Y" << y << " ";
        }
    }
    data << " - Cell" << std::endl;
    data << "   )" << std::endl;
// Init
// Sagen das das Tile eine ID hat
    data << "   (:init" << std::endl;
    for(const Tile& t1: gameField.getTiles()){
        for(const Tile& t2: gameField.getTiles()){
            if(t1.getId() == t2.getId()){
                data << "       (isTile" << t1.getId() << " Tile" << t2.getId() << ")" << std::endl;
            } else {
        //        data << "       (not (isTile" << t1.getId() << " Tile" << t2.getId() << "))" << std::endl;
            }
        }
    }
// Sagen das die Positionen eine Position haben
    for(uint posX=0; posX < gameField.getField().getSizeX(); ++posX){
        for(uint posY=0; posY < gameField.getField().getSizeY(); ++posY){
            data << "       ";
            for(uint tileX=0; tileX < gameField.getField().getSizeX(); ++tileX){
                for(uint tileY=0; tileY < gameField.getField().getSizeY(); ++tileY){
                    if((posX == tileX) && (posY == tileY)){
                        data << "(isPosition_X" << posX << "_Y" << posY;
                        data << " Cell_X" << tileX << "_Y" << tileY << ") ";
                    } else {
               //         data << "(not (isPosition_X" << posX << "_Y" << posY;
               //         data << " Cell_X" << tileX << "_Y" << tileY << ")) ";
                    }
                }
            }
            data << std::endl;
        }
    }
// Alle Tiles auf unbenutzt setzen
    for(const Tile& t1: gameField.getTiles()){
        data << "       (not (wasUsed Tile" << t1.getId() << "))" << std::endl;
    }
// ALle Farben setzten
    for(uint posX=0; posX < gameField.getField().getSizeX(); ++posX){
        for(uint posY=0; posY < gameField.getField().getSizeY(); ++posY){
            data << "       ";
            for(uint color=0; color < gameField.getField().getMod(); ++color){
                if(gameField.getField().getField(posX, posY) == color){
                    data << "(hasColor" << color << " Cell_X" << posX << "_Y" << posY << ") ";
                } else {
              //      data << "(not (hasColor" << color << " Cell_X" << posX << "_Y" << posY << ")) ";
                }
            }
            data << std::endl;
        }
    }
    data << "   )" << std::endl;

// Goal
    data << "   (:goal (and" << std::endl;
// Alle Tiles werden benutzt
    for(const Tile& t: gameField.getTiles()){
        data <<  "              (wasUsed Tile" <<t.getId() << ")" << std::endl;
    }
// Alle Zellen haben farbe 0
    for(uint posX=0; posX < gameField.getField().getSizeX(); ++posX){
        for(uint posY=0; posY < gameField.getField().getSizeY(); ++posY){
            data << "              (hasColor0 Cell_X" << posX << "_Y" << posY << ")" << std::endl;
        }
    }
    data << "           )" << std::endl;
    data << "   )" << std::endl;

    data << ")";
}
