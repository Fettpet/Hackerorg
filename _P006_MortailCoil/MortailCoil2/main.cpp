#include <iostream>
#include "World/world.h"
#include "Solver/regiondfs.h"
#include "Solver/dfs.h"

using namespace std;

int main()
{
    World::world w;
    w.readFromFile("/home/fettpet/coil.txt");
    Solver::dfs solver(w);
    //solver.solve();
    return 0;
}

