#include "point.h"

point::point()
{
    x=0;
    y=0;
}

std::ostream& operator<< (std::ostream& os, point &val){
    os << "x: " << val.x << " y:" << val.y;
    return os;
}

bool point::operator==(const point& t){
    return ((t.x == x) && (t.y == y));
}
