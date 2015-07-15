/**
    @Author: Sebastian Hahn
    @Email: seb.hahn1991@googlemail.com
This class Represent the Modulo Field. It has the Size n x m and a Modul.
Each Cell is a Modulo n Adder.
*/
#ifndef FIELD_H
#define FIELD_H
#include <vector>
#include <iostream>
#include <exception>
/**************************************************************************************************
 *                              Typedefs
 * ***********************************************************************************************/
/**
 * @brief uchar short for unsigned char
*/
typedef unsigned char uchar;
/**
 * @brief uint short form for unsigned int
 */
typedef unsigned int uint;


/**************************************************************************************************
 *                                      Class gamefield
 * ***********************************************************************************************/
namespace Game{
    class Field
    {
    public:
// Constructor
        Field();
        Field(const uint sizeX, const uint sizeY, const uint newMod);
        Field(const Field& other);
//  load
        void load(const std::string& fieldString);
// Getter
        uint getSizeX() const;
        uint getSizeY() const;
        uchar getMod() const;
        uchar getField(const uint x, const uint y)  const;
// Setter
        void setMod(const uchar newMod);
        void setField(const uint x, const uint y, const uint Mod);
        void resize(const uint sizeX, const uint sizeY);
// CopyOperator
        Field& operator=(const Field& other);
    private:
/**
* @brief InnerGameFieldContainer Container for the inner container
*/
        typedef std::vector<uchar> InnerGameFieldContainer;
/**
* @brief OuterGameFieldContainer Container for the game field informations
*/
        typedef std::vector<InnerGameFieldContainer> OuterGameFieldContainer;
        OuterGameFieldContainer fields;
/**
* @brief sizeX Number of Rows in the fields
*/
        uint sizeX;
/**
* @brief sizeY Number of Columns in the field
*/
        uint sizeY;
/**
* @brief mod The modulo for each field
*/
        uint mod;
    };
}

// Output operator
std::ostream& operator<<(std::ostream& out, const Game::Field& other);

#endif // FIELD_H
