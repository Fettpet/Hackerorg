/**
    // Create a Sudokufield with size 12x12
    Sudokufield test(12);
    // set the field x=8, y=3 to 10
    test(8,3) = 10;
    // Output the complete Sudoku
    cout << test << std::endl;
    // Output the cell x=8, y=3;
    cout << test(8,3) << std::endl;
*/

#ifndef SUDOKUFIELD_H
#define SUDOKUFIELD_H
#include <vector>
#include <iostream>
#include <fstream>
#include "pos.h"
#include <map>

class Field
{
public:
//Constructors
    Field(std::string inputString);
  //Getter
    short getColor(short x, short y) const;
    short getForm(short x, short y) const;
    short getSizeX()const;
    short getSizeY()const;
    short getColorCount() const;
    short getFormCount() const;
  //Setter
    void setColor(short x, short y, short color);
    void setForm(short x, short y, short form);
    void setVarToNumber(const std::map<std::string, int>& varToNumber );
    void setNumberToVar(const std::vector<std::string>& numberToVar );
    
//getter


    
private:
    short sizeX, sizeY;
    std::map<Pos, int> varToNumber;
    std::vector<Pos> numberToVar;
	
    std::vector< std::vector< short> > form, color;
};

std::ostream& operator <<(std::ostream& out, const Field& f);
#endif // SUDOKUFIELD_H
