#ifndef DEEPSEARCH_H
#define DEEPSEARCH_H
#include "solver.h"
#include "../Modulo/game.h"
#include "../Modulo/pos.h"

namespace Solver{
    class DeepSearch: public Solver::SolverBaseClass
    {
    public:
        DeepSearch(const Game::Game& t);

        virtual Game::Game getSolution();

        uint calcHeuristic() const;
        uint calcPartialHeuristic(const uint tile, const uint goalX, const uint goalY) const;
        void move(uint tile, Pos<uint> newPos);

        // Debuggging
        void printCurrentValues();
        void printTiles();
    private:
        void backtrack(uint tile);
// deepSearch
        bool deepSearch(const uint step);

// The list in which order the Tiles are observed
        typedef std::vector<uint> TileObserveContainer;
        TileObserveContainer tileObserveList;
        void createTileOvserveList();


// Solution container
        typedef std::vector<Pos<uint> > SolutionContainer;
        std::vector< uint > maxFlips;
        SolutionContainer solution, positions, positionsMax;
        void createSolutionContainer();

// can current State Solve
        bool canSolve(const Game::Game& g);
        uint heuristicValue;
        std::vector< std::vector< uint > > currentValue;
    };
}
#endif // DEEPSEARCH_H
