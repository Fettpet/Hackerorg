#include "statistic.h"
#include "../IO/IO.h"
#include <sstream>
#include <vector>
#include <set>
#include "compositiontile.h"
/**************************************************************************************************
 *                      Constructor
 * ***********************************************************************************************/
complextest::statistic::statistic()
{
}

/**************************************************************************************************
 *                                          Statistik test
 * ********************************************************************************************////


void complextest::statistic::createStatisticFile(const std::string& fileName, const std::string& user, const std::string& pass, uint countTests, uint level){
    std::fstream statFile;
    statFile.open(fileName, std::ios::out);
    for(uint i=0; i<countTests; ++i){
        IO::gotoLevel(user, pass, level);

    }

}

/**************************************************************************************************
 *                          field analysy
 * ***********************************************************************************************/

double complextest::statistic::fieldAnalysis(const Game::Game field){
    long long int nenner(0), zaehler(0);
    std::set< compositionTile> set;
    for(const auto& tile1 : field.getTiles()){
        for(const auto& tile2: field.getTiles()){
            if(tile1.getId() == tile2.getId()) continue;
            set.clear();
            for(uint x1=0; x1<=field.getField().getSizeX() -tile1.getSizeX(); ++x1){
                for(uint y1=0; y1<=field.getField().getSizeY() - tile1.getSizeY(); ++y1){
                    std::cout << "x: " << x1 << "y:" << y1 << std::endl;
                    for(uint x2=0; x2 < field.getField().getSizeX() - tile2.getSizeX(); ++x2){
                        for(uint y2=0; y2 < field.getField().getSizeY() - tile2.getSizeY(); ++y2){
                            compositionTile tile(field);
                            tile.addTile(x1, y1, tile1);
                            tile.addTile(x2,y2, tile2);
                            set.insert(tile);
                            zaehler++;
                        }
                    }
                }
            }
            nenner +=set.size();
        }
    }
    std::cout << "Nenner: " << nenner << " Zähler: " << zaehler << std::endl;

    return 0;
}
