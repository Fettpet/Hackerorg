/**
  @Author: Sebastian Hahn
  @Email:  seb.hahn1991@googlemail.com
This class represent the complete Modulo game. It is a combination of
Field and many tiles
*/
#ifndef GAME_H
#define GAME_H
#include <vector>
#include "field.h"
#include "tile.h"
#include <string>
/**************************************************************************************************
 *                                  typedef
 * ***********************************************************************************************/
typedef unsigned int uint;

namespace Game {
    typedef  std::vector< Tile > TileContainer;
}
/**************************************************************************************************
 *                                          class
 * ***********************************************************************************************/
namespace Game{

    class Game
    {
    public:
// Constructor
        Game();
        Game(const std::string& inputFile, const uint lev = 0);
        Game(const Game& other);
// Operator
        Game& operator=(const Game& other);

// Getter
        const Field& getField()const;
        Field& getField();
        const Tile& getTile(const uint nr)const;
        Tile& getTile(const uint nr);
        const Tile& getTileById(const uint nr)const;
        Tile& getTileById(const uint nr);

        std::string getHackerOrgSolution() const;
        uint getTileCount()const;
        TileContainer getTiles()const;
        uint getLevel() const;
// testint
        void executeTest();
// Setter
        void addTile(const Tile& add);

    private:
        uint tileCount;
        TileContainer tiles;
        Field gameField;
        static uint GlobalLevel;
        uint Level;
    };
}

std::ostream& operator<<(std::ostream& out, const Game::Game& other);

#endif // GAME_H
