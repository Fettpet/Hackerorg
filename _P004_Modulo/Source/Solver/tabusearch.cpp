#include "tabusearch.h"
#include <chrono>
#include <random>
using namespace Solver;

bool TabuSearch::finished;
boost::random::mt19937 TabuSearch::rngSeed;
boost::random::uniform_int_distribution<> TabuSearch::randomSeed(0, 123131231);
/**************************************************************************************************
 *                              Constructor
 * ***********************************************************************************************/
TabuSearch::TabuSearch(const Game::Game& field, uint seed):
    rng(seed),
    SolverBaseClass(field), restartCounter(0), restartValue(field.getTileCount() *__RESTARTCONSTANT__)
{
    positions.resize(field.getTileCount());
    currentValue.resize(field.getField().getSizeX());
    tabuValue.resize(field.getTileCount(), 0);
    for(uint i=0; i< field.getField().getSizeX(); ++i){
        currentValue[i].resize(field.getField().getSizeY(), 0);
    }

    for(uint i=0;i<gameField.getTileCount(); ++i){
        positionsMax.push_back(Pos<uint>(field.getField().getSizeX() - field.getTile(i).getSizeX(), field.getField().getSizeY() - field.getTile(i).getSizeY()));
    }

    restart();
}


/**************************************************************************************************
 *                                  Solver
 * ***********************************************************************************************/
Game::Game TabuSearch::getSolution(){
        Move bestMove;

        uint tile, x, y, partialHeuristic;
        boost::random::uniform_int_distribution<> randomOneTwo(0, 1);
        boost::random::uniform_int_distribution<> tileRandom(0, gameField.getTileCount()-1);
        bool found;
    // Alle möglichkeiten ermitteln
        while(!finished){
            if(heuristicValue == 0) {
                break;
            }
            if(restartCounter == restartValue) restart();
            found = false;

            for(tile=0; tile<gameField.getTileCount(); ++tile){
                if(tabuValue[tile] > 0){
                    tabuValue[tile]--;
                    continue;
                }
                for(x=0; x<positionsMax[tile].getX(); ++x){
                    for(y=0; y<positionsMax[tile].getY(); ++y){
                        partialHeuristic = calcPartialHeuristic(tile, positions[tile].getX(), positions[tile].getY(), x, y);
                        if(partialHeuristic < heuristicValue){
                            move(tile, Pos<uint>(x,y));
                            found = true;
                            tabuValue[tile] = gameField.getTileCount()  >> 1;
                            break;
                        }
                    }
                    if(found) break;
                }
                if(found) break;
            }
    // Random Move
            if(!found){
                restartCounter++;
                tile = tileRandom(rng);
                tabuValue[tile] = gameField.getTileCount() >> 1;
                boost::random::uniform_int_distribution<> posXRandom(0, positionsMax[tile].getX());
                boost::random::uniform_int_distribution<> posYRandom(0, positionsMax[tile].getY());
                x = posXRandom(rng);
                y = posYRandom(rng);
                move(tile, Pos<uint>(x, y));
            }

        }
        if(!finished){
            for(int i=0;i<gameField.getTileCount(); ++i){

                gameField.getTile(i).setSolutionX(positions[i].getX());
                gameField.getTile(i).setSolutionY(positions[i].getY());
            }
            return gameField;
        }
        return Game::Game();

}

/**************************************************************************************************
 *                              Help functions
 * ***********************************************************************************************/

/**
 Start a new calculation
 * @brief TabuSearch::restart
 */

void TabuSearch::restart(){
    std::cout << "Restart Value: " << heuristicValue << std::endl;
    for(uint i=0; i<gameField.getTileCount(); ++i){
        boost::random::uniform_int_distribution<> posX(0, positionsMax[i].getX());
        boost::random::uniform_int_distribution<> posY(0, positionsMax[i].getY());
        positions[i] = Pos<uint>(posX(rng), posY(rng));
        tabuValue[i] = 0;
    }
    restartCounter = 0;
    getTheCurrentValues();
    heuristicValue = calcHeuristic();
}

/**
  calculate the values for each modulo k adder
 * @brief TabuSearch::getTheCurrentValues
 * @return
 */
void TabuSearch::getTheCurrentValues(){
    uint x, y, i, startX, startY;
// Start value
    for(x=0; x<gameField.getField().getSizeX(); ++x)
        for(y=0; y<gameField.getField().getSizeY(); ++y){
            currentValue[x][y] = gameField.getField().getField(x, y);
        }

// Tiles addieren
    for(i=0; i<gameField.getTileCount(); ++i){
        startX = positions[i].getX();
        startY = positions[i].getY();
        for(x=0; x<gameField.getTile(i).getSizeX(); ++x)
            for(y=0; y<gameField.getTile(i).getSizeY(); ++y){
                currentValue[x+ startX][y+ startY] += gameField.getTile(i).getField(x, y);
            }
    }
// Modulo rechnen
    for(x=0; x<gameField.getField().getSizeX(); ++x)
        for(y=0; y<gameField.getField().getSizeY(); ++y){
            currentValue[x][y] %= gameField.getField().getMod();
        }
}

/**
  Calculate the Hash value;
 * @brief TabuSearch::calcHash
 * @return
 */
uint TabuSearch::calcHeuristic() const{
    uint x, y;
    uint result(0);
    for(x=0; x<gameField.getField().getSizeX(); ++x)
        for(y=0; y<gameField.getField().getSizeY(); ++y){
            if(currentValue[x][y] != 0)
                result += gameField.getField().getMod() - currentValue[x][y];
        }
    return result;
}

/**
 * @brief calcPartialHash
 * @param tile
 * @param startX
 * @param startY
 * @param goalX
 * @param goalY
 * @return
 */
uint TabuSearch::calcPartialHeuristic(const uint tile, const uint startX, const uint startY, const uint goalX, const uint goalY) const{
    uint x, y;
    uint result(heuristicValue);
// Undo
    for(x=0; x<gameField.getTile(tile).getSizeX(); ++x)
        for(y=0; y<gameField.getTile(tile).getSizeY(); ++y){
            if(gameField.getTile(tile).getField(x, y) == true){
                if(currentValue[startX + x][startY + y] == 1){
                    result -= gameField.getField().getMod()-1;
                }else{
                    result += 1;
                }
            }
        }
// Set new
    for(x=0; x<gameField.getTile(tile).getSizeX(); ++x)
        for(y=0; y<gameField.getTile(tile).getSizeY(); ++y){
            if(gameField.getTile(tile).getField(x, y) == true){
                if(currentValue[goalX + x][goalY + y] == 0){
                    result += gameField.getField().getMod()-1;
                }else{
                    result -= 1;
                }
            }
        }
    return result;
}

/**************************************************************************************************
 *                                      Move
 * ***********************************************************************************************/
void TabuSearch::move(uint tile, Pos<uint> newPos){
    uint x, y;
// undo
        heuristicValue = calcPartialHeuristic(tile, positions[tile].getX(), positions[tile].getY(), newPos.getX(), newPos.getY());
    for(x=0; x<gameField.getTile(tile).getSizeX(); ++x)
        for(y=0; y<gameField.getTile(tile).getSizeY(); ++y){
            if(gameField.getTile(tile).getField(x, y) == true){
                if(currentValue[positions[tile].getX() + x][positions[tile].getY() + y] == 0){
                    currentValue[positions[tile].getX() + x][positions[tile].getY() + y] = gameField.getField().getMod()-1;
                }else{
                    currentValue[positions[tile].getX() + x][positions[tile].getY() + y]--;
                }
            }
        }
    for(x=0; x<gameField.getTile(tile).getSizeX(); ++x)
        for(y=0; y<gameField.getTile(tile).getSizeY(); ++y){
            if(gameField.getTile(tile).getField(x, y) == true){
                if(currentValue[newPos.getX() + x][newPos.getY() + y] == gameField.getField().getMod()-1){
                    currentValue[newPos.getX()+ x][newPos.getY() + y] = 0;
                }else{
                    currentValue[newPos.getX() + x][newPos.getY()+ y]++;
                }
            }
        }

    positions[tile] = newPos;
}

/**************************************************************************************************
 *                              Debugging
 * ***********************************************************************************************/
void TabuSearch::printCurrentValues(){
    uint x, y;
    for(y=0; y<gameField.getField().getSizeY(); ++y){
        for(x=0; x<gameField.getField().getSizeX(); ++x){
            std::cout << currentValue[x][y] << " ";
        }
        std::cout << std::endl;
    }
}

void TabuSearch::printTiles(){
    for(uint i=0; i< gameField.getTileCount(); ++i){
        std::cout << positions[i] <<  std::endl << gameField.getTile(i) << std::endl;
    }
}

/**************************************************************************************************
 *                              Parallel
 * ***********************************************************************************************/
void TabuSearch::setFinished(){
    finished = true;
}

void TabuSearch::setNewGame(){
    finished = false;
}
