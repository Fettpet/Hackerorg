#ifndef POS_H
#define POS_H
#include <iostream>
#include <cmath>
typedef unsigned char uchar;
typedef unsigned int uint;

namespace Solver
{
namespace Data
{
template<typename X>
class Pos
{
public:
    Pos():
       x(0), y(0)
    {}

    Pos(X col, X row):
        x(col), y(row)
    {}

    //Pos(const QString& t);
    Pos(const Pos& old):
        x(old.x), y(old.y){}

// getter
    X getX()const{
        return x;
    }
    X getY()const{
        return y;
    }
    X getPosX()const{
        return x;
    }
    X getPosY()const{
        return y;
    }

    X getRow() const{
        return y;
    }

    X getCol() const{
        return x;
    }
// Setter
    void setX(const X _x){
        x = _x;
    }
    void setY(const X _y){
        y = _y;
    }
// Operatoren
    Pos& operator=(const Pos& old){
        x = old.x;
        y = old.y;
        return *this;
    }

    bool operator==(const Pos& other)const{
        return (x == other.x) && (y == other.y);
    }
    bool operator!=(const Pos& other) const{
        return (x != other.x) || (y != other.y);
    }
    bool operator<(const Pos& other)const{
        if(x < other.x) return true;
        if(x > other.x) return false;
        return y < other.y;
    }
// tostring
 //   QString toString()const;
protected:
    X x, y;
};
}
}
typedef Solver::Data::Pos<uint> Position;
std::ostream& operator<<(std::ostream& out, const Solver::Data::Pos<uchar>& p);
std::ostream& operator<<(std::ostream& out, const Solver::Data::Pos<unsigned int>& p);
#endif // POS_H
