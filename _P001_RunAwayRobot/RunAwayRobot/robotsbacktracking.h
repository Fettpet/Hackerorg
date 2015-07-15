#ifndef ROBOTSBACKTRACKING_H
#define ROBOTSBACKTRACKING_H
#include <QString>
#include <QVector>
#include "point.h"
#include "robotsworld.h"
#include "bestpathentry.h"
#include "bigInt/BigIntegerLibrary.hh"

class robotsbacktracking
{
public:
    robotsbacktracking(RobotsWorld &world);
    QString calculate();
    QString BackTracking();
    BigInteger canIgnore(const QString& path)const ;
    unsigned int getNumberCanGo(const QString& path, unsigned int minSteps, unsigned maxSteps);
    RobotsWorld calculateNewRobotsWorld(unsigned int countD, unsigned int countR);
private:
    unsigned int minStepD, maxStepD, minStepR, maxStepR;
    RobotsWorld world;
    QVector<point> WayOut, BlockingBeginning;
    QVector<point> getAllPointsRestart();
    QString calculateAllPermutationsBacktracking(unsigned int countD, unsigned int countR);
    QString calculateAllPermutationsBacktracking(point t);
    QString testAllPermutations(unsigned int countD, unsigned int countR);
    QString testAllPermutations(point t);
    void calculateMinMaxSteps(const unsigned int Steps);
    QString calculateBrutForce(unsigned int steps);
    bool wayCanGoOut(const QString& path)const;

};

#endif // ROBOTSBACKTRACKING_H
