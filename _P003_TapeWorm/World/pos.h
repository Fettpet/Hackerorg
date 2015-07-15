#ifndef POS_H
#define POS_H
#include <iostream>
#include <cmath>
typedef unsigned char uchar;

template<typename X>
class Pos
{
public:
    Pos(){}
    Pos(X _x, X _y):
        x(_x), y(_y)
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
private:
    X x, y;
};

std::ostream& operator<<(std::ostream& out, const Pos<uchar>& p);

#endif // POS_H
