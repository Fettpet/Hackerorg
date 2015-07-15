#include "pos.h"
#include <ostream>


std::ostream& operator<<(std::ostream& out, const Pos<uchar>& p){
    out << "(" << (int)p.getX() << "|" << (int)p.getY() << ")";
    return out;
}
