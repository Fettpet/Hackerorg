#ifndef TABUSEARCH_H
#define TABUSEARCH_H
#include "Modulo/game.h"
#include "Modulo/pos.h"
#include <vector>
#include <boost/random/mersenne_twister.hpp>
#include "solver.h"
#include <boost/random/uniform_int_distribution.hpp>

#define __RESTARTCONSTANT__ 25000
namespace Solver {

typedef struct {
    uint tile;
    uint posX, posY;
    uint HeuristicValue;
}Move;

class TabuSearch : public SolverBaseClass
{
public:
    TabuSearch(const Game::Game &, uint seed);

// Solution
    virtual Game::Game getSolution();

// For parallel
    static void setFinished();
    static void setNewGame();

// Restart a game
    void restart();
// Moves


    uint calcHeuristic() const;
    uint calcPartialHeuristic(const uint tile, const uint startX, const uint startY, const uint goalX, const uint goalY) const;
    void move(uint tile, Pos<uint> newPos);

    // Debuggging
    void printCurrentValues();
    void printTiles();
private:

// Variables
    boost::random::mt19937 rng;
    static boost::random::mt19937 rngSeed;
static boost::random::uniform_int_distribution<> randomSeed;
    uint restartCounter, restartValue;
    std::vector< std::vector< uint > > currentValue;
    std::vector<Pos<uint > > positions;
    std::vector<Pos<uint > > positionsMax;
    uint heuristicValue;
    std::vector<uint> tabuValue;

// functions
    void getTheCurrentValues();
static bool finished;


};

}

#endif // TABUSEARCH_H
