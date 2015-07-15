#include "field.h"
#include "../Defines/compileoptions.h"
#include "../Exceptions/stringexception.h"
#include "../Exceptions/modiszero.h"
#include <cmath>

using namespace Game;
/**************************************************************************************************
 *                                          Constructor
 * *****************************+******************************************************************/
/**
    Default Constructor
 * @brief Field::Field
*/
Field::Field():
    sizeX(0), sizeY(0), mod(0){}

/**
  create a empty Field.
 * @brief Field::Field:
 * @param _sizeX: The size X
 * @param _sizeY: The size Y
 * @param _newMod: The Modulo adder n
 */
Field::Field(const uint _sizeX, const uint _sizeY, const uint _newMod):
    sizeX(_sizeX), sizeY(_sizeY), mod(_newMod)
{
    InnerGameFieldContainer buffer;
    for(uint x=0; x<sizeX; ++x){
        buffer.push_back(0);
    }
    for(uint y=0; y<sizeY; ++y){
        fields.push_back(buffer);
    }
}

/**
Copy Constructor
 * @brief Field::Field
 * @param other
*/
Field::Field(const Field& other):
    fields(other.fields),
    sizeX(other.sizeX), sizeY(other.sizeY),
    mod(other.mod){}

/**************************************************************************************************
 *                                  getter
 * ***********************************************************************************************/
/**
 * @brief Field::getSizeX
 * @return The size X
 */
uint Field::getSizeX() const{
    return sizeX;
}
/**
 * @brief Field::getSizeY
 * @return The size Y
 */
uint Field::getSizeY() const{
    return sizeY;
}
/**
 * @brief Field::getMod
 * @return the modular value
 */
uchar Field::getMod() const{
    return mod;
}
/**
 * @brief Field::getField
 * @param x: Position x
 * @param y: Position y
 * @return The Modular value of Position (x,y)
 */
uchar Field::getField(const uint x, const uint y) const{
    return fields[y][x];
}

/**************************************************************************************************
 *                                      Setter
 * ***********************************************************************************************/
/**
 * @brief Field::resize ändert die größe des feldes. Setzt alle Felder auf 0.
 * @param sizeX new count of rows
 * @param sizeY new count of columns
 */
void Field::resize(const uint _sizeX, const uint _sizeY){
    sizeX = _sizeX;
    sizeY = _sizeY;
    fields.clear();
    InnerGameFieldContainer buffer;
    for(uint x=0; x<sizeX; ++x){
        buffer.push_back(0);
    }
    for(uint y=0; y<sizeY; ++y){
        fields.push_back(buffer);
    }
}

/**
  chance Mod to the param newMod
 * @brief Field::setMod
 * @param newMod: The new Mow
 */
void Field::setMod(const uchar newMod){
    if(newMod == 0){
        Exception::ModIsZero t;
        throw t;
    }
    mod = newMod;
}

/**
  chance the value of Field (x, y) to mod
 * @brief setField
 * @param x: the x position
 * @param y: The y position
 * @param mod: The new Mod
*/

void Field::setField(const uint _x, const uint _y, const uint _mod){
// Test before
    if(_x >= fields.size()){
        throw Exception::StringException("The size x is to big");
    }
    if(_y >= fields[0].size()){
        throw Exception::StringException("The size y is to big");
    }
    if(_mod >= mod){
        throw Exception::StringException("The mod is to big");
    }
// The setting
    fields[_x][_y] = _mod;
}

/**************************************************************************************************
 *                                          load
 * ***********************************************************************************************/
/**
   Get the information of the gamefield from a String.
   The String has following Structure:
   Allowed Symbols: 0,1,2,3,4,5,6,7,8,9 and ","
   0-9 is Gives a Adder the value. , is a newline command
    You can build a Modulo n Adder
 * @brief Field::load
 * @param fieldString
*/
void Field::load(const std::string& fieldString){
    fields.clear();
    InnerGameFieldContainer buffer;
    for(uchar c : fieldString){
        if(c == ','){
            fields.push_back(buffer);
            buffer.clear();
        }else{
            c -= '0';
            if(c>=mod){
                std::cerr << "Maybe an error: the input is greater than the mod" << std::endl;
                mod=c+1;
            }
            buffer.push_back(c);
        }
    }
    fields.push_back(buffer);
    sizeY = fields.size();
    sizeX = fields[0].size();
}

/**************************************************************************************************
 *                                      operator
 * ***********************************************************************************************/
/**
  Copy operator
 * @brief Field::operator =
 * @param other
 * @return New Copy of this Element
*/
Field& Field::operator=(const Field& other){
    sizeX = other.sizeX;
    sizeY = other.sizeY;
    mod = other.mod;
    fields = other.fields;
    return *this;
}

/**************************************************************************************************
 *                                  output
 * ***********************************************************************************************/

std::ostream& operator<<(std::ostream& out, const Field& other){
    uint countLineStrips = (3 + std::ceil(std::log10(other.getMod()))) * other.getSizeX() + 1;
    for(uint i=0; i< countLineStrips; ++i){
        out << "-";
    }
    out << std::endl;
    for(uint y=0; y<other.getSizeY(); ++y){
        out << "|";
        for(uint x=0; x<other.getSizeX(); ++x){
            out << " " <<(int)other.getField(x,y) << " |";
        }
        out << std::endl;
        for(uint i=0; i< countLineStrips; ++i){
            out << "-";
        }
        out << std::endl;

    }
    return out;
}
