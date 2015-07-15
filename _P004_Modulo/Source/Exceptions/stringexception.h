/**
  This class is a container class for Errors.


*/

#ifndef STRINGEXCEPTION_H
#define STRINGEXCEPTION_H
#include <exception>
#include <string>

namespace Exception{
    class StringException : public std::exception
    {
    public:
// Constructor
        StringException():w("No error specified"){}
        StringException(const std::string& ex): w(ex){}
// what
        virtual const char* what() const throw(){
            return w.c_str();
        }
    private:
        std::string w;
    };
}
#endif // STRINGEXCEPTION_H
