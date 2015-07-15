#include "game.h"
#include <fstream>
#include <exception>
#include <sstream>
#include <iostream>
#include "../Exceptions/stringexception.h"
using namespace Game;
uint Game::Game::GlobalLevel(0);

/**************************************************************************************************
 *                              Constructor
 * ***********************************************************************************************/
/**
  The Standard Constructor. Load an Easy Gamefield
 * @brief Game::Game
 */
Game::Game::Game():tileCount(0){
}

/**
  This function open a file (inputFile). Load the Data for gamefield from
  this file. Set the currentlevel to lev. The File is a HTML file witch can be loaded
  from Hacker.org
 * @brief Game::Game
 * @param inputFile: The inputfile
 * @param lev: The current Level. If it is 0 it is set to GlobalLevel +1
 */
Game::Game::Game(const std::string& inputFile, const uint lev ){
// Set the Level
    if(lev != 0){
        GlobalLevel = lev;
        Level = lev;
    } else{
        GlobalLevel++;
        Level = GlobalLevel;
    }
// Load the Level
    std::fstream file;
    file.open(inputFile.c_str(), std::ios::in);
    while(!file.eof()){
        std::string buffer;
        std::getline(file, buffer);
        if(buffer.find("<param name=\"depth\"") != std::string::npos){
            buffer = buffer.substr(buffer.find("value")+ 7);
            buffer = buffer.substr(0, buffer.find("\""));
            gameField.setMod(std::stoi(buffer));
         }else if(buffer.find("<param name=\"board\"") != std::string::npos){
            buffer = buffer.substr(buffer.find("value")+ 7);
            buffer = buffer.substr(0, buffer.find("\""));
            gameField.load(buffer);
        } else if(buffer.find("<param name=\"pieces\"") != std::string::npos){
            buffer = buffer.substr(buffer.find("value")+ 7);
            buffer = buffer.substr(0, buffer.find("\""));
            while(!(buffer.find(" ") == std::string::npos)){
                std::string bu = buffer.substr(0, buffer.find(" "));
                tiles.push_back(Tile(bu));
                buffer = buffer.substr(buffer.find(" ")+1);
            }
            tiles.push_back(Tile(buffer));
        }
    }
    tileCount =tiles.size();
}

/**
  Copyconstructor
 * @brief Game::Game
 * @param other
 */
Game::Game::Game(const Game& other):
    tileCount(other.tileCount),
    tiles(other.tiles),
    gameField(other.gameField),
    Level(other.Level)
{}

/**************************************************************************************************
 *                                      operator
 * ***********************************************************************************************/
/**
  Copy operator
 * @brief Game::operator =
 * @param other: The other Game
 * @return
 */
Game::Game& Game::Game::operator=(const Game& other){
    tiles = other.tiles;
    gameField = other.gameField;
    Level = other.Level;
    return *this;
}

/**************************************************************************************************
 *                                      Getter
 * ***********************************************************************************************/
/**
    return a const reference to the current gamefield
 * @brief Game::getField
 * @return
*/
const Game::Field& Game::Game::getField()const{
    return gameField;
}

/**
  return a reference to the current gamefield
 * @brief Game::getField
 * @return
 */
Game::Field& Game::Game::getField(){
    return gameField;
}

/**
  return the n-th Tile
 * @brief Game::getTile
 * @param n: Number of the Tile
 * @return
 */
const Tile& Game::Game::getTile(const uint n)const{
    if(n>=tileCount){
        throw Exception::StringException("The n is greater than the size of Tile");
    }
    return tiles[n];
}

/**
return the n-th Tile
 * @brief Game::getTile
 * @param n
 * @return
 */
Tile& Game::Game::getTile(uint n){
    if(n>=tileCount){
        throw Exception::StringException("The n is greater than the size of Tile");
    }
    return tiles[n];
}

/**
    return the count of tiles
 * @brief Game::getTileCount
 * @return
*/
uint Game::Game::getTileCount()const{
    return tileCount;
}



/**
return all Tiles, that are stored in the tilecontainer
 * @brief Game::getTiles
 * @return
*/
Game::TileContainer Game::Game::getTiles()const{
    TileContainer result;
    std::vector<bool> used;
    for(uint t=0; t<tileCount; ++t){
        used.push_back(false);
    }

    for(uint t=0; t<tileCount; ++t){

        uint min = 0;
        uint minNum = 0;
        for(uint i=0; i<tileCount; ++i){
            if(used[i]) continue;
            if(tiles[i].getCountSet() > min){
                min = tiles[i].getCountSet();
                minNum = i;
            }
        }
        result.push_back(tiles[minNum]);
        used[minNum] = true;
    }
    return result;
}

/**
  return the current level
 * @brief Game::getLevel
 * @return
 */
uint Game::Game::getLevel() const{
    return Level;
}
/**
  Search the tile with tileid id in TileContainer
 * @brief leById
 * @param id: The id witch is looked for
 * @return The tile with the id, if it is found
 *          in other cases it Return an Empty Tile
 */
const Tile& Game::Game::getTileById(const uint id)const{
    for(const Tile& t: tiles){
        if(t.getId() == id){
            return t;
        }
    }
    throw Exception::StringException("ID not found");
}

Tile& Game::Game::getTileById(const uint id){
    for(Tile& t: tiles){
        if(t.getId() == id){
            return t;
        }
    }
    throw Exception::StringException("ID not found");
}
/**
 * @brief getHackerOrgSolution
 * @return
 */
std::string Game::Game::getHackerOrgSolution() const{
    std::stringstream result;
    result << std::hex;
    for(uint i=0; i< tiles.size(); ++i){
        if(tiles[i].getSolutionX() < 16){
            result << "0";
        }
        result << tiles[i].getSolutionX();
        if(tiles[i].getSolutionY() < 16){
            result << "0";
        }
        result <<tiles[i].getSolutionY();
    }

    return result.str();
}

/**************************************************************************************************
 *                                  Setter
 * ***********************************************************************************************/
void Game::Game::addTile(const Tile& add){
    tiles.push_back(add);
    tileCount++;
}

/**************************************************************************************************
 *                                      output
 * ***********************************************************************************************/
std::ostream& operator <<(std::ostream& out, const Game::Game& g){
    out << "Field:" << std::endl;
    out << g.getField() << std::endl;
    out << "Tiles: " << g.getTileCount() << std::endl;
    for(uint i=0 ; i<g.getTileCount(); ++i){
        out << "Tile Nr: " << i << std::endl;
        out << g.getTile(i) << std::endl;
    }
    return out;
}

/**************************************************************************************************
 *                                      testing
 * ***********************************************************************************************/
void Game::Game::executeTest(){
    for(const Tile& t: tiles){
        for(uint x=t.getSolutionX(); x< t.getSolutionX() + t.getSizeX(); ++x){
            for(uint y=t.getSolutionY(); y< t.getSolutionY() + t.getSizeY(); ++y){
                gameField.setField(x, y, (gameField.getField(x, y) +1) % gameField.getMod());
            }
        }
    }
}
