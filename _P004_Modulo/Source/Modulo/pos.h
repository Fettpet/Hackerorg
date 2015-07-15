#ifndef POS_H
#define POS_H
#include <iostream>
#include <cmath>
#include <string>

typedef unsigned char uchar;
template< class T>
class Pos
{
public:
/**
    Default constructor. Do nothing
* @brief Pos
*/
    Pos(){}
/**
  Set the x and the y to a value
  The value value is default
* @brief Pos
* @param _x: the x value
* @param _y: the y value
*/
    Pos(T _x, T _y):
        x(_x), y(_y)
        {}
/**
  Set x,y and value to the given parameter
* @brief Pos
* @param x: the x value
* @param y: the y value
* @param value: the value value
*/
    Pos(T _x, T _y, T _value):
        x(_x), y(_y), value(_value)
        {}
/**
Copy constructor
* @brief Pos
* @param old: The 0ld Pos
*/
    Pos(const Pos& old):
        x(old.x), y(old.y), value(old.value){}
// getter
/**
  return the x value
* @brief getX
* @return
*/
    T getX()const{
        return x;
    }
/**
  return the y value
* @brief getY
* @return
*/
    T getY()const{
        return y;
    }
/**
return the value
* @brief getValue
* @return
*/
    T getValue()const{
        return value;
    }
// Setter
/**
  set the value x to the param value
* @brief setX
* @param _x
*/
    void setX(const T& _x){
        x = _x;
    }
/**
set the valu y to the param value;
* @brief setY
* @param _y
*/
    void setY(const T& _y){
        y = _y;
    }
/**
set the value value to the parameter Value
* @brief setValue
* @param value
*/
    void setValue(const T _value){
        value = _value;
    }

// Operatoren
/**
copy operator
* @brief operator =
* @param old
* @return
*/
    Pos& operator=(const Pos& old){
        x = old.x;
        y = old.y;
        value = old.value;
        return *this;
    }
/**
compare the objects to equality
*@brief operator ==
* @param other
* @return
*/
    bool operator==(const Pos& other)const{
        if(x != other.x) return false;
        if(y != other.y) return false;
        if(value != other.value) return false;
        return true;
    }
/**
a lexicalic ordering
* @brief operator <
* @param other
* @return
*/
    bool operator<(const Pos& other)const{
        if(x < other.x) return true;
        if(x > other.x) return false;
        if(y < other.y) return true;
        if(y > other.y) return false;
        if(value < other.value) return true;
        return false;
    }
private:
    T x, y, value;
};

typedef Pos<uint> PosInt;

std::ostream& operator<<(std::ostream& out, const Pos<uchar>& p);
std::ostream& operator<<(std::ostream& out, const Pos<uint>& p);

#endif // POS_H
