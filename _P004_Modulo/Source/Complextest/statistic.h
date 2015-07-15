#ifndef STATISTIC_H
#define STATISTIC_H
#include <string>
#include <fstream>
#include "../Modulo/game.h"
namespace complextest{



class statistic
{
public:
    statistic();
    void createStatisticFile(const std::string& fileName, const std::string& user, const std::string& pass, uint countTests, uint level);


    double fieldAnalysis(const Game::Game field);
};

}
#endif // STATISTIC_H
