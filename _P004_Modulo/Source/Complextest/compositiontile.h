#ifndef COMPOSITIONTILE_H
#define COMPOSITIONTILE_H
#include <vector>
#include "../Modulo/tile.h"
#include "../Modulo/game.h"

typedef unsigned int uint;

class compositionTile
{
public:
    compositionTile(const Game::Game& g){
        std::vector<int> buffer;
        for(uint i=0; i<g.getField().getSizeX();++i){
            buffer.push_back(0);
        }
        for(uint i=0; i<g.getField().getSizeY();++i){
            data.push_back(buffer);
        }
        mod = g.getField().getMod();
    }

    void setMod(int _mod){
        mod = _mod;
    }
    void setField(int x, int y, int value){
        data[x][y] = value % mod;
    }

    bool operator<(const compositionTile& other) const {
        if(other.data.size() != other.data.size()) return false;
        for(uint i=0; i<other.data.size(); ++i){
            if(other.data[i].size() != data[i].size()){
                return false;
            }
        }
        for(uint x=0; x<data.size(); ++x){
            for(uint y=0; y<data[x].size(); ++y){
                if(data[x][y] < other.data[x][y]) return true;
                if(data[x][y] > other.data[x][y]) return false;
            }
        }
        return false;
    }

    compositionTile& operator=(const compositionTile& other){
        data = other.data;
        mod = other.mod;
        return *this;
    }

    void addTile(uint posX, uint posY, const Tile& t){
        if(t.getSizeX() + posX >= data.size()){
            std::cerr << "FEHLER ADDTile";
        }
        if(t.getSizeY() + posY >= data[0].size()){
            std::cerr << "FEHLER ADDTile";
        }

        for(uint x=0; x<t.getSizeX(); ++x)
            for(uint y=0; y<t.getSizeY(); ++y){
                data[x+posX][y+posY] += t.getField(x,y) % mod;
            }

    }

private:
    std::vector<std::vector<int> > data;
    int mod;
};

#endif // COMPOSITIONTILE_H
