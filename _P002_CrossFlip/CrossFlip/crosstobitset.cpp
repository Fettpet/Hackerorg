#include "crosstobitset.h"
#include <parallel/algorithm>
#include "pos.h"
std::vector< boost::dynamic_bitset<> > CrossToBitset::StringList;
QMap<Pos, int> CrossToBitset::PosToInt;
QMap<int, Pos> CrossToBitset::IntToPos;
int CrossToBitset::countVars;
int CrossToBitset::currentPos;
boost::dynamic_bitset<> CrossToBitset::currentBitString;

CrossToBitset::CrossToBitset()
{
}


QString CrossToBitset::solve(const CrossFlip &t){
#if DEBUG == 1
    std::cout << "The CrossFlip Board: " << std::endl;
    std::cout << t << std::endl;
#endif
    createVars(t);
    createStringList(t);
#if DEBUG == 1
    std::cout << "The Bit List before solving:" << std::endl;
    for(int i=0; i< StringList.size(); ++i){
        std::cout << StringList[i] << std::endl;
    }
#endif
    solveSystem();
#if DEBUG == 1
    std::cout << "The Bit List after solving:" << std::endl;
    for(int i=0; i< StringList.size(); ++i){
        std::cout << StringList[i] << std::endl;
    }
#endif
    return readSolution(t);
}


/**************************************************************************************************
 *                              Create Bitset strings
 * ***********************************************************************************************/

void CrossToBitset::createStringList(const CrossFlip &t){
    StringList.clear();
    for(int y=0; y<t.getSizeY(); ++y){
        for(int x=0; x<t.getSizeX(); ++x){

            if(t.isBlocked(x,y)) continue;
            boost::dynamic_bitset<> buffer(countVars+1);
// nach Rechts

            for(int xx = x; xx >= 0; --xx){
                if(t.isBlocked(xx, y)) break;
                buffer.set(PosToInt[Pos(xx, y)], true);
            }

// Nach unten
            for(int xx = x+1; xx< t.getSizeX(); ++xx){
                if(t.isBlocked(xx, y)) break;
                buffer.set(PosToInt[Pos(xx, y)], true);
            }

// Nach Links
            for(int yy=y-1; yy >= 0; --yy){
                if(t.isBlocked(x,yy)) break;
                buffer.set(PosToInt[Pos(x, yy)], true);
            }
// Nach Rechts
            for(int yy=y+1; yy < t.getSizeY(); ++yy){
                if(t.isBlocked(x,yy)) break;
                buffer.set(PosToInt[Pos(x, yy)], true);
            }
            buffer.set(countVars, t.hasStartValue(x,y));

            StringList.push_back(buffer);
        }
    }
}

/**************************************************************************************************
 *                                  Create Vars
 * ***********************************************************************************************/
int CrossToBitset::createVars(const CrossFlip& sys){
    PosToInt.clear();
    IntToPos.clear();
    int count = 0;
    for(int y=0; y<sys.getSizeY(); ++y){
        for(int x=0; x<sys.getSizeX(); ++x){

            if(!sys.isBlocked(x,y)){
#if DEBUG == 1
                std::cout << "(" << x << "," << y << ") = " << count<< std::endl;
#endif
                PosToInt.insert(Pos(x,y), count);
                IntToPos.insert(count, Pos(x,y));
                count++;
            }
        }
    }
    countVars = count;
    return count;
}

/**************************************************************************************************
 *                          solve the linear System
 * ***********************************************************************************************/
void CrossToBitset::solveSystem(){
    createUnderMatrix();
#if DEBUG == 1
    std::cout << "The under empty Matrix" << std::endl;
    for(int i=0; i< StringList.size(); ++i){
        std::cout << StringList[i] << std::endl;
    }
#endif
    createUpperMatrix();
}

void CrossToBitset::DoThread(boost::dynamic_bitset<>& list){
    if(list[currentPos]){
        list ^= currentBitString;
    }
}

void CrossToBitset::createUnderMatrix(){

    for(int pos=0; pos<StringList.size()-1;++pos){
// falls der Wert in der Diagonale 0 ist
        if(!StringList[pos][pos]){
// wird er auf eins gesetzt
            for(int x=pos+1; x<StringList.size(); ++x){
                if(StringList[x][pos]){
                    StringList[pos] ^= StringList[x];
                    break;
                }
            }
        }
/*
        currentBitString = StringList[pos];
        currentPos = pos;
        __gnu_parallel::for_each(StringList.begin(), StringList.begin() + pos -1, DoThread);
//*/

        const register boost::dynamic_bitset<> buffer(StringList[pos]);
// create form
#pragma omp parallel for
        for(int x=pos+1; x<StringList.size(); ++x){
            if(StringList[x][pos]){
                StringList[x] ^= buffer;
            }
        }
//*/
    }
}

void CrossToBitset::createUpperMatrix(){
    for(int pos=1; pos<StringList.size();++pos){
        if(!StringList[pos][pos]) continue;
/*
        currentBitString = StringList[pos];
        currentPos = pos;
        __gnu_parallel::for_each(StringList.begin(), StringList.begin() + pos -1, DoThread);
//*/

        const register boost::dynamic_bitset<> buffer(StringList[pos]);
#pragma omp parallel for
        for(int x=pos-1; x>= 0; --x){
            if(StringList[x][pos]){
                StringList[x] ^= buffer;
            }
        }
//*/
    }
}

QString CrossToBitset::readSolution(const CrossFlip& t){
    int count(0);
    QString result;
    for(int y=0; y<t.getSizeY(); ++y){
        for(int x=0; x<t.getSizeX(); ++x){
            if(t.isBlocked(x,y)){
                result += '0';
            } else {
                if(StringList[count][count]){
                    result += '0' + StringList[count][StringList[count].size()-1];
                } else {
                    result += '0';
                }
                count++;
            }
        }
    }
#if DEBUG == 1
    std::cout << "Solution String: " << result.toStdString() << std::endl;
#endif
    return result;
}
