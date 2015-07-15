#include "pos.h"
#include <ostream>
#include <sstream>

/**************************************************************************************************
 *                                  output
 * ***********************************************************************************************/
std::ostream& operator<<(std::ostream& out, const Pos<uchar>& p){
    out << "a" << p.getX() << "_" << p.getY() << "_" << p.getValue();
    return out;
}

std::ostream& operator<<(std::ostream& out, const Pos<uint>& p){
    out << "a" << p.getX() << "_" << p.getY() << "_" << p.getValue();
    return out;
}
