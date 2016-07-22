#include <iostream>
#include "World/world.h"
#include "Solver/regiondfs.h"
#include "Solver/dfs.h"
#include "Solver/regionsolverwithoutheuristics.h"
using namespace std;

int main()
{
    World::world w;
    w.readFromFile("/home/fettpet/coil.txt");

    Solver::RegionSolverWithoutHeuristics reg(w.getRegion(0));
    reg.solve(Position(0,1));
    //Solver::dfs solver(w);
    //std::cout << "Solver Case: " << solver.getCaseOfStartPositions() << std::endl;
    //solver.solve();
    return 0;
}

