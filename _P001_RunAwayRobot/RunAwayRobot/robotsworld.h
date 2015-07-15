#ifndef ROBOTSWORLD_H
#define ROBOTSWORLD_H
#include <QString>
#include "downloadmanager.h"
#include <QEventLoop>
#include <QObject>
#include <QStringList>
#include <QVector>
#include "point.h"
#include "bestpathentry.h"
#include <QMessageBox>
#include <fstream>
#include <cmath>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cassert>

class RobotsWorld
{
public:
    RobotsWorld();
    RobotsWorld( const RobotsWorld& t);

// getter
    unsigned int getSizeX()const;
    unsigned int getSizeY()const;
    bool getEntry(const int x, const int y)const;
    bool isBlocked(const int x, const int y) const;
    bool isPassable(const int x, const int y) const;
    unsigned int getMinSteps() const;
    unsigned int getMaxSteps() const;
    void showMessageBox() const;
    void showMessageBox(point tp) const;
    QString getField() const;
    QVector<point> getPointThereCanGoOut() const;
    QVector<point> getPointThereBlockAtTheBegining(const unsigned int numberSteps)const;
// Setter
    void setUnreachableToBlocked();
    void setImpasseToBlock();
    void setBlocked(const int x, const int y);
    void setPassable(const int x, const int y);
    void setEntry(const int x, const int y, bool entry);

    QString calculateBruteForce();
// Test und submit
    void ReadFromData(const QString& fileName);
    void ReadFromData();
    void WriteToData(const QString& fileName)const ;
    void ReadFromInternet(const QString& user, const QString& pass);
    void CreateTest();
    void submitSolution(const QString& solution, const QString& user, const QString& pass)const;
    bool testPath(const QString& path)const;

// Operatoren
    RobotsWorld& operator= (RobotsWorld const& rhs);
private:
    unsigned int sizeX, sizeY;
    unsigned int countMinSteps, countMaxSteps;
    bool **world;
};

#endif // ROBOTSWORLD_H

