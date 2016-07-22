#ifndef SPLITTER_H
#define SPLITTER_H
#include "world.h"
#include <QVector>
#include <QString>
#include <sstream>
#include <utility>

class Splitter
{
public:
    Splitter();
    void exec(const world& input);
    int countNumberWorlds()const;
    world getWorld(int nr)const;
    QString getTheSplitAsString()const;
private:
    QVector<world> worlds;
    QVector<std::pair<int, int> > Entrance;

    void createEntraceList();
    void deleteEntracesTheyEndInTheSameField();
    void deleteEntracesNeighbour();
    void testTheEntraces();
    world testEntrace(const world& t, std::pair<int, int> point);
};

#endif // SPLITTER_H
