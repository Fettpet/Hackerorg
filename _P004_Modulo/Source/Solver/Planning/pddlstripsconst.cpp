#include "pddlstripsconst.h"
#include <fstream>
#include <sstream>
/**************************************************************************************************
 *                                  Constructor
 * ***********************************************************************************************/
Solver::PddlStripsConst::PddlStripsConst(const Game::Game& t, const std::string& sol):
    Solver::PddlBaseClass(t, sol)
{}

/**************************************************************************************************
 *                          virtual functions
 * ***********************************************************************************************/
/**
 * @brief Solver::PddlStripsConst::createProblemFile
 * @param outFileName
 */
void Solver::PddlStripsConst::createProblemFile(const std::string& outFileName){
    std::fstream data;
    uint sizeX(gameField.getField().getSizeX());
    uint sizeY(gameField.getField().getSizeY());
    data.open(outFileName, std::ios::out);
    data << "(define (problem modulo-strips-const)" << std::endl;
    data << "   (:domain Modulo-Strips-Const)" << std::endl;
    data << "   (:objects" << std::endl;
    data << "   )" << std::endl;

    data << "   (:init" << std::endl;
// Sagen das die Tiles unbenutzt sind
    for(const Tile& t1: gameField.getTiles()){
        data << "       (notUsed" << " Tile" << t1.getId() << ")" << std::endl;
    }
// Sagen das die Tiles Positionen unbenutzt sind
    for(const Tile& t: gameField.getTiles())
        for(uint x=0; x<t.getSizeX(); ++x)
            for(uint y=0; y<t.getSizeY(); ++y){
                if(t.getField(x,y))
                    data << "       (notUsed TileCell_T" << t.getId() <<"_X" << x << "_Y" << y << ")" << std::endl;
            }

// Den Zellen Farbe geben
    for(uint x=0; x<sizeX; ++x)
        for(uint y=0; y<sizeY; ++y){
            data << "       (hasColor" << (int)gameField.getField().getField(x,y) << " Cell_X" << x << "_Y" << y <<")" << std::endl;
        }
    data << "   )" << std::endl;

// Zielzustand
    data << "   (:goal (and" << std::endl;
//1. Farbe
    for(uint x=0; x<sizeX; ++x)
        for(uint y=0; y<sizeY; ++y){
            data << "       (hasColor0 Cell_X" << x << "_Y" << y <<")" << std::endl;
        }
// 2. Alle Tilezellen wurden benutzt
    for(const Tile& t: gameField.getTiles())
        for(uint x=0; x<t.getSizeX(); ++x)
            for(uint y=0; y<t.getSizeY(); ++y){
                if(t.getField(x,y))
                    data << "       (wasUsed TileCell_T" << t.getId() <<"_X" << x << "_Y" << y << ")" << std::endl;
            }
// 3. Alle Tiles wurden benutzt
    for(const Tile& t: gameField.getTiles()){
         data << "       (wasUsed Tile" << t.getId() << ")" << std::endl;
    }
    data << "   ))" << std::endl;
    data << ")"<< std::endl;
    data.close();
}

/**
 * @brief Solver::PddlStripsConst::createDomainFile
 * @param outFileName
 */
void Solver::PddlStripsConst::createDomainFile(const std::string& domainFileName){
    std::fstream data;
    uint sizeX(gameField.getField().getSizeX());
    uint sizeY(gameField.getField().getSizeY());
    data.open(domainFileName, std::ios::out);
    data << "(define (domain Modulo-Strips-Const)" << std::endl;
// requirements
    data << "   (:requirements :strips)" << std::endl;
// Prädikate
    data << "   (:predicates" << std::endl;

    data << "       (notUsed ?t)" << std::endl;
    data << "       (wasUsed ?t)" << std::endl;
// Prädikate für Farben
    for(uint i=0; i< gameField.getField().getMod(); ++i){
        data << "       (hasColor" << i << " ?c)" << std::endl;
    }

// Prädikate für das Starte
     for(uint fieldX = 0; fieldX < sizeX; ++fieldX){
        for(uint fieldY = 0; fieldY < sizeY; ++fieldY){
            data << "       (TileStartAtPos_X" << fieldX << "_Y" << fieldY << " ?t)" << std::endl;
        }
    }

// Prädikate für Setzungen
// Tile_T_isSetTo_X_Y_Cell_X_Y
    data << "   )" << std::endl;

/************************************************
 *                  Constants
 * *********************************************/
// Gamefield Zellen
    data << "   (:constants" << std::endl;// Cell_X0_Y0 Cell_X0_Y1 Cell_X1_Y0 Cell_X1_Y1 Tile1)
    for(uint fieldX = 0; fieldX < sizeX; ++fieldX){
        for(uint fieldY = 0; fieldY < sizeY; ++fieldY){
            data << "       Cell_X" << fieldX << "_Y" << fieldY << std::endl;
        }
    }
// Tiles
    for(const Tile& t: gameField.getTiles()){
        data << "       Tile" << t.getId() << std::endl;
    }
// Tile Zellen
    for(const Tile& t: gameField.getTiles()){
        for(uint x=0; x<t.getSizeX(); ++x)
            for(uint y=0; y<t.getSizeY(); ++y){
                data << "       TileCell_T" << t.getId() << "_X" << x << "_Y" << y << std::endl;
            }
    }
    data << "   )" << std::endl;
/**************
 *  Actions
 * *************/
// 1.Action start
        for(const Tile& t: gameField.getTiles()){
            for(uint fieldX = 0; fieldX < sizeX; ++fieldX){
                for(uint fieldY = 0; fieldY < sizeY; ++fieldY){
                    if(t.getSizeX() + fieldX > sizeX) break;
                    if(t.getSizeY() + fieldY > sizeY) break;
                    data << "   (:action setTile_T" << t.getId() << "_X" << fieldX << "_Y" << fieldY << std::endl;
    // Create Parameter List
                    data << "       :parameters ()" << std::endl;

    // Create Precondition List
                    data << "       :precondition (and (notUsed Tile" << t.getId() << ")" << std::endl;
                    data << "                     )" << std::endl;
    // Create Effect liste
                    data << "       :effect (and (wasUsed Tile" << t.getId() << ")" << std::endl;
                    data << "                    (TileStartAtPos_X" << fieldX << "_Y" << fieldY <<" Tile" << t.getId() <<")" << std::endl;
                    data << "                    (not (notUsed Tile" << t.getId() << "))" << std::endl;
                    data << "               )" << std::endl;
                    data << "   )" << std::endl;
    // Für jede Farbe
                }
            }
        }
/* 2. Action recolor
*/
    for(const Tile& t: gameField.getTiles()){
        for(uint fieldX = 0; fieldX < sizeX; ++fieldX){
            for(uint fieldY = 0; fieldY < sizeY; ++fieldY){
                if(t.getSizeX() + fieldX > sizeX) break;
                if(t.getSizeY() + fieldY > sizeY) break;
                for(uint tileX=0; tileX < t.getSizeX(); ++tileX){
                    for(uint tileY=0; tileY < t.getSizeY(); ++tileY){
                        for(uint color=0; color<gameField.getField().getMod()-1; ++color){
// Action
                            data << "   (:action chanceColor" << color << "_T" << t.getId() << "_X" << fieldX << "_Y" << fieldY
                                 << "_TX" << tileX << "_TY" << tileY << std::endl;
// Parameter
                            data << "       :parameters()"  << std::endl;
// Precondition
// Tile
                            data << "       :precondition (and (TileStartAtPos_X" << fieldX << "_Y" << fieldY << " Tile" << t.getId() << ")" << std::endl;
                            data << "                          (hasColor" << color << " Cell_X" << tileX + fieldX << "_Y" << fieldY + tileY << ")" << std::endl;
                            data << "                          (notUsed TileCell_T" << t.getId() << "_X" << tileX << "_Y" << tileY << ")" << std::endl;
// TileCell
                            data << "                     )" << std::endl;
// Effect
                            data << "       :effect (and (not (notUsed TileCell_T" << t.getId() << "_X" << tileX << "_Y" << tileY << "))" << std::endl;
                            data << "                    (wasUsed TileCell_T" << t.getId() << "_X" << tileX << "_Y" << tileY << ")" << std::endl;
                            data << "                    (not (hasColor" << color << " Cell_X" << tileX + fieldX << "_Y" << fieldY + tileY << "))" << std::endl;
                            data << "                    (hasColor" << color +1 << " Cell_X" << tileX + fieldX << "_Y" << fieldY + tileY << ")" << std::endl;
                            data << "               )" << std::endl;
                            data << "   )" << std::endl;

                        }
                    }
                }
            }
        }
    }

/* 3. Action recolor von n-1 auf 0
*/
    uint color = gameField.getField().getMod()-1;
    for(const Tile& t: gameField.getTiles()){
        for(uint fieldX = 0; fieldX < sizeX; ++fieldX){
            for(uint fieldY = 0; fieldY < sizeY; ++fieldY){
                if(t.getSizeX() + fieldX > sizeX) break;
                if(t.getSizeY() + fieldY > sizeY) break;
                for(uint tileX=0; tileX < t.getSizeX(); ++tileX){
                    for(uint tileY=0; tileY < t.getSizeY(); ++tileY){
    // Action
                        data << "   (:action chanceColor" << color << "_T" << t.getId() << "_X" << fieldX << "_Y" << fieldY
                             << "_TX" << tileX << "_TY" << tileY << std::endl;
// Parameter
                        data << "       :parameters()"  << std::endl;
// Precondition
// Tile
                        data << "       :precondition (and (TileStartAtPos_X" << fieldX << "_Y" << fieldY << " Tile" << t.getId() << ")" << std::endl;
                        data << "                          (hasColor" << color << " Cell_X" << tileX + fieldX << "_Y" << fieldY + tileY << ")" << std::endl;
                        data << "                          (notUsed TileCell_T" << t.getId() << "_X" << tileX << "_Y" << tileY << ")" << std::endl;
// TileCell
                        data << "                     )" << std::endl;
// Effect
                        data << "       :effect (and (not (notUsed TileCell_T" << t.getId() << "_X" << tileX << "_Y" << tileY << "))" << std::endl;
                        data << "                    (wasUsed TileCell_T" << t.getId() << "_X" << tileX << "_Y" << tileY << ")" << std::endl;
                        data << "                    (not (hasColor" << color << " Cell_X" << tileX + fieldX << "_Y" << fieldY + tileY << "))" << std::endl;
                        data << "                    (hasColor" << 0 << " Cell_X" << tileX + fieldX << "_Y" << fieldY + tileY << ")" << std::endl;
                        data << "               )" << std::endl;
                        data << "   )" << std::endl;


                    }
                }
            }
        }
    }
    data << ")" << std::endl;
    data.close();
}

/**
 * @brief Solver::PddlStripsConst::readSolution
 * @param solutionFile
 */
void Solver::PddlStripsConst::readSolution(const std::string& solutionFile){
    readSolutionMadagascar(solutionFile);
}

/**
 * @brief Solver::PddlStripsConst::startSolver
 * @param domainFile
 * @param problemFile
 * @param solutionFile
 */
void Solver::PddlStripsConst::startSolver(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile){
    startSolverMadagascar(domainFile, problemFile, solutionFile);
}

/**************************************************************************************************
 *                                  Madagascar
 * ***********************************************************************************************/
void Solver::PddlStripsConst::startSolverMadagascar(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile){
    std::stringstream str;
    str << "/home/fettpet/Dropbox/Programmieren/_P018_Modulo/Solver/M -P 0 -o " << solutionFile << " " << domainFile << " " << problemFile;
    std::system(str.str().c_str());
}

void Solver::PddlStripsConst::readSolutionMadagascar(const std::string &solutionFile){
    std::fstream data;
    data.open(solutionFile, std::ios::in);
    while(!data.eof()){
        std::string buffer, tile, posX, posY;
        std::getline(data, buffer);
        if(buffer.empty()) break;
        if(buffer.find("settile") == std::string::npos) continue;
        tile = buffer.substr(buffer.find("_t")+2, buffer.find("_x")- buffer.find("_t")-2);
        posX = buffer.substr(buffer.find("_x")+2, buffer.find("_y")- buffer.find("_x")-2);
        posY = buffer.substr(buffer.find("_y")+2, buffer.find("(")- buffer.find("_y")-2);
        gameField.getTileById(std::atoi(tile.c_str())).setSolutionX(std::atoi(posX.c_str()));
        gameField.getTileById(std::atoi(tile.c_str())).setSolutionY(std::atoi(posY.c_str()));
    }
}
