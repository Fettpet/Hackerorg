#ifndef CROSS_H
#define CROSS_H
#include <vector>
#include <QString>
#include <iostream>
class CrossFlip
{
public:
    CrossFlip(QByteArray fileData);
    int getSizeX() const;
    int getSizeY() const;
    int getLevel() const;
    bool hasStartValue(int x, int y) const;
    bool isBlocked(int x, int y) const;
private:
    int sizeX, sizeY;
    int level;
    std::vector< std::vector < bool > > Blocked, startValue;
};

std::ostream& operator<<(std::ostream& out, const CrossFlip& t);

#endif // CROSS_H
