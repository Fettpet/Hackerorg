#include "IO.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "../Exceptions/stringexception.h"
/**************************************************************************************************
 *                                  read
 * ***********************************************************************************************/
/**
  need curl
  Read the current solution from hacker.org
 * @brief readGameFromHackerOrg
 * @param user: The users name
 * @param password: the password
 * @return
*/
Game::Game IO::readGameFromHackerOrg(const std::string& user,
                           const std::string& password){
    std::stringstream downloadUrl;
    downloadUrl << "curl --data \"name=" << user << "&password=" << password << "\" "
                << "--output /tmp/hacker.html "
                << "http://www.hacker.org/modulo/index.php";
    std::cout << downloadUrl.str() << std::endl;
    std::system(downloadUrl.str().c_str());

    return Game::Game("/tmp/hacker.html");
}
/**
Aufbau der datei:

 * @brief IO::readGameFromFile
 * @param filename
 * @return
 */
Game::Game IO::readGameFromFile(const std::string &filename){
    Game::Game result;
    std::fstream data;
    std::stringstream convert;
    uchar mod;
    uint countTiles, sizeXField, sizeYField;
    uint sizeXTile, sizeYTile;
    int solutionX, solutionY;
    std::string line, buffer;
// open file
    data.open(filename, std::ios::in);
    if(!data.is_open()){
        std::stringstream str;
        str << "couldn't open " << filename;
        throw Exception::StringException(str.str());
    }

// read Modulo
    std::getline(data, line);
    line = line.substr(line.find("=")+1);
    line = line.substr(0, line.find(";"));
    mod = std::atoi(line.c_str());
    result.getField().setMod(mod);

// readtTileCount
    std::getline(data, line);
    line = line.substr(line.find("=")+1);
    line = line.substr(0, line.find(";"));
    countTiles = std::atoi(line.c_str());
// read Fieldsize X
    std::getline(data, line);
    std::getline(data, line);
    line = line.substr(line.find("=")+1);
    line = line.substr(0, line.find(";"));
    sizeXField = std::atoi(line.c_str());

// read Fieldsize Y
    std::getline(data, line);
    line = line.substr(line.find("=")+1);
    line = line.substr(0, line.find(";"));
    sizeYField = std::atoi(line.c_str());

    result.getField().resize(sizeXField, sizeYField);
// read Field
    for(uint y=0; y<sizeYField; ++y){
        std::getline(data, line);
        std::getline(data, line);
        line.erase(0,1);
        for(uint x=0; x<sizeXField; ++x){
            line.erase(0,1);
            buffer = line.substr(0, line.find(" ")+1);
            buffer = buffer.substr(0, buffer.find(" "));
            result.getField().setField(y,x, std::atoi(buffer.c_str()));
            line.erase(0, line.find("|")+1);
        }
    }

// read Tiles
    for(uint i=0; i<countTiles; ++i) {
// size Tile x

        std::getline(data, line);
        std::getline(data, line);
        std::getline(data, line);
        line = line.substr(line.find("=")+1);
        line = line.substr(0, line.find(";"));
        sizeXTile = std::atoi(line.c_str());

// size Tile y
        std::getline(data, line);
        line = line.substr(line.find("=")+1);
        line = line.substr(0, line.find(";"));
        sizeYTile = std::atoi(line.c_str());

// Solution X
        std::getline(data, line);

        line = line.substr(line.find("=")+1);
        line = line.substr(0, line.find(";"));
        if(line != "UNKNOWN"){
            solutionX = std::atoi(line.c_str());
        } else {
            solutionX = -1;
        }

// Solution Y
        std::getline(data, line);
        line = line.substr(line.find("=")+1);
        line = line.substr(0, line.find(";"));
        if(line != "UNKNOWN"){
            solutionY = std::atoi(line.c_str());
        } else {
            solutionY = -1;
        }
    //   std::cout << "(" << sizeXTile << ", " << sizeYTile << ") (" << solutionX << ", " << solutionY << ")" << std::endl;
        Tile t(sizeXTile, sizeYTile, solutionX, solutionY);
// read the tile
        for(uint y=0; y<sizeYTile; ++y){
            std::getline(data, line);
            std::getline(data, line);

            for(uint x=0; x<sizeXTile; ++x){
                line.erase(0,1);
                buffer = line;//.substr(0, line.find("|")+1);
                buffer = buffer.substr(0, 1);//buffer.find("|"));
                if(buffer[0] == 'X'){
                    t.setField(x, y, true);
                } else{
                    t.setField(x, y, false);
                }
                line.erase(0, line.find("|"));
            }

        }
   //     std::cout << "Size (x, y): " << t.getSizeX() << " " << t.getSizeY() <<std::endl;
   //     std::cout << t;
        std::cout << t << std::endl;
        std::cout << "Count " << t.getCountSet() << std::endl;
        result.addTile(t);
    }

    //std::cout << result;
    return result;
}

/**************************************************************************************************
 *                                      write
 * ***********************************************************************************************/

/**
 * @brief writeSolutionToHacker: send the solution to Hacker.org need curl
 * @param user
 * @param password
 * @param solution
 */
void IO::writeSolutionToHacker(const std::string& user, const std::string& password, const std::string solution){
    std::stringstream downloadUrl;
    downloadUrl << "curl --data \"name=" << user << "&password=" << password << "&seq=" << solution << "\" "
                << "--output /tmp/hacker.html "
                << "http://www.hacker.org/modulo/index.php";
    std::cout << downloadUrl.str() << std::endl;
    std::system(downloadUrl.str().c_str());
}

/**

 * @brief IO::writeGameToFile write a Game to a File
 * @param game: The game
 * @param filename The filename
 */
void IO::writeGameToFile(const Game::Game& game, const std::string filename){
    std::fstream data;
    data.open(filename, std::ios::out);
    if(!data.is_open()){
        std::stringstream str;
        str << "Couldn`t open file " << filename;
        throw Exception::StringException(str.str());
    }
// Begin vars
    data << "Modulo=" << (int)game.getField().getMod() << ";" << std::endl;
    data << "CountTile=" << game.getTileCount() << ";" << std::endl;
    data << std::endl;
    data << "SizeXField=" << game.getField().getSizeX() << ";" << std::endl;
    data << "SizeYField=" << game.getField().getSizeY() << ";" << std::endl;


// Field
    data << game.getField() << std::endl;
//    data << std::endl;

// Tiles
    for(Tile t: game.getTiles()){
        data << "SizeXTile=" << t.getSizeX() << ";" << std::endl;
        data << "SizeYTile=" << t.getSizeY() << ";" << std::endl;
        if(t.getSolutionX() == -1){
            data << "SolutionX=UNKNOWN;" << std::endl;
        } else {
            data << "SolutionX=" << t.getSolutionX() << ";" << std::endl;
        }

        if(t.getSolutionY() == -1){
            data << "SolutionY=UNKNOWN;" << std::endl;
        } else {
            data << "SolutionY=" << t.getSolutionY() << ";" << std::endl;
        }

        data << t;
        data << std::endl;
    }
}

/**************************************************************************************************
 *                                              Goto Level
 * ***********************************************************************************************/
void IO::gotoLevel(const std::string& user, const std::string& password, uint level){
    std::stringstream downloadUrl;
    downloadUrl << "curl --data \"name=" << user << "&password=" << password << "&gotolevel=" << level << "\" "
                << "--output /tmp/hacker.html "
                << "http://www.hacker.org/modulo/index.php";
    std::cout << downloadUrl.str() << std::endl;
    std::system(downloadUrl.str().c_str());
}
