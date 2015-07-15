#ifndef NOWAYFOUND_H
#define NOWAYFOUND_H
#include <exception>

class NoWayFound: public std::exception
{
public:
    virtual const char* what() const throw(){
        return "No Way Found";
    }
};

#endif // NOWAYFOUND_H
