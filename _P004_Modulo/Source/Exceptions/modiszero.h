/**
    This class represent a Error, if the Modular value is zero in class game.
*/
#ifndef MODISZERO_H
#define MODISZERO_H
#include <exception>

namespace Exception{
    class ModIsZero : public std::exception
    {
    public:
        ModIsZero(){}

        virtual const char* what() const throw(){
            return "Mod can not be zero";
        }
    };
}

#endif // MODISZERO_H
