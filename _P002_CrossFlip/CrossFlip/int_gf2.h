#ifndef INT_GF2_H
#define INT_GF2_H
#define MODVALUE 2
#include <iostream>

class int_gf2
{
public:
    int_gf2();
    int_gf2(int v);
    int_gf2(bool v);
    int_gf2(float v);
    int_gf2(double v);
    operator double() const;
    /*
    operator double() const;
    operator float() const;
    operator int() const;
*/
    int_gf2& operator=(const int_gf2& o);

    const int_gf2 operator+(int_gf2 const& rhs);
    const int_gf2 operator-(int_gf2 const& rhs);
    const int_gf2 operator/(int_gf2 const& rhs);
    const int_gf2 operator*(int_gf2 const& rhs);
    const int_gf2 operator%(int_gf2 const& rhs);

    int_gf2& operator+=(int_gf2 const& rhs);
    int_gf2& operator-=(int_gf2 const& rhs);
    int_gf2& operator*=(int_gf2 const& rhs);
    int_gf2& operator/=(int_gf2 const& rhs);
    int_gf2& operator%=(int_gf2 const& rhs);

//    bool operator==(int_gf2 const& rhs);
  //  bool operator!=(int_gf2 const& rhs);

    bool operator<(int_gf2 const& rhs);
    bool operator<=(int_gf2 const& rhs);
 //   bool operator>(int_gf2 const& rhs);
    bool operator>=(int_gf2 const& rhs);

    bool getValue() const;

private:
    bool value;
};

std::ostream& operator<<(std::ostream& out, const int_gf2& t);

#endif // INT_GF2_H
