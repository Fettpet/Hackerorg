#include "recolor.h"

void Recolor::getValidWormsNewColors(world& w){
// 1. Aufteilen der Würmer nach Farbe
    std::vector< std::vector< uint > > wurmColors;
    seperateWorms(w, wurmColors);
//2. Einen Wurm mit eindeutiger Länge finden
    for(uint color=0; color< wurmColors.size(); ++color){
        if(wurmColors[color].size() < 2) continue;
        for(uint first=0; first< wurmColors[color].size(); ++first){
            bool found=false;
            for(uint second=0; second<wurmColors[color].size(); ++second){
                if(first == second) continue;
                if(w.getWorm(wurmColors[color][first]).getLength() == w.getWorm(wurmColors[color][second]).getLength()){
                    found = true;
                    break;
                }
            }
            if(!found){
                std::vector<WormPart> positions;
                if(foundSeperatePartInMap(w, wurmColors[color][first], positions)){
                    w.getWorm(wurmColors[color][first]).setColor(wurmColors.size());
                    for(const auto& p: positions){
                        w.getWormMap().setTheHomeFor(p.getX(), p.getY(), wurmColors.size());
                    }
                    std::vector<uint> buffer;
                    std::vector<uint>::iterator it = wurmColors[color].begin();
                    it += first;
                    buffer.push_back(wurmColors[color][first]);
                    wurmColors.push_back(buffer);
                    //wurmColors.erase(it);
                    wurmColors[color].erase(it);

                }
            }
        }
    }
/*
 1. Aufteilen der Würmer nach Farbe
 2. Nachschauen ob es Würmer gibt die eine Eindeutige Länge haben
 3. Nachschauen ob dieses Feld Seperat liegt
 4. Umfärben
*/
}

/**************************************************************************************************
 *                                      Helpfunctions
 * ***********************************************************************************************/
/**
Teilt die Würmer nach Farben auf
 * @brief seperateWorms
 * @param w
 * @param result
 * debuggt
*/
void Recolor::seperateWorms(world& w, std::vector< std::vector< uint > >& result){
    const uint colorCount(w.getWormMap().getColorCount());
    for(uint curColor=result.size(); curColor<=colorCount; ++curColor){
        result.push_back( std::vector< uint>());
    }
    for(uint i=0; i<w.getWormCount(); ++i){
        result[w.getWorm(i).getColor()].push_back(i);
    }
#if DEBUG == 1
    for(int i=0; i<result.size(); ++i){
        std::cout << "Color " << i << " hat folgende Elemente: ";
        for(int j=0; j< result[i].size(); ++j){
            std::cout << result[i][j] << ", ";
        }
        std::cout << std::endl;
    }
#endif
}

bool Recolor::foundSeperatePartInMap(const world& w, const uint wurmId, std::vector<WormPart>& positions){
// create a visited map
    const register uint sizeX(w.getSizeX()), sizeY(w.getSizeY());
    const uint wormColor(w.getWorm(wurmId).getColor());
    std::vector< std::vector< bool > > visited;
    for(uint i=0; i<sizeY; ++i){
        std::vector<bool> buffer;
        for(uint j=0; j<sizeX; ++j){
            buffer.push_back(false);
        }
        visited.push_back(buffer);
    }
// Die Visited map durchgehen
    for(uint x=0; x<sizeX; ++x){
        for(uint y=0; y<sizeY; ++y){
            if((!visited[y][x]) && (w.getWormMap().getTheHomeFor(x,y) == wormColor)){
                if(getLength(w, visited, x,y, wormColor, positions) == w.getWorm(wurmId).getLength()){
                    return true;
                }
                positions.clear();
            }
        }
    }
    return false;
}

uint Recolor::getLength(const world& w, std::vector< std::vector< bool> >& visitedMap, uint x, uint y, uint color, std::vector<WormPart>& positions){
    NeighborContainer mustVisited;
    mustVisited.push_back(WormPart(x,y));

    while(!mustVisited.empty()){
        WormPart cur = mustVisited[mustVisited.size()-1];
        mustVisited.pop_back();
        if((w.getWormMap().getTheHomeFor(cur.getX(), cur.getY()) == color) && (!visitedMap[cur.getX()][cur.getY()])){
            visitedMap[cur.getX()][cur.getY()] = true;
            positions.push_back(cur);
            NeighborContainer* buffer = w.getNeighborFields(cur);
            for( auto p: *buffer)
                mustVisited.push_back(p);
        }
    }

    return positions.size();
}
