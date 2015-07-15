#include "int_gf2.h"
#include <cmath>

/**************************************************************************************************
 *                                  Constructors
 * ***********************************************************************************************/
int_gf2::int_gf2():
    value(0)
{
}

int_gf2::int_gf2(int v):
    value(std::abs(v&1)){
}
int_gf2::int_gf2(bool v):
    value(v){

}

int_gf2::int_gf2(float v):
    value((int)v &1){
}

int_gf2::int_gf2(double v):
    value((int)v &1){
}

/**************************************************************************************************
 *                          Converter to standart values
 * ***********************************************************************************************/

int_gf2::operator double() const{
    return value;
}
/*
int_gf2::operator double() const{
    return value;
}

int_gf2::operator float() const{
    return value;
}

int_gf2::operator int() const{
    return value;
}
*/
/**************************************************************************************************
 *                                          operators + -  / * %
 * ***********************************************************************************************/
const int_gf2 int_gf2::operator+(int_gf2 const& rhs){
    return (value ^ rhs.value);
}

const int_gf2 int_gf2::operator-(int_gf2 const& rhs){
    return (value ^ rhs.value);
}

const int_gf2 int_gf2::operator/(int_gf2 const& rhs){
    return (int)std::abs(value / rhs.value) & 1;
}

const int_gf2 int_gf2::operator*(int_gf2 const& rhs){
    return (value & rhs.value);
}

const int_gf2 int_gf2::operator%(int_gf2 const& rhs){
    return (value % rhs.value) &1;
}
/**************************************************************************************************
 *                  Copy Operators with = + - / *
 * ***********************************************************************************************/

int_gf2& int_gf2::operator=(const int_gf2& o){
    value = o.value;
    return *this;
}

int_gf2& int_gf2::operator+=(int_gf2 const& rhs){
    value = (value ^ rhs.value);
    return *this;
}

int_gf2& int_gf2::operator-=(int_gf2 const& rhs){
    value = (value ^ rhs.value);
    return *this;
}

int_gf2& int_gf2::operator*=(int_gf2 const& rhs){
    value = (value & rhs.value);
    return *this;
}

int_gf2& int_gf2::operator/=(int_gf2 const& rhs){
    value = (value & rhs.value);
    return *this;
}

int_gf2& int_gf2::operator%=(int_gf2 const& rhs){
    value = (int)std::abs(value % rhs.value) &1;
    return *this;
}

/**************************************************************************************************
 *                                  compare operators
 * ***********************************************************************************************/
/*
bool int_gf2::operator==( int_gf2 const& rhs){
    return value == rhs.value;
}

bool int_gf2::operator!=( int_gf2 const& rhs){
    return value != rhs.value;
}
*/
bool int_gf2::operator<(int_gf2 const& rhs){
    return value < rhs.value;
}

bool int_gf2::operator<=(int_gf2 const& rhs){
    return value <= rhs.value;
}
/*
bool int_gf2::operator>( int_gf2 const& rhs){
    return value > rhs.value;
}
*/
bool int_gf2::operator>=( int_gf2 const& rhs){
    return value >= rhs.value;
}
/**************************************************************************************************
 *                              getter
 * **********************************************************************************************/
bool int_gf2::getValue() const {
    return value;
}

/**************************************************************************************************
 *                                  ausgabe
 * ************************************************************************************************/

std::ostream& operator<<(std::ostream& out, const int_gf2& t){
    out << t.getValue();
    return out;
}
