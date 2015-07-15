#include "pddlstrips.h"
#include <fstream>
#include <sstream>
/**************************************************************************************************
 *                                  Constructor
 * ***********************************************************************************************/
Solver::PddlStrips::PddlStrips(const Game::Game& t, const std::string& sol):
    Solver::PddlBaseClass(t, sol)
{}

/**************************************************************************************************
 *                          virtual functions
 * ***********************************************************************************************/
/**
 * @brief Solver::PddlStrips::createProblemFile
 * @param outFileName
 */
void Solver::PddlStrips::createProblemFile(const std::string& outFileName){
    std::fstream data;
    uint sizeX(gameField.getField().getSizeX());
    uint sizeY(gameField.getField().getSizeY());
    data.open(outFileName, std::ios::out);
    data << "(define (problem modulo-strips)" << std::endl;
    data << "   (:domain Modulo-Strips)" << std::endl;
    data << "   (:objects" << std::endl;
    data << "       ";
    for(const Tile& t: gameField.getTiles()){
        data << "Tile" << t.getId() << " ";
    }
    data << std::endl;

    for(const Tile& t: gameField.getTiles()){
        data << "       ";
        for(uint x=0; x<t.getSizeX(); ++x){
            for(uint y=0;y<t.getSizeY(); ++y){
                if(t.getField(x,y))
                    data << "Tile" << t.getId() <<"_Pos_X" << x << "_y" << y << " ";
            }
        }
        data << std::endl;
    }
    data << "       ";
    for(uint x=0; x<gameField.getField().getSizeX(); ++x){
        for(uint y=0; y<gameField.getField().getSizeY(); ++y){
            data << "Cell_X" << x << "_Y" << y << " ";
        }
    }
    data << std::endl;
    data << "   )" << std::endl;

    data << "   (:init" << std::endl;
// Den Tiles eine Nummer geben
    for(const Tile& t1: gameField.getTiles()){
        data << "       (isTile" << t1.getId() << " Tile" << t1.getId() << ")" << std::endl;
    }
// Sagen das die Positionen eine Position haben
    for(uint posX=0; posX < gameField.getField().getSizeX(); ++posX){
        for(uint posY=0; posY < gameField.getField().getSizeY(); ++posY){
            data << "       ";
            data << "(isPosition_X" << posX << "_Y" << posY;
            data << " Cell_X" << posX << "_Y" << posY << ") ";
            data << std::endl;
        }
    }
// Sagen das die Tiles unbenutzt sind
    for(const Tile& t1: gameField.getTiles()){
        data << "       (notUsed" << " Tile" << t1.getId() << ")" << std::endl;
    }
// Sagen das die Tiles Positionen unbenutzt sind
    for(const Tile& t: gameField.getTiles())
        for(uint x=0; x<t.getSizeX(); ++x)
            for(uint y=0; y<t.getSizeY(); ++y){
                if(t.getField(x,y))
                    data << "       (notUsed Tile" << t.getId() <<"_Pos_X" << x << "_y" << y << ")" << std::endl;
            }
//Den Tile Zellen sagen wo sie sind
    for(const Tile& t: gameField.getTiles())
        for(uint x=0; x<t.getSizeX(); ++x)
            for(uint y=0; y<t.getSizeY(); ++y){
                if(t.getField(x,y))
                    data << "       (Tile_" << t.getId() << "_Pos_X" << x << "_Y" << y
                         << " Tile" << t.getId() << "_Pos_X" << x << "_y" << y << ")" << std::endl;
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
                    data << "       (wasUsed Tile" << t.getId() <<"_Pos_X" << x << "_y" << y << ")" << std::endl;
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
 * @brief Solver::PddlStrips::createDomainFile
 * @param outFileName
 */
void Solver::PddlStrips::createDomainFile(const std::string& domainFileName){
    std::fstream data;
    uint sizeX(gameField.getField().getSizeX());
    uint sizeY(gameField.getField().getSizeY());
    data.open(domainFileName, std::ios::out);
    data << "(define (domain Modulo-Strips)" << std::endl;
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
// Prädikate für Tiles
    for(const Tile& t: gameField.getTiles()){
        data << "       (isTile" << t.getId() << " ?t)" << std::endl;
    }
// Prädikate für die Zellen
    for(uint posX=0; posX < gameField.getField().getSizeX(); ++posX){
        for(uint posY=0; posY < gameField.getField().getSizeY(); ++posY){
            data << "       (isPosition_X" << posX << "_Y" << posY << " ?c)" << std::endl;
        }
    }
/*
// Prädikate des Tiles
    for(const Tile& t: gameField.getTiles())
        for(uint x=0; x<t.getSizeX(); ++x)
            for(uint y=0; y<t.getSizeY(); ++y){
                data << "       (isTilePosition_T" << t.getId() << "_X" << x << "_Y" << y << " ?t)" << std::endl;
            }
*/
// Prädikate für das Starten
    for(const Tile& t: gameField.getTiles()){
        for(uint fieldX = 0; fieldX < sizeX; ++fieldX){
            for(uint fieldY = 0; fieldY < sizeY; ++fieldY){
                if(t.getSizeX() + fieldX > sizeX) break;
                if(t.getSizeY() + fieldY > sizeY) break;
                data << "       (Tile_" << t.getId() << "_startAtPos_X" << fieldX << "_Y" << fieldY << " ?t)" << std::endl;
            }
        }
    }
// Prädikate für das Starten
    for(const Tile& t: gameField.getTiles()){
        for(uint fieldX = 0; fieldX < t.getSizeX(); ++fieldX){
            for(uint fieldY = 0; fieldY < t.getSizeY(); ++fieldY){
                data << "       (Tile_" << t.getId() << "_Pos_X" << fieldX << "_Y" << fieldY << " ?tp)" << std::endl;
            }
        }
    }
// Prädikate für Abgeschlossen
     data << "       (isTileUncomplete ?t)" << std::endl;
// Prädikate für Setzungen
// Tile_T_isSetTo_X_Y_Cell_X_Y
/*
    for(const Tile& t: gameField.getTiles()){
        for(uint fieldX = 0; fieldX < sizeX; ++fieldX){
            for(uint fieldY = 0; fieldY < sizeY; ++fieldY){
                if(t.getSizeX() + fieldX > sizeX) break;
                if(t.getSizeY() + fieldY > sizeY) break;
                for(uint tileX=0; tileX < t.getSizeX(); ++tileX){
                    for(uint tileY=0; tileY < t.getSizeY(); ++tileY){
                        data << "       (Tile_T" << t.getId() << "_isSetTo_X" << fieldX << "_Y" << fieldY
                             << "_Cell_X" << tileX << "_Y" << tileY << " ?t)" << std::endl;
                    }
                }
            }
        }
    }
*/
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
                    data << "       :parameters (?used)" << std::endl;

    // Create Precondition List
                    data << "       :precondition (and (isTile" << t.getId() << " ?used)" << std::endl;
                    data << "                          (notUsed ?used)" << std::endl;
                    data << "                     )" << std::endl;
    // Create Effect liste
                    data << "       :effect (and (wasUsed ?used)" << std::endl;
                    data << "                    (Tile_" << t.getId() << "_startAtPos_X" << fieldX << "_Y" << fieldY <<" ?used)" << std::endl;
                    data << "                    (not (notUsed ?used))" << std::endl;
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
                            data << "       :parameters( ?tile ?cell ?tileCell)"  << std::endl;
// Precondition
// Tile
                            data << "       :precondition (and (isTile" << t.getId() << " ?tile)" << std::endl;
                            data << "                          (Tile_" << t.getId() << "_startAtPos_X" << fieldX << "_Y" << fieldY << " ?tile)" << std::endl;
// Cell
                            data << "                          (isPosition_X" << tileX + fieldX << "_Y" << fieldY + tileY << " ?cell)" << std::endl;
                            data << "                          (hasColor" << color << " ?cell)" << std::endl;
// TileCell
// Tile_1_Pos_X0_Y0
                            data << "                          (Tile_" << t.getId() << "_Pos_X" << tileX << "_Y" << tileY << " ?tileCell)" << std::endl;
                            data << "                          (notUsed ?tileCell)" << std::endl;
                            data << "                     )" << std::endl;
// Effect
                            data << "       :effect (and (not (notUsed ?tileCell))" << std::endl;
                            data << "                    (wasUsed ?tileCell)" << std::endl;
                            data << "                    (not (hasColor" << color << " ?cell))" << std::endl;
                            data << "                    (hasColor" << color +1 << " ?cell)" << std::endl;
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
                        data << "       :parameters( ?tile ?cell ?tileCell)"  << std::endl;
    // Precondition
    // Tile
                        data << "       :precondition (and (isTile" << t.getId() << " ?tile)" << std::endl;
                        data << "                          (Tile_" << t.getId() << "_startAtPos_X" << fieldX << "_Y" << fieldY << " ?tile)" << std::endl;
    // Cell
                        data << "                          (isPosition_X" << tileX + fieldX << "_Y" << fieldY + tileY << " ?cell)" << std::endl;
                        data << "                          (hasColor" << color << " ?cell)" << std::endl;
    // TileCell
    // Tile_1_Pos_X0_Y0
                        data << "                          (Tile_" << t.getId() << "_Pos_X" << tileX << "_Y" << tileY << " ?tileCell)" << std::endl;
                        data << "                          (notUsed ?tileCell)" << std::endl;
                        data << "                     )" << std::endl;
    // Effect
                        data << "       :effect (and (not (notUsed ?tileCell))" << std::endl;
                        data << "                    (wasUsed ?tileCell)" << std::endl;
                        data << "                    (not (hasColor" << color << " ?cell))" << std::endl;
                        data << "                    (hasColor" << 0 << " ?cell)" << std::endl;
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
 * @brief Solver::PddlStrips::readSolution
 * @param solutionFile
 */
void Solver::PddlStrips::readSolution(const std::string& solutionFile){
    readSolutionMadagascar(solutionFile);
}

/**
 * @brief Solver::PddlStrips::startSolver
 * @param domainFile
 * @param problemFile
 * @param solutionFile
 */
void Solver::PddlStrips::startSolver(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile){
    startSolverMadagascar(domainFile, problemFile, solutionFile);
}

/**************************************************************************************************
 *                                  Madagascar
 * ***********************************************************************************************/
void Solver::PddlStrips::startSolverMadagascar(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile){
    std::stringstream str;
    str << "/home/fettpet/Dropbox/Programmieren/_P018_Modulo/Solver/M -P 0 -o " << solutionFile << " " << domainFile << " " << problemFile;
    std::system(str.str().c_str());
}

void Solver::PddlStrips::readSolutionMadagascar(const std::string &solutionFile){
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
