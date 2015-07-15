#ifndef CROSSTOBITSET_H
#define CROSSTOBITSET_H
#include <QString>
#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <QMap>
#include "pos.h"
#include "cross.h"
#define DEBUG 0

class CrossToBitset
{
public:
    CrossToBitset();
    static QString solve(const CrossFlip& t);

    static void createStringList(const CrossFlip& t);
    static int createVars(const CrossFlip& t);
    static void solveSystem();
    static void createUnderMatrix();
    static void createUpperMatrix();
    static QString readSolution(const CrossFlip& t);
    static void DoThread(boost::dynamic_bitset<>& list);
private:
    static boost::dynamic_bitset<> currentBitString;
    static int currentPos;
    static std::vector< boost::dynamic_bitset<> > StringList;
    static QMap<Pos, int> PosToInt;
    static QMap<int, Pos> IntToPos;
    static int countVars;
};

#endif // CROSSTOBITSET_H
