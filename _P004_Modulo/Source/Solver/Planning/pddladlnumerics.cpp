#include "pddladlnumerics.h"
#include <fstream>
#include <sstream>

Solver::PddlAdlNumerics::PddlAdlNumerics(const Game::Game& t, const std::string solver):
    Solver::PddlBaseClass(t, solver){}

/**************************************************************************************************
 *                              Problem file
 * ***********************************************************************************************/
void Solver::PddlAdlNumerics::createProblemFile(const std::string& outFileName){
    std::fstream data;
    data.open(outFileName, std::ios::out);
    data << "(define (problem modulo-adlNumericsProblem)" << std::endl;
    data << "   (:domain Modulo-adlNumerics)" << std::endl;
/**********
 * objects
 * ******/
    data << "   (:objects" << std::endl;
// Tile
    data << "       ";
    for(Tile t: gameField.getTiles()){
        data << "tile_" << t.getId() << " ";
    }
    data << "- Tile" << std::endl;
// Tile Zellen
    for(Tile t: gameField.getTiles()){
        data << "       ";
        for(uint sizeX=0; sizeX<t.getSizeX(); ++sizeX)
            for(uint sizeY=0; sizeY<t.getSizeY(); ++sizeY){
                if(t.getField(sizeX, sizeY))
                    data << "tileCell_" << t.getId() << "_" << sizeX << "_" <<sizeY << " ";

            }
        data << "- TileCell" << std::endl;
    }
//Positions
    data << "       ";
    for(uint posX=0; posX < gameField.getField().getSizeX(); ++posX){
        for(uint posY=0; posY < gameField.getField().getSizeY(); ++posY){
           data << "pos_" << posX << "_" << posY << " ";
        }
    }
    data << "- Position" << std::endl;
// Gamefield Zellen
    data << "       ";
    for(uint posX=0; posX < gameField.getField().getSizeX(); ++posX){
        for(uint posY=0; posY < gameField.getField().getSizeY(); ++posY){
           data << "gameFieldCell_" << posX << "_" << posY << " ";
        }
    }
    data << "- GameFieldCell" << std::endl;
// Values
    data << "       ";
    for(uint i=0; i < gameField.getField().getMod(); ++i){
        data << "value_" << i << " ";
    }
    data << "- Value" << std::endl;
    data << "   )" << std::endl;
/**********
* inital state
* ******/
    data << "   (:init" << std::endl;
// Tile not used
    for(Tile t: gameField.getTiles()){
        data << "       (notUsed tile_" << t.getId() << ")" << std::endl;
    }
// Tile Cell Not Used
    for(Tile t: gameField.getTiles()){
        for(uint sizeX=0; sizeX<t.getSizeX(); ++sizeX)
            for(uint sizeY=0; sizeY<t.getSizeY(); ++sizeY){
                if(t.getField(sizeX, sizeY))
                    data << "       (notUsed tileCell_" << t.getId() << "_" << sizeX << "_" <<sizeY << ")" << std::endl;
            }
    }
// HasValue zuweisen
    for(uint sizeX=0; sizeX<gameField.getField().getSizeX(); ++sizeX)
        for(uint sizeY=0; sizeY<gameField.getField().getSizeY(); ++sizeY){
            data << "       (hasValue gameFieldCell_" << sizeX << "_" <<sizeY << " value_" << (int)gameField.getField().getField(sizeX, sizeY) << ")" << std::endl;
        }
// positionAdd
    for(uint sizeX=0; sizeX<gameField.getField().getSizeX(); ++sizeX)
        for(uint sizeY=0; sizeY<gameField.getField().getSizeY(); ++sizeY){
            for(uint sizeX2=0; sizeX2<gameField.getField().getSizeX() - sizeX; ++sizeX2)
                for(uint sizeY2=0; sizeY2<gameField.getField().getSizeY() - sizeY; ++sizeY2){
                    data << "       (positionAdd " << "pos_" << sizeX << "_" << sizeY << " pos_" << sizeX2 << "_" << sizeY2 << " pos_" << sizeX + sizeX2 << "_" << sizeY + sizeY2 << ")" << std::endl;
                }
        }
// isCellFormTile
    for(Tile t: gameField.getTiles()){
        data << "       ";
        for(uint sizeX=0; sizeX<t.getSizeX(); ++sizeX)
            for(uint sizeY=0; sizeY<t.getSizeY(); ++sizeY){
                if(t.getField(sizeX, sizeY))
                    data << "(isCellFormTile tileCell_" << t.getId() << "_" << sizeX << "_" <<sizeY << " tile_" << t.getId() <<  " ) ";
            }
        data << std::endl;
     }
// isAtPosition gamefield
    for(uint sizeX=0; sizeX<gameField.getField().getSizeX(); ++sizeX)
        for(uint sizeY=0; sizeY<gameField.getField().getSizeY(); ++sizeY){
            data << "       (isAtPositionFieldCell gameFieldCell_" << sizeX << "_" << sizeY << " pos_" << sizeX << "_" << sizeY << ")" << std::endl;
        }
// isAtPosition gamefield
    for(Tile t: gameField.getTiles()){
        for(uint sizeX=0; sizeX<t.getSizeX(); ++sizeX)
            for(uint sizeY=0; sizeY<t.getSizeY(); ++sizeY){
                if(t.getField(sizeX, sizeY))
                    data << "       (isAtPositionTileCell tileCell_" << t.getId() << "_" << sizeX << "_" << sizeY << " pos_" << sizeX << "_" << sizeY << ")" << std::endl;
            }
    }
// next Value
    for(uint i=0; i< gameField.getField().getMod(); ++i){
        data << "       (nextValue value_" << i << " value_" << (i+1) % gameField.getField().getMod() << ")" << std::endl;
    }
// legal Positions
    for(Tile t: gameField.getTiles()){
        for(uint posX=0; posX <= gameField.getField().getSizeX() - t.getSizeX(); ++posX){
            for(uint posY=0; posY <= gameField.getField().getSizeY() - t.getSizeY(); ++posY){
                data << "       (legalPosition tile_" << t.getId() << " pos_" << posX << "_" << posY << ")" << std::endl;
            }
        }
    }
    data << "   )" << std::endl;
/***************
    goal
***************/
    data << "   (:goal (and" << std::endl;
// Alle Farben sind 0
    for(uint sizeX=0; sizeX<gameField.getField().getSizeX(); ++sizeX)
        for(uint sizeY=0; sizeY<gameField.getField().getSizeY(); ++sizeY){
            data << "       (hasValue gameFieldCell_" << sizeX << "_" << sizeY << " value_0)" << std::endl;
        }

    for(Tile t: gameField.getTiles()){
        for(uint sizeX=0; sizeX<t.getSizeX(); ++sizeX)
            for(uint sizeY=0; sizeY<t.getSizeY(); ++sizeY){
                if(t.getField(sizeX, sizeY))
                    data << "       (wasUsed tileCell_" << t.getId() << "_" << sizeX << "_" <<sizeY << ")" << std::endl;
            }
    }
    data << "   ))" << std::endl;
    data << ")" << std::endl;
}

/**************************************************************************************************
 *                                      Domain file
 * ***********************************************************************************************/
void Solver::PddlAdlNumerics::createDomainFile(const std::string& outFileName){
    std::fstream data;
    data.open(outFileName, std::ios::out);
    data << "(define (domain Modulo-adlNumerics)" << std::endl;
// requirements
    data << "   (:requirements :typing)" << std::endl;
// types
    data << "   (:types" << std::endl;
    data << "       Tile" << std::endl;
    data << "       TileCell" << std::endl;
    data << "       Position" << std::endl;
    data << "       GameFieldCell" << std::endl;
    data << "       Value" << std::endl;
    data << "   )" << std::endl<< std::endl;
// Prädikate
    data << "   (:predicates" << std::endl;
    data << "       (wasUsed ?t)" << std::endl;
    data << "       (notUsed ?t)" << std::endl;
    data << "       (positionAdd ?p1 - Position ?p2 - Position ?ps - Position)" << std::endl;
    data << "       (hasValue ?gfc - GameFieldCell ?v - Value)" << std::endl;
    data << "       (isCellFormTile ?tc - TileCell ?t - Tile)" << std::endl;
    data << "       (isAtPositionTileCell ?tc - TileCell ?p - Position)" << std::endl;
    data << "       (isAtPositionFieldCell ?tc - GameFieldCell ?p - Position)" << std::endl;
    data << "       (nextValue ?v1 - Value ?v2 - Value)" << std::endl;
    data << "       (legalPosition ?t - Tile ?p - Position)" << std::endl;
    data << "       (isAtPositionTile ?t - Tile ?p - Position)" << std::endl;
    data << "   )" << std::endl << std::endl;
// Functions
//    data << "   (:functions" << std::endl;
//    data << "       (rowCount ?p - Position)" << std::endl;
//    data << "       (colCount ?p - Position)" << std::endl;
//    data << "   )" << std::endl << std::endl;
// Action putdown
    data << "   (:action putdown" << std::endl;
    data << "       :parameters(" << std::endl;
    data << "           ?t - Tile" << std::endl;
    data << "           ?p - Position" << std::endl;
    data << "       )" << std::endl;
    data << "       :precondition( and" << std::endl;
//    data << "           (legalPosition ?t ?p)" << std::endl;
    data << "           (notUsed ?t)" << std::endl;
    data << "       )" << std::endl;
    data << "       :effect ( and" << std::endl;
    data << "           (not (notUsed ?t))" << std::endl;
    data << "           (wasUsed ?t)" << std::endl;
    data << "           (isAtPositionTile ?t ?p)" << std::endl;
    data << "       )" << std::endl;
    data << "   )" << std::endl;

// Action
    data << "   (:action setCells" << std::endl;
    data << "       :parameters(" << std::endl;
    data << "           ?t - Tile" << std::endl;
    data << "           ?p - Position" << std::endl;
    data << "           ?tc - TileCell" << std::endl;
    data << "           ?gfc - GameFieldCell" << std::endl;
    data << "           ?ptc - Position" << std::endl;
    data << "           ?pgfc - Position" << std::endl;
    data << "           ?vBefore - Value " << std::endl;
    data << "           ?vAfter - Value" << std::endl;
    data << "       )" << std::endl;
    data << "      " << std::endl;
    data << "       :precondition( and" << std::endl;
    data << "           (positionAdd ?ptc ?p ?pgfc)" << std::endl;
    data << "           (isCellFormTile ?tc ?t)" << std::endl;
    data << "           (isAtPositionTileCell ?tc ?ptc)" << std::endl;
    data << "           (isAtPositionFieldCell ?gfc ?pgfc)" << std::endl;
    data << "           (notUsed ?tc)" << std::endl;
    data << "           (hasValue ?gfc ?vBefore)" << std::endl;
    data << "           (nextValue ?vBefore ?vAfter)" << std::endl;
    data << "           (isAtPositionTile ?t ?p)" << std::endl;
    data << "       )" << std::endl;
    data << "       " << std::endl;
    data << "       :effect ( and" << std::endl;
    data << "           (not (notUsed ?tc))" << std::endl;
    data << "           (wasUsed ?tc)" << std::endl;
    data << "           (not (hasValue ?gfc ?vBefore))" << std::endl;
    data << "           (hasValue ?gfc ?vAfter)" << std::endl;
    data << "       )" << std::endl;
    data << "    )" << std::endl;
    data << ")" << std::endl;
}

/**************************************************************************************************
 *                          Communicttion
 * ***********************************************************************************************/

void Solver::PddlAdlNumerics::startSolver(const std::string& domainFile, const std::string& problemFile, const std::string& solutionFile){
    std::stringstream str;
    str << solver << " -P 0 " << domainFile << " " << problemFile << " -o " << solutionFile;
    std::system(str.str().c_str());
}

void Solver::PddlAdlNumerics::readSolution(const std::string& solutionFile){
    std::fstream data;
    data.open(solutionFile, std::ios::in);
    while(!data.eof()){
        std::string buffer, tile, posX, posY;
        std::getline(data, buffer);
        if(buffer.find("setcell") != buffer.npos) continue;
        if(buffer.empty()) break;
        tile = buffer.substr(buffer.find("tile_")+5);
        tile = tile.substr(0, tile.find(" "));

        posX = buffer.substr(buffer.find("pos_")+4);
        posY = posX;
        posX = posX.substr(0, posX.find("_"));

        posY = posY.substr(posY.find("_")+1);
        posY = posY.substr(0, posY.find(")"));
        gameField.getTileById(std::atoi(tile.c_str())).setSolutionX(std::atoi(posX.c_str()));
        gameField.getTileById(std::atoi(tile.c_str())).setSolutionY(std::atoi(posY.c_str()));
    }
}
