#ifndef POINT_H
#define POINT_H
#include <ostream>

class point
{
public:
    point();
    unsigned int x,y;

    bool operator==(const point& t);
};

std::ostream& operator<< (std::ostream& os, point &val);

#endif // POINT_H
