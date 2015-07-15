#ifndef PDDLSOLUTION_H
#define PDDLSOLUTION_H
#include <string>
#include "../../World/world.h"

class pddlSolution
{
public:
    pddlSolution(world _w):
        w(_w){}

    void createDomainFile(const std::string& fileName);
    void createProblemFile(const std::string& fileName);

private:
    world w;

};

#endif // PDDLSOLUTION_H
