#include "deepsearch.h"

/**************************************************************************************************
 *                                  Construcot
 * ***********************************************************************************************/
Solver::DeepSearch::DeepSearch(const Game::Game& field):
    SolverBaseClass(field)
{
    positions.resize(field.getTileCount());
    currentValue.resize(field.getField().getSizeX());
    for(uint i=0; i< field.getField().getSizeX(); ++i){
        currentValue[i].resize(field.getField().getSizeY(), 0);
    }
    for(uint x=0; x<field.getField().getSizeX(); ++x){
        for(uint y=0; y<field.getField().getSizeY(); ++y){
            currentValue[x][y] = field.getField().getField(x,y);
        }
    }
    for(uint i=0;i<gameField.getTileCount(); ++i){
        positionsMax.push_back(Pos<uint>(field.getField().getSizeX() - field.getTile(i).getSizeX()+1,1+ field.getField().getSizeY() - field.getTile(i).getSizeY()));
        std::cout << "Max Pos: " <<positionsMax[i] << std::endl;
    }

    heuristicValue = calcHeuristic();

    std::vector<bool> used;
    used.resize(field.getTileCount(), false);
    tileObserveList.resize(field.getTileCount());
    for(uint i=0; i<used.size(); ++i){
        uint filledValue(9999999);
        uint filledNumber(0);
        for(uint tile=0; tile<field.getTileCount(); ++tile){
            if(used[tile]) continue;
            if(gameField.getTile(tile).getCountSet() < filledValue){
                filledValue = gameField.getTile(tile).getCountSet();
                filledNumber = tile;
            }
        }
        used[i] = true;
        tileObserveList[i] = filledNumber;
    }

    maxFlips.resize(field.getTileCount(), false);
    maxFlips[field.getTileCount()-1] = field.getTile(tileObserveList[field.getTileCount()-1]).getCountSet();
    for(int i=field.getTileCount()-2; i>=0;--i){
        maxFlips[i] = maxFlips[i+1] + field.getTile(tileObserveList[i]).getCountSet();
    }

}

/**************************************************************************************************
 *                                      Heuristics
 * ***********************************************************************************************/



/**
  Calculate the Hash value;
 * @brief TabuSearch::calcHash
 * @return
 */
uint Solver::DeepSearch::calcHeuristic() const{
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
uint Solver::DeepSearch::calcPartialHeuristic(const uint tile, const uint goalX, const uint goalY) const{
    uint x, y;
    uint result(heuristicValue);
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
void Solver::DeepSearch::move(uint tile, Pos<uint> newPos){
    uint x, y;
    for(x=0; x<gameField.getTile(tile).getSizeX(); ++x)
        for(y=0; y<gameField.getTile(tile).getSizeY(); ++y){
            if(gameField.getTile(tile).getField(x, y) == true){
                if(currentValue[newPos.getX() + x][newPos.getY() + y] == gameField.getField().getMod()-1){
                    currentValue[newPos.getX()+ x][newPos.getY() + y] = 0;
                    heuristicValue--;
                }else if (currentValue[newPos.getX() + x][newPos.getY() + y] == 0){
                    currentValue[newPos.getX() + x][newPos.getY()+ y] = 1;
                    heuristicValue += gameField.getField().getMod()-1;
                } else {
                    currentValue[newPos.getX()+ x][newPos.getY() + y]++;
                    heuristicValue--;
                }
            }
        }
    positions[tile] = newPos;
}


void Solver::DeepSearch::backtrack(uint tile){
    Pos<uint> newPos = positions[tile];
    uint x, y;
    for(x=0; x<gameField.getTile(tile).getSizeX(); ++x)
        for(y=0; y<gameField.getTile(tile).getSizeY(); ++y){
            if(gameField.getTile(tile).getField(x, y) == true){
                if(currentValue[newPos.getX() + x][newPos.getY() + y] == 0){
                    currentValue[newPos.getX()+ x][newPos.getY() + y] = gameField.getField().getMod()-1;
                    heuristicValue++;
                }else if (currentValue[newPos.getX() + x][newPos.getY() + y] == 1){
                    currentValue[newPos.getX() + x][newPos.getY()+ y] = 0;
                    heuristicValue -= gameField.getField().getMod() -1;
                } else {
                    currentValue[newPos.getX()+ x][newPos.getY() + y]--;
                    heuristicValue++;
                }
            }
        }
}

/**************************************************************************************************
 *                          Deep search
 * ***********************************************************************************************/
/**
 * @brief deepSearch
 * @param cur
 * @param step
 * @return
 */
bool Solver::DeepSearch::deepSearch(const uint step){
    std::cout << "Heur" << heuristicValue << " max " << maxFlips[step] << std::endl;
    if(heuristicValue > maxFlips[step]) return false;
    uint tile = tileObserveList[step];
    for(uint x=0; x< positionsMax[tile].getX(); ++x){
        for(uint y=0; y<positionsMax[tile].getY(); ++y){
            move(tile, Pos<uint>(x, y));
            if(step == gameField.getTileCount()-1){
                if(heuristicValue == 0) return true;
            }else if(deepSearch(step+1)){
                return true;
            }
            backtrack(tile);
        }
    }
    return false;
}
/**************************************************************************************************
 *                          Search
 * ***********************************************************************************************/
Game::Game Solver::DeepSearch::getSolution(){
    deepSearch(0);
    for(int i=0;i<gameField.getTileCount(); ++i){
        gameField.getTile(i).setSolutionX(positions[i].getX());
        gameField.getTile(i).setSolutionY(positions[i].getY());
    }
    return gameField;

}

/**************************************************************************************************
 *                              Debugging
 * ***********************************************************************************************/
void Solver::DeepSearch::printCurrentValues(){
    uint x, y;
    for(y=0; y<gameField.getField().getSizeY(); ++y){
        for(x=0; x<gameField.getField().getSizeX(); ++x){
            std::cout << currentValue[x][y] << " ";
        }
        std::cout << std::endl;
    }
}

void Solver::DeepSearch::printTiles(){
    for(uint i=0; i< gameField.getTileCount(); ++i){
        std::cout << positions[i] <<  std::endl << gameField.getTile(i) << std::endl;
    }
}
