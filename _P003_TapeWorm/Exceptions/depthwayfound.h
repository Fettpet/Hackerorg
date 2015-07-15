#ifndef DEPTHWAYFOUND_H
#define DEPTHWAYFOUND_H
#include <exception>
#include "World/worlddeep.h"
class DepthWayFound: std::exception
{
public:
    DepthWayFound(){}
    DepthWayFound(const DepthWayFound& other):
        w(other.w)
    {}

    virtual const char* what() const throw(){
        return "No Way Found";
    }
    WorldDeep w;
};

#endif // DEPTHWAYFOUND_H
