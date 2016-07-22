#include "pos.h"
#include <ostream>


std::ostream& operator<<(std::ostream& out, const Solver::Data::Pos<uchar>& p){
    out << "(" << (int)p.getX() << "|" << (int)p.getY() << ")";
    return out;
}

std::ostream& operator<<(std::ostream& out, const Position& p){
    out << "(" << (int)p.getX() << "|" << (int)p.getY() << ")";
    return out;
}
