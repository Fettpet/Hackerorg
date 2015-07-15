#ifndef DEPTHRESTART_H
#define DEPTHRESTART_H
#include <exception>

class DepthRestart: std::exception
{
public:
    DepthRestart(){}

    virtual const char* what() const throw(){
        return "No Way Found";
    }
};
#endif // DEPTHRESTART_H
