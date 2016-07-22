#include "world.h"
#include <QMessageBox>
#include <QEventLoop>
#include <iostream>
#include <QObject>
#include <QTimer>
#include <QByteArray>
#include <sstream>
#include <fstream>
#include <boost/foreach.hpp>
#include <QFile>
#define ImpassCount 3
#define PROTOKOLL 0
#define DEBUG 0
#define UnpassableCount 4
#define ImpassListBreak 3


/**************************************************************************************************
                                Konstruktoren
**************************************************************************************************/
world::world(){
    CurrentPosX = -1;
    CurrentPosY = -1;
}

world::world(const world& t){
    sizeX = t.sizeX;
    sizeY = t.sizeY;
    CurrentPosX = t.CurrentPosX;
    CurrentPosY = t.CurrentPosY;
    MortailWorld = t.MortailWorld;
    OnColumnBlock = t.OnColumnBlock;
    OnRowBlock = t.OnRowBlock;
    ImpasseList = t.ImpasseList;
}


void world::LoadFromInternet(QString user, QString pass){
    MortailWorld.clear();
    OnColumnBlock.clear();
    OnRowBlock.clear();
    ImpasseList.clear();
     //http://www.hacker.org/coil/index.php?name=<username>&password=<password>&path=DRU&x=1&y=2

    std::stringstream downloadUrl;
    downloadUrl << "curl --data \"name=" << user.toStdString() << "&password=" << pass.toStdString() << "\" "
                << "--output /tmp/hacker.html "
                << "http://www.hacker.org/coil/index.php";
    std::system(downloadUrl.str().c_str());

    QFile data("/tmp/hacker.html");
    if (!data.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QByteArray Source = data.readAll();

    //FlashVars="x=3&y=3&board=X.......X"
    Source = Source.mid(Source.indexOf("FlashVars"));
    Source = Source.mid(Source.indexOf("=")+2);
    Source = Source.mid(0,Source.indexOf("\""));
    std::cout << Source.data() << std::endl;
    QString x = Source.mid(Source.indexOf("x")+2);
    x = x.mid(0,x.indexOf("&"));
    QString y = Source.mid(Source.indexOf("y")+2);
    y = y.mid(0,y.indexOf("&"));
    QString Level = Source.mid(Source.indexOf("board")+6);
    sizeX = x.toInt();
    sizeY = y.toInt();
    for(unsigned int i=0; i <sizeY; ++i){
        QVector<FieldEntry> newEntry;
        for(unsigned int j=0; j<sizeX; ++j){
            if(Level.at(sizeX*i +j) == 'X'){
                newEntry.push_back(1);
            } else{
                newEntry.push_back(0);
            }
        }
        MortailWorld.push_back(newEntry);
    }
    setOnColumnBlock();
    setOnRowBlock();
    setImpasseList();
}

 void world::SubmitSolution(QString user, QString pass){
     std::stringstream downloadUrl;
     downloadUrl << "curl --data \"name=" << user.toStdString() << "&password=" << pass.toStdString()
                 << "&" << getSolution().toStdString()
                 << "\" --output /tmp/hacker.html "
                 << "http://www.hacker.org/coil/index.php";
     std::cout << downloadUrl.str().c_str() << std::endl;
     //http://www.hacker.org/coil/index.php?x=1&y=0&path=RDLDR
    std::system(downloadUrl.str().c_str());
 }

QString world::getTheWorldAsHTMLString(){
    std::stringstream str;
    str << "<html><body>";
    str << "sizeX " << sizeX << " sizeY " << sizeY << "<br>";
    str << "sizeX " << MortailWorld.size() << " sizeY " << MortailWorld[0].size() << "<br>";
    for(unsigned int i=0; i< sizeY;++i){
        for(unsigned int j=0; j< sizeX; ++j){
           if(getValue(j,i) == 1){
               str <<"<font color=\"#ff0000\">" << getValue(j,i) %10 << "</font>";
           } else if(getValue(j,i) == 2){
                str <<"<font color=\"#0000ff\">" << getValue(j,i) %10<< "</font>";
           } else if(getValue(j,i) > 2){
                str <<"<font color=\"#00ff00\">" << getValue(j,i) %10<< "</font>";
           } else if(isOneWay(j,i)){
               if(isOneWayBegin(j,i)){
                    str <<"<font color=\"#00ffff\">" << getValue(j,i) %10<< "</font>";
               } else {
                   str <<"<font color=\"#000000\">" << getValue(j,i) %10<< "</font>";
               }
           }else {
               if(ImpasseList.contains( std::make_pair(j,i))){
                    str <<"<font color=\"#ff00ff\">" <<getValue(j,i) %10<< "</font>";
               } else {
                   str <<"<font color=\"#000000\">" <<getValue(j,i) %10<< "</font>";
               }
           }
        }
        str << "<br>";
    }
    str << "Anzahl besetzt in einer Zeile: " << OnRowBlock.size() << "<br>";
    for(int i=0; i<OnRowBlock.size(); ++i){
        str << OnRowBlock[i] << " ";
    }
    str << "<br> Anzahl besetzt in einer Spalte:" << OnColumnBlock.size() <<  "<br>";
    for(int i=0; i< OnColumnBlock.size(); ++i){
        str << OnColumnBlock[i] << " ";
    }
    str << "<br>";
    for(int i=0; i< ImpasseList.size(); ++i){
        str << "<br>x:" << ImpasseList[i].first << " y: " << ImpasseList[i].second << "<br>";
    }
    str << "</body></html>";
    return QString(str.str().c_str());
}

/**************************************************************************************************
                                        Operatoren
**************************************************************************************************/
bool world::operator==(const world& t){
    if(t.getSizeX() != sizeX) return false;
    if(t.getSizeY() != sizeY) return false;

    for(int x=0; x<t.getSizeX(); ++x){
        for(int y=0; y<t.getSizeY(); ++y){
            if(t.getValue(x,y) != getValue(x,y)) return false;
        }
    }
    return true;
}

world& world::operator=(const world& t){
    sizeX = t.sizeX;
    sizeY = t.sizeY;
    CurrentPosX = t.CurrentPosX;
    CurrentPosY = t.CurrentPosY;
    MortailWorld = t.MortailWorld;
    OnColumnBlock = t.OnColumnBlock;
    OnRowBlock = t.OnRowBlock;
    ImpasseList = t.ImpasseList;
}

/**************************************************************************************************
                                GETTER
**************************************************************************************************/
unsigned int world::getSizeX() const{
    return sizeX;
}
unsigned int world::getSizeY() const{
    return sizeY;
}
int world::getValue(int x, int y) const{
    if((x < 0) || (x>=sizeX)) return -1;
    if((y<0) || (y>=sizeY)) return -1;
    return MortailWorld[y][x].getValue();
}

FieldEntry world::getField(int x, int y) const{
    if((x < 0) || (x>=sizeX)) return -1;
    if((y<0) || (y>=sizeY)) return -1;
    return MortailWorld[y][x];
}

unsigned int world::getCountImpasses() const{
    return ImpasseList.size();
}

QVector<std::pair<int, int> > world::getImpasseList(){
    return ImpasseList;
}

// Gibt den Lösungsstring zurück
QString world::getSolution(){
    std::stringstream str;
    //x=1&y=0&path=RDLDR
    str << "x=" << moveList[0].startX <<"&y=" << moveList[0].startY << "&path=";
    foreach(move t, moveList){
        if(t.MoveDirection == Up){
            str << "U";
        } else if(t.MoveDirection == Down){
            str << "D";
        } else if(t.MoveDirection == Left){
            str << "L";
        } else if(t.MoveDirection == Right){
            str << "R";
        }
    }
    return QString(str.str().c_str());
}
/**************************************************************************************************
                                               Setter
**************************************************************************************************/

void world::RemoveImpasseList(int x, int y){
    for(int i=0; i<ImpasseList.size(); ++i){
        if((ImpasseList[i].first == x) && (ImpasseList[i].second == y)){
            ImpasseList.remove(i);
            break;
        }
    }
}

void world::InsertImpasseList(int x, int y){
        RemoveImpasseList(x,y);
        ImpasseList.push_back(std::make_pair(x,y));
}

void world::InsertImpasseList(std::pair<int, int> val){
    InsertImpasseList(val.first, val.second);
}

/*
  Setzt das Feld (x, y) auf den Wert Value
    @param
*/
void world::setValue(int x, int y, long unsigned value){
    if((x > sizeX) || (x  < 0)) {
        std::cerr << "x ist in setValue falsch" << x << std::endl;
        return;
    }
    if((y > sizeY) || (y  < 0)) {
        std::cerr << "y ist in setValue falsch" << y << std::endl;
        return;
    }
    MortailWorld[y][x] = value;
}
/*
  Setzt in jeder Spalte die Anzahl der geblockten Elemente, wird nur aufgerufen wenn LoadFromInternet aufgerufen wird
  da es sehr langsam ist
*/
void world::setOnColumnBlock(){
    OnColumnBlock.clear();
    for(int i=0; i<sizeX; ++i){
        unsigned int result=0;
        for(int j=0; j<sizeY; ++j){
            if(getValue(i,j) != 0)
                result++;
        }
        OnColumnBlock.push_back(result);
    }
}
/*
  Setzt in jeder Zeile die Anzahl der geblockten Elemente, wird nur aufgerufen wenn LoadFromInternet aufgerufen wird
  da es sehr langsam ist
*/
void world::setOnRowBlock(){
    OnRowBlock.clear();
    for(int i=0; i<sizeY; ++i){
        unsigned int result=0;
        for(int j=0; j<sizeX; ++j){
            if(getValue(j,i) != 0)
                result++;
        }
        OnRowBlock.push_back(result);
    }
}
/* Erstellt am Anfang eine Liste mit Sackgassen.
   Kein eintrag bedeutet, nichts, kein Informationsgewinn
   Ein Eintrag bedeutet, das Dieser Punkt entwerder der Start oder das Ende ist
   Zwei Einträge (das Optimum) bedeutet, das Einer davon der Start und der Andere das ende ist
*/
void world::setImpasseList(){
    ImpasseList.clear();
    for(int i=0; i<sizeX; ++i){
        for(int j=0; j<sizeY; ++j){
            if(isImpasse(i,j)) {
                InsertImpasseList(i,j);
            }
        }
    }
}
/* Setzt auf ein Feld das Token
   @param x die X Koordinate
   @param y die Y Koordinate
*/
bool world::setStart(int x, int y){

#if PROTOKOLL == 1
    std::fstream datei;
    datei.open("/home/fettpet/Protokoll.htm", std::ios::out);
    datei << "Start: P(" << x << ", " << y << ")<br>";
    datei.close();
#endif
    if(isBlocked(x,y)){
        return false;
    }
    CurrentPosX = x;
    CurrentPosY = y;
    setValue(x,y,2);
    OnColumnBlock[x]++;
    OnRowBlock[y]++;

// Die Sackgassenliste aktuallisieren
    setImpasseList();
    RemoveImpasseList(x,y);

// Startet mit backtracking
    return backtracking(doAMove(Nothing));

}
/* Setzt das Feld zurück auf den ausgangswert
*/
void world::clear(){
    CurrentPosX = -1;
    CurrentPosY = -1;
    for(int x=0; x<sizeX; ++x){
        for(int y=0; y<sizeY; ++y){
            if(getValue(x,y) >= 2)
                setValue(x,y, 0);
        }
    }

    setOnColumnBlock();
    setOnRowBlock();
    setImpasseList();
    moveList.clear();

}
/**************************************************************************************************
                                    Tester
**************************************************************************************************/
/* überprüft eine Feld ob eine Feld geblockt ist
   @param x die X Koordinate
   @param y die Y Koordinate
   @return
       true falls das Feld geblockt ist
       false falls das Feld nicht geblockt ist
*/
bool world::isBlocked(int x, int y) const{
    if((x <0) || (y<0)) return true;
    if((x >= sizeX) || (y >= sizeY)) return true;
    return (getValue(x,y) != 0);
}

/* Testet einen Weg, ob es ein Einbahnstraße ist,
   also ob es nur 2 Wege gibt weiter zu gehen
    @param x die X Koordinate
    @param y die Y Koordinate
    @return
        true, falls es nur 2 Wege gibt
        false, falls es geblockt ist oder mehr bzw weniger Wege existieren
*/
bool world::isOneWay(int x, int y) const{
    int sum =0;
    if(isBlocked(x,y)) return false;
    if(isBlocked(x+1,y)) sum++;
    if(isBlocked(x-1,y)) sum++;
    if(isBlocked(x,y+1)) sum++;
    if(isBlocked(x,y-1)) sum++;
    for(int i=0; i<getField(x,y).getNumberJumpsTo();++i){
        if(isBlocked(getField(x,y).getJumpTo(i).first, getField(x,y).getJumpTo(i).second)) sum++;
    }
    return (sum==2);
}

/* schaut nach ob es ein Einbahnweg ist, der an maximal einen weiteren Einbahnweg liegt
     ine Sackgasse ist
   @param x die X Koordinate
   @param y die Y Koordinate
*/
 bool world::isOneWayBegin(int x, int y) const{
     int sum =0;
     if(!isOneWay(x,y)) return false;
     if(isOneWay(x+1,y)) sum++;
     if(isOneWay(x-1,y)) sum++;
     if(isOneWay(x,y+1)) sum++;
     if(isOneWay(x,y-1)) sum++;
     if(sum <= 1){
         if((!isBlocked(x+1,y) && !isBlocked(x,y+1)) && (!isBlocked(x+1,y+1)))return false;
         if((!isBlocked(x+1,y) && !isBlocked(x,y-1)) && (!isBlocked(x+1,y-1)))return false;
         if((!isBlocked(x-1,y) && !isBlocked(x,y+1)) && (!isBlocked(x-1,y+1)))return false;
         if((!isBlocked(x-1,y) && !isBlocked(x,y-1)) && (!isBlocked(x-1,y-1)))return false;
         return true;
     }
     return false;
 }

/* überprüft eine Feld ob es einate
   @param y die Y Koordinate
   @return
       true falls es eine Sackgasse ist
       false falls es keine Sackgasse ist, oder geblockt
*/
bool world::isImpasse(int x, int y) const{
    int sum =0;
    if(isBlocked(x,y)) return false;
    if(!isBlocked(x+1,y)) sum++;
    if(!isBlocked(x-1,y)) sum++;
    if(!isBlocked(x,y+1)) sum++;
    if(!isBlocked(x,y-1)) sum++;
    for(int i=0; i<getField(x,y).getNumberJumpsTo();++i){
        if(isBlocked(getField(x,y).getJumpTo(i).first, getField(x,y).getJumpTo(i).second)) sum++;
    }
    return (sum == 1);
}
/* überprüft ein Feld ob es Unpassierbar ist, d.h. man kommt nicht drauf
   @param x die X Koordinate
   @param y die Y Koordinate
   @return
       true falls es eine Unpassierbar ist
       false falls es Passierbar oder geblockt ist
*/
bool world::isUnpassable(int x, int y) const{
    int sum =0;
    if(isBlocked(x,y)) return false;
    if(!isBlocked(x+1,y)) sum++;
    if(!isBlocked(x-1,y)) sum++;
    if(!isBlocked(x,y+1)) sum++;
    if(!isBlocked(x,y-1)) sum++;
    for(int i=0; i<getField(x,y).getNumberJumpsTo();++i){
        if(isBlocked(getField(x,y).getJumpTo(i).first, getField(x,y).getJumpTo(i).second)) sum++;
    }
    return (sum == 0);
}
/* Testet die Welt ob sie alle Felder abdeckt
    @return
        true, falls es der Fall ist,
        false, falls es nicht der Fall ist
*/
bool world::isValid(){
    for(int x=0; x<sizeX; ++x){
        for(int y=0; y<sizeY; ++y){
            if(getValue(x,y) == 0) return false;
        }
    }
    return true;
}
/* Haut alle Sackgassen aus der impasseList raus, die keine Sackgassen mehr sind

*/
void world::CheckImpasseList(){
    for(int i=0; i<ImpasseList.size();++i){
        if(!isImpasse(ImpasseList[i].first, ImpasseList[i].second)){
            ImpasseList.remove(i);
        }
    }
}
/* Schaut die gegebene Zeile an, ob sie über die komplette Länge geht,
  ist dies der Fall schaut sie alle an die Über der Zeile sind und alle die unter der Zeile sind
  sind in beiden Fällen welche noch nicht auf Maximum, kann das Puzzel nicht gelösst werden und
  die Funktion returnt false
  ansonsten true
    debuggt
*/
bool world::checkRowCanGoOn(int x){
    if((x < 0) || (x>=OnRowBlock.size())) return true;
    if(OnRowBlock[x] != sizeX) return true;
    bool checkUp=false, checkDown=false;
    for(int i=0; i<x; ++i){
        if(OnRowBlock[i] != sizeX) {
            checkUp = true;
            break;
        }
    }
    for(int i=x; i<OnRowBlock.size(); ++i){
        if(OnRowBlock[i] != sizeX) {
            checkDown = true;
            break;
        }
    }
    if(checkDown&checkUp) return false;
    return true;
}
/* Schaut die gegebene Spalte an, ob sie über die komplette Länge geht,
  ist dies der Fall schaut sie alle an die Über der Zeile sind und alle die unter der Zeile sind
  sind in beiden Fällen welche noch nicht auf Maximum, kann das Puzzel nicht gelösst werden und
  die Funktion returnt false
  ansonsten true
    debuggt
*/
bool world::checkColumnCanGoOn(int y){
    if((y < 0) || (y>=OnColumnBlock.size())) return true;

    if(OnColumnBlock[y] != sizeY) return true;
    bool checkUp=false, checkDown=false;
    for(int i=0; i<y; ++i){
        if(OnColumnBlock[i] != sizeY) {
            checkUp = true;
            break;
        }
    }
    for(int i=y; i<OnColumnBlock.size(); ++i){
        if(OnColumnBlock[i] != sizeY) {
            checkDown = true;
            break;
        }
    }
    if(checkDown&checkUp) return false;
    return true;
}

/**************************************************************************************************
                        Moves
**************************************************************************************************/

move world::doAMove(direction dir){
#if DEBUG == 1
    std::cout<< "vorher"<<std::endl;
    QMessageBox t;
    t.setText(getTheWorldAsHTMLString());
    t.exec();
#endif
#if PROTOKOLL == 1
    std::fstream datei;
    datei.open("/home/fettpet/Protokoll.htm",std::ios::out | std::ios::app);
    datei << getTheWorldAsHTMLString().toStdString() << "<br>" << std::endl;
    datei.close();
#endif
    move result;
    bool canGoOn = true;
    result.startX = CurrentPosX;
    result.startY = CurrentPosY;
    result.MoveDirection = Nothing;
    if(dir == Up){
        result.MoveDirection = Up;
        canGoOn = moveUp();
    } else if(dir == Down){
        result.MoveDirection = Down;
        canGoOn = moveDown();
    } else if(dir == Right){
        result.MoveDirection = Right;
        canGoOn = moveRight();
    } else if(dir == Left){
        result.MoveDirection = Left;
        canGoOn = moveLeft();
    }
    CheckImpasseList();
    result.endX = CurrentPosX;
    result.endY = CurrentPosY;
    moveList.push_back(result);
// Es gibt zu viele Sackgassen
    if((ImpasseList.size()>=3) || (canGoOn == false)){
        if(isUnpassable(CurrentPosX+1, CurrentPosY)){
            result.nextSteps.push_back(Right);
        }else
        if(isUnpassable(CurrentPosX-1, CurrentPosY)){
            result.nextSteps.push_back(Left);
        }else
        if(isUnpassable(CurrentPosX, CurrentPosY+1)){
            result.nextSteps.push_back(Down);
        }else
        if(isUnpassable(CurrentPosX, CurrentPosY-1)){
            result.nextSteps.push_back(Up);
        }else{
            return result;
        }
        // Falls es 2 Sackgassen sind
        // Muss er eine der Zwei entlanggehen
    }else if(ImpasseList.size() == 2){
        if(isImpasse(CurrentPosX+1, CurrentPosY)){
            result.nextSteps.push_back(Right);
        }
        if(isImpasse(CurrentPosX-1, CurrentPosY)){
            result.nextSteps.push_back(Left);
        }
        if(isImpasse(CurrentPosX, CurrentPosY+1)){
            result.nextSteps.push_back(Down);
        }
        if(isImpasse(CurrentPosX, CurrentPosY-1)){
            result.nextSteps.push_back(Up);
        }
        return result;
    } else {
        if(!isBlocked(CurrentPosX+1, CurrentPosY)){
            result.nextSteps.push_back(Right);
        }
        if(!isBlocked(CurrentPosX-1, CurrentPosY)){
            result.nextSteps.push_back(Left);
        }
        if(!isBlocked(CurrentPosX, CurrentPosY+1)){
            result.nextSteps.push_back(Down);
        }

        if(!isBlocked(CurrentPosX, CurrentPosY-1)){

            result.nextSteps.push_back(Up);
        }
    }


    return result;
}

/* Geht einen Schritt zurück
   hat keinen Parameter und auch sonst nichts^^
*/
void world::OnMoveBack(){

#if PROTOKOLL == 1
    std::fstream datei;
    datei.open("/home/fettpet/Protokoll.htm",std::ios::out | std::ios::app);
    datei << getTheWorldAsHTMLString().toStdString() << "<br>" << std::endl;
    datei.close();
#endif
#if DEBUG == 1
    std::cout<< "vorher"<<std::endl;
    QMessageBox t;
    t.setText(getTheWorldAsHTMLString());
    t.exec();
#endif
    if(moveList.isEmpty()) return;
    if(moveList.last().MoveDirection == Up){
        moveUpBackControll(moveList.last());
    } else if(moveList.last().MoveDirection == Down){
        moveDownBackControll(moveList.last());
    }else if(moveList.last().MoveDirection == Right){
        moveRightBackControll(moveList.last());
    }else if(moveList.last().MoveDirection == Left){
        moveLeftBackControll(moveList.last());
    }
    CurrentPosX = moveList.last().startX;
    CurrentPosY = moveList.last().startY;
    moveList.remove(moveList.size()-1);
    if(isImpasse(CurrentPosX+1, CurrentPosY)){
        InsertImpasseList( std::make_pair(CurrentPosX+1, CurrentPosY));
    }
    if(isImpasse(CurrentPosX-1, CurrentPosY)){
         InsertImpasseList( std::make_pair(CurrentPosX-1, CurrentPosY));
    }
    if(isImpasse(CurrentPosX, CurrentPosY+1)){
        InsertImpasseList( std::make_pair(CurrentPosX, CurrentPosY+1));
    }
    if(isImpasse(CurrentPosX, CurrentPosY-1)){
         InsertImpasseList( std::make_pair(CurrentPosX, CurrentPosY-1));
    }
    CheckImpasseList();
}


bool world::moveUp(){
    bool result = true;
    for(int i= CurrentPosY; i>=0; --i){
        if(isBlocked(CurrentPosX, i-1)){
            CurrentPosY = i;
            break;
        }
        setValue(CurrentPosX, i-1, getValue(CurrentPosX,i)+1);
        if(isImpasse(CurrentPosX -1, i-1)) {
            InsertImpasseList( std::make_pair(CurrentPosX-1, i-1));
        }
        if(isUnpassable(CurrentPosX-1, i-1)){
            CurrentPosY = i-1;
            result = false;
        }
        if(isImpasse(CurrentPosX+1, i-1)) {
            InsertImpasseList( std::make_pair(CurrentPosX+1, i-1));
        }
        if(isUnpassable(CurrentPosX+1, i-2)){
            CurrentPosY = i-2;
            result= false;
        }
    }
    return  result;
}
bool world::moveDown(){
    bool result = true;
    for(int i= CurrentPosY; i<sizeY; ++i){
        if(isBlocked(CurrentPosX, i+1)){
            CurrentPosY = i;
            break;
        }
        setValue(CurrentPosX, i+1, getValue(CurrentPosX,i)+1);
        if(isImpasse(CurrentPosX -1, i+1)) {
            InsertImpasseList( std::make_pair(CurrentPosX-1, i+1));
        }
        if(isUnpassable(CurrentPosX-1, i+2)){
            CurrentPosY = i+2;
            result = false;
        }
        if(isImpasse(CurrentPosX+1, i+1)) {
            InsertImpasseList( std::make_pair(CurrentPosX+1, i+1));
        }
        if(isUnpassable(CurrentPosX+1, i-2)){
            CurrentPosY = i-2;
            result = false;
        }
    }
    return result;
}
bool world::moveRight(){
    bool result = true;
    for(int i= CurrentPosX; i<sizeX; ++i){
        if(isBlocked(i+1, CurrentPosY)){
            CurrentPosX = i;
            break;
        }
        setValue(i+1, CurrentPosY, getValue(i,CurrentPosY)+1);
        if(isImpasse(i+1, CurrentPosY-1)) {
            InsertImpasseList( std::make_pair(i+1, CurrentPosY-1));
        }
        if(isUnpassable(i+2, CurrentPosY-1)){
            CurrentPosX = i+2;
            result =  false;
        }
        if(isImpasse(i+1, CurrentPosY+1)) {
            InsertImpasseList( std::make_pair(i+1, CurrentPosY+1));
        }
        if(isUnpassable(i+2, CurrentPosY+1)){
            CurrentPosX = i+2;
            result = false;
        }
    }
    return result;
}
bool world::moveLeft(){
    bool result = true;
    for(int i= CurrentPosX; i>=0; --i){
        if(isBlocked(i-1, CurrentPosY)){
            CurrentPosX = i;
            break;
        }
        setValue(i-1, CurrentPosY, getValue(i,CurrentPosY)+1);
        if(isImpasse(i-1, CurrentPosY-1)) {
            InsertImpasseList( std::make_pair(i-1, CurrentPosY-1));
        }
        if(isImpasse(i-1, CurrentPosY+1)) {
            InsertImpasseList( std::make_pair(i-1, CurrentPosY+1));

        }
        if(isUnpassable(i-2, CurrentPosY-1)){
            CurrentPosX = i-2;
            result = false;
        }
        if(isUnpassable(i-2, CurrentPosY+1)){
            CurrentPosX = i-2;
            result = false;
        }
    }

    return result;
}

/********************************************
                Moves Back
*******************************************/


void world::moveUpBackControll(const move&  Dir){
    for(int i=Dir.endY; i < Dir.startY; ++i){
        setValue(Dir.startX, i, 0);
    }
    CheckImpasseList();
}
void world::moveDownBackControll(const move&  Dir){
    for(int i=Dir.endY; i > Dir.startY; --i){
        setValue(Dir.startX, i, 0);
    }
    CheckImpasseList();
}
void world::moveRightBackControll(const move&  Dir){
    for(int i=Dir.endX; i > Dir.startX; --i){
        setValue(i, Dir.startY, 0);
    }
    CheckImpasseList();
}
void world::moveLeftBackControll(const move&  Dir){
    for(int i=Dir.endX; i < Dir.startX; ++i){
        setValue(i, Dir.startY, 0);
    }
    CheckImpasseList();
}

/**************************************************************************************************
                                Solver
**************************************************************************************************/

bool world::backtracking(const move& Dir){
    bool gemovt = false;
    for(int i=0; i< Dir.nextSteps.size(); ++i){
        if(Dir.nextSteps[i] == Down){
            gemovt = true;
            if(backtracking(doAMove(Down))) return true;
            OnMoveBack();
        }  else if(Dir.nextSteps[i] == Up) {
            gemovt = true;
            if(backtracking(doAMove(Up))) return true;
            OnMoveBack();
        } else if(Dir.nextSteps[i] == Left){
            gemovt = true;
            if(backtracking(doAMove(Left))) return true;
            OnMoveBack();
        } else if(Dir.nextSteps[i] == Right){
            gemovt = true;
            if(backtracking(doAMove(Right))) return true;
            OnMoveBack();
        }
    }

    if(gemovt == false){
        if(isValid()){
            return true;
        }
    }
    return false;
}

bool world::backtracking(){
    bool gemovt = false;
    if(!isBlocked(CurrentPosX+1, CurrentPosY)){
       doAMove(Right);
       if(backtracking()) return true;
       OnMoveBack();
    }
    if(!isBlocked(CurrentPosX-1, CurrentPosY)){
        doAMove(Left);
        if(backtracking()) return true;
        OnMoveBack();
    }
    if(!isBlocked(CurrentPosX, CurrentPosY+1)){
        doAMove(Down);
        if(backtracking()) return true;
        OnMoveBack();
    }

    if(!isBlocked(CurrentPosX, CurrentPosY-1)){
        doAMove(Up);
        if(backtracking()) return true;
        OnMoveBack();
    }
    if(gemovt == false){
        if(isValid()){
            return true;
        }
    }
    return false;
}
