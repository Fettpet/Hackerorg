#ifndef SOLVER_H
#define SOLVER_H
#include "world.h"

class solver
{
public:
    solver();
    void exec();
private:
    void normal(world& t);
    void oneIsGiven(world& t);
    void TwoAreGiven(world& t);
};

#endif // SOLVER_H
