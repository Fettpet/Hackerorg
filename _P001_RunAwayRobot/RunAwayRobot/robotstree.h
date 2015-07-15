#ifndef ROBOTSTREE_H
#define ROBOTSTREE_H
#include "robotsworld.h"
#include <QStringList>
#include <QMessageBox>

class RobotsTree
{
public:
    RobotsTree();
    RobotsTree(const RobotsWorld& world);
    RobotsTree(const RobotsWorld& world, QString path, int x, int y);
    unsigned int maxDeepth();
    QStringList toStringList(int minSize);
    QStringList toStringList();
private:
    RobotsTree *before;
    RobotsTree *nextD, *nextR;

};

#endif // ROBOTSTREE_H
