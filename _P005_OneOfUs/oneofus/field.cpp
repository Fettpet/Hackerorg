#include "fieldd.h"
#include <cassert>
#include <set>
#include <cstdlib>
#include <iostream>
#include <map>
#include <QString>
/**************************************************************************************************
 *                                              Construktors
 *************************************************************************************************/
/**
 * @brief Field::Field
 * @param fileName: A string
 */
Field::Field(std::string inputString)
{
    color.clear();
    form.clear();
//x=3&y=3&board=CbBbCbAaCbCaCbAbCb
    QString buffer(inputString.c_str());
    buffer = buffer.mid(2);
    sizeX = buffer.mid(0, buffer.indexOf("&")).toInt();

    buffer = buffer.mid(buffer.indexOf("=")+1);
    sizeY = buffer.mid(0, buffer.indexOf("&")).toInt();

    buffer = buffer.mid(buffer.indexOf("=")+1);
    std::cout << "sizeX: " << sizeX << " sizeY: "<< sizeY << " rest: " << buffer.toStdString() << std::endl;
    for(int x=0; x<sizeX; ++x){
        std::vector<short> bufferColor, bufferForm;
        for(int y=0; y<sizeY; ++y ){
            bufferForm.push_back(buffer[2*(x+y*sizeX)].toLatin1());
            bufferColor.push_back(buffer[2*(x+y*sizeX)+1].toLatin1());
        }
        color.push_back(bufferColor);
        form.push_back(bufferForm);
    }
    std::cout << "Form:" << std::endl;

    for(int y=0; y<sizeY; ++y){
        for(int x=0; x<sizeX; ++x ){
            std::cout << form[x][y];
        }
        std::cout << std::endl;
    }
}

/**************************************************************************************************
 *                                  Getter
 *************************************************************************************************/
short Field::getColorCount() const{
    short result(0);
    for(int x=0; x<sizeX; ++x){
        for(int y=0; y<sizeY; ++y ){
            result = std::max(result, color[x][y]);
        }
    }
    return result - 'a';
}
short Field::getFormCount() const{
    short result(0);
    for(int x=0; x<sizeX; ++x){
        for(int y=0; y<sizeY; ++y ){
            result = std::max(result, form[x][y]);
        }
    }
    return result - 'A';
}

short Field::getSizeX()const{
    return sizeX;
}

short Field::getSizeY()const{
    return sizeY;
}

short Field::getColor(short x, short y) const{
    return color[x][y];
}
short Field::getForm(short x, short y) const{
    return form[x][y];
}

/**************************************************************************************************
 *                                  Setter
 *************************************************************************************************/




/**************************************************************************************************
 *                                  output
 *************************************************************************************************/
/**
    overload the standard output
*/

std::ostream& operator <<(std::ostream& out, const Field& f){
    std::cout << "Color: " << std::endl;
    for(int y=0; y<f.getSizeY(); ++y){
        for(int x=0; x<f.getSizeX(); ++x){
            std::cout << (char)f.getColor(x,y);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << "Form: " << std::endl;
    for(int y=0; y<f.getSizeY(); ++y){
        for(int x=0; x<f.getSizeX(); ++x){
            std::cout << (char)f.getForm(x,y);
        }
        std::cout << std::endl;
    }
    return out;
}

