#ifndef EXACTLYONE_H
#define EXACTLYONE_H

#include <set>
#include <sstream>

class ExactlyOne
{
public:
    static int getExactlyOne(std::set<int>& vars, std::stringstream& out );
};

#endif // EXACTLYONE_H
