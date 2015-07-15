#include "cross.h"
#include <iostream>
/**************************************************************************************************
 *                                          Constructor
 * *******************************************************************************************+++*/

CrossFlip::CrossFlip(QByteArray fileData){
    fileData = fileData.mid(fileData.indexOf("var boardinit"));
    fileData = fileData.mid(0, fileData.indexOf("</script>"));

    QByteArray board;
    board = fileData.mid(fileData.indexOf("\"")+1);
    board = board.mid(0, board.indexOf("\""));
    std::vector<bool> bufferBlocked, bufferStartValue;
    for(int i=0; i< board.size(); ++i){
        if(board[i] == ','){
            Blocked.push_back(bufferBlocked);
            startValue.push_back(bufferStartValue);
            bufferBlocked.clear();
            bufferStartValue.clear();
            continue;
        }
        if(board[i] == '0'){
            bufferBlocked.push_back(false);
            bufferStartValue.push_back(false);
        }else if(board[i] == '1'){
            bufferBlocked.push_back(false);
            bufferStartValue.push_back(true);
        }else if(board[i] == '2'){
            bufferBlocked.push_back(true);
            bufferStartValue.push_back(false);
        }
    }
    Blocked.push_back(bufferBlocked);
    startValue.push_back(bufferStartValue);
    QByteArray lev = fileData;
    lev = lev.mid(lev.indexOf("level"));
    lev = lev.mid(lev.indexOf("=")+2);
    lev = lev.mid(0, lev.indexOf(";"));
    level = lev.toInt();
}

/**************************************************************************************************
 *                                          getter
 * ***********************************************************************************************/

int CrossFlip::getSizeX() const{
    return Blocked[0].size();
}
int CrossFlip::getSizeY() const{
    return Blocked.size();
}
int CrossFlip::getLevel() const{
    return level;
}
bool CrossFlip::hasStartValue(int x, int y) const{
    return startValue[y][x];
}
bool CrossFlip::isBlocked(int x, int y) const{
    return Blocked[y][x];
}

/**************************************************************************************************
 *                                              ausgabe
 * ***********************************************************************************************/
std::ostream& operator<<(std::ostream& out, const CrossFlip& t){
    out << "Size: (" << t.getSizeX() << ", " << t.getSizeY() <<")" << std::endl;
    out << "Blocked" << std::endl;
    for(int y=0; y<t.getSizeY(); ++y){
        for(int x=0; x<t.getSizeX(); ++x){
            out << t.isBlocked(x,y);
        }
        out << std::endl;
    }
    out << "Start" << std::endl;
    for(int y=0; y<t.getSizeY(); ++y){
        for(int x=0; x<t.getSizeX(); ++x){
            out << t.hasStartValue(x,y);
        }
        out << std::endl;
    }
    return out;
}
