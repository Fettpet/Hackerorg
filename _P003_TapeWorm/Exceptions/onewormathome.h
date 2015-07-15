#ifndef ONEWORMATHOME_H
#define ONEWORMATHOME_H
#include "World/worm.h"
#include "World/world.h"
class OneWormAtHome: public std::exception
{
public:
    OneWormAtHome();
    OneWormAtHome(const OneWormAtHome& other): w(other.w), wurm(other.wurm){}
    OneWormAtHome(const Worm& wur, const world& wo): w(wo), wurm(wur){}

    virtual const char* what() const throw(){
        return "Worm At Home";
    }
    world w;
    Worm wurm;
};

#endif // ONEWORMATHOME_H
