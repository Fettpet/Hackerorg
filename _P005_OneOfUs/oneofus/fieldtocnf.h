#ifndef FIELDTOCNF_H
#define FIELDTOCNF_H
#include <sstream>
#include <map>
#include <vector>
#include "pos.h"
#include "fieldd.h"

class FieldToCnf
{
public:
    FieldToCnf();
    void doIt(const Field& f);
// getter
    std::stringstream& getResult();
    int getVarCount();
    int getClauselCount();
    std::string getSolutionString(std::string fileName);
private:
    void calcExactlyOneForAllCells(const Field& f);
    void calcExactlyOneForAllValues(const Field& f);
    void calcNextPosForCell(short x, short y, const Field& f);

    void calcTransforms(const Field& f);
    int countVar, countClausel;
    std::stringstream result;
    std::map<Pos, int> PosToInt;
    std::vector<Pos> IntToPos;

};

#endif // FIELDTOCNF_H
