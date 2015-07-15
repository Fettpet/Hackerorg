#include "robotsworld.h"
#include <QDebug>

/**********************************************************************************************************************
                                Konstruktoren
**********************************************************************************************************************/
/* Standart Konstruktor

  */
RobotsWorld::RobotsWorld(){
    sizeX = 0;
    sizeY = 0;
    countMinSteps = 0;
    countMaxSteps = 0;
}

RobotsWorld::RobotsWorld( const RobotsWorld& t){
    sizeX = t.sizeX;
    sizeY = t.sizeY;
    countMinSteps = t.countMinSteps;
    countMaxSteps = t.countMaxSteps;

    world = new bool*[sizeY];
    for(unsigned int i=0; i< sizeY; ++i){
        world[i] = new bool[sizeX];
    }

    for(int i=0; i< sizeX; ++i)
        for(int j=0; j < sizeY; ++j){
            world[i][j] = t.world[i][j];
        }
}

/**********************************************************************************************************************
                                          Getter
**********************************************************************************************************************/
/* getSizeX
    gibt Die Feldbreite zurück
    @return: Die Feldbreite (Wie groß x ist)
*/
unsigned int RobotsWorld::getSizeX()const{
    return sizeX;
}
/* getSizeY
    gibt Die Höhe des Feldes zurück
    @return: Die Feldhoehe (Wie groß y ist)
*/
unsigned int RobotsWorld::getSizeY()const{
    return sizeY;
}

/* getSizeY
      gibt die minimale anzahl an schritten zurück
    @return: minimale Pfadlänge
*/
unsigned int RobotsWorld::getMinSteps() const{
    return countMinSteps;
}

/* getMaxSteps
    gibt die maximale anzahl an schritten zurück
    @return: maximale Pfadlänge
*/
unsigned int RobotsWorld::getMaxSteps() const{
    return countMaxSteps;
}


/* getPointThereCanGoOut
    Berechnet Alle Punkte, bei dennen ein Erreichbarer ausgang ist
    AUCHTUNG DIESE PUNKTE LIEGEN NICHT IM FELD (sondern knapp drausen
    @return: einen QVector<point> der all diese Punkte enthält
*/
QVector<point> RobotsWorld::getPointThereCanGoOut()const{
    QVector<point> result;
    for(unsigned int x=0; x<sizeX; ++x){
        if(isPassable(x,sizeY-1)) {
            point add;
            add.x = x;
            add.y = sizeY;
            result.push_back(add);
        }
    }
    for(unsigned int y=0; y<sizeY; ++y){
        if(isPassable(sizeX-1, y)) {
            point add;
            add.x = sizeX;
            add.y = y;
            result.push_back(add);
        }
    }
    return result;
}

/* getEntry
    gibt den Eintrag in der Welt zurück
    @param x: Die Position x
    @param y: Die y Position
    @return: 0 es ist passierbar
             1 es ist blockiert
*/

bool RobotsWorld::getEntry(const int x, const int y)const{
    if( x<0) qDebug() << "x ist in getEntry zu klein";
    if( y<0) qDebug() << "y ist in getEntry zu klein";
    if (x>=sizeX) qDebug() << "x ist in getEntry zu groß";
    if (y>=sizeY) qDebug() << "y ist in getEntry zu groß";
    return world[x][y];
}

/* getEntry
    gibt zurück ob das Feld blockiert ist
    @param x: Die Position x
    @param y: Die y Position
    @return: false es ist passierbar
             true es ist blockiert
*/
bool RobotsWorld::isBlocked(const int x, const int y) const{
    if((x<0) || (y<0)) return true;
    if (x>=sizeX) qDebug() << "x ist in isBlocked zu groß";
    if (y>=sizeY) qDebug() << "y ist in isBlocked zu groß";
    return world[x][y];
}
/* getEntry
    gibt zurück ob das Feld passierbar ist
    @param x: Die Position x
    @param y: Die y Position
    @return: true es ist passierbar
             false es ist blockiert
*/
bool RobotsWorld::isPassable(const int x,const int y) const{
    if((x<0) || (y<0)) return false;
    if (x>=sizeX) qDebug() << "x ist in isPassable zu groß";
    if (y>=sizeY) qDebug() << "y ist in isPassable zu groß";
    return !world[x][y];
}
/* Gibt Das Feld als MessageBox aus

*/
void RobotsWorld::showMessageBox() const{
    // Feldinformationen
        std::stringstream str;
        str << "<html><body><br>" << std::endl;
        str << "SizeX: " << sizeX << std::endl << " <br>SizeY " << sizeY << "<br>";
        for(unsigned int i=0; i<sizeX;++i){
            for(unsigned int j=0; j<sizeY;++j){
                if(i + j == countMinSteps){
                    str << "<font color=\"aqua\">" <<getEntry(j,i) << "</font>";
                }else if(i+j == countMaxSteps){
                    str << "<font color=\"red\">" <<getEntry(j,i) << "</font>";
                } else{
                    str << getEntry(j,i);
                }
            }
            str << "<br>";
        }
        str << "</body></html>";
        QMessageBox t;
        t.setText(str.str().c_str());
        t.exec();
}
void RobotsWorld::showMessageBox(point tp) const{
    // Feldinformationen
        std::stringstream str;
        str << "<html><body><br>" << std::endl;
        str << "SizeX: " << sizeX << std::endl << " <br>SizeY " << sizeY << "<br>";
        for(unsigned int i=0; i<sizeX;++i){
            for(unsigned int j=0; j<sizeY;++j){
                if(( i % tp.x == 0 ) && ( j % tp.y == 0)){
                      str << "<font color=\"yellow\">" <<getEntry(j,i) << "</font>";
                      continue;
                }
                if(i + j == countMinSteps){
                    str << "<font color=\"aqua\">" <<getEntry(j,i) << "</font>";
                }else if(i+j == countMaxSteps){
                    str << "<font color=\"red\">" <<getEntry(j,i) << "</font>";
                } else{
                    str << getEntry(j,i);
                }
            }
            str << "<br>";
        }
        str << "</body></html>";
        QMessageBox t;
        t.setText(str.str().c_str());
        t.exec();
}
/*  getField
    @return einen String mit dem Feld drin
  */
QString RobotsWorld::getField() const{
    std::stringstream str;
    str << "<html><body><br>" << std::endl;
    str << "SizeX: " << sizeX << std::endl << " <br>SizeY " << sizeY << "<br>";
    for(unsigned int i=0; i<sizeX;++i){
        for(unsigned int j=0; j<sizeY;++j){
            if(i + j == countMinSteps){
                str << "<font color=\"aqua\">" <<getEntry(j,i) << "</font>";
            }else if(i+j == countMaxSteps){
                str << "<font color=\"red\">" <<getEntry(j,i) << "</font>";
            } else{
                if(getEntry(j,i)){
                    str << "<font color=\"#000000\">" << getEntry(j,i) << "</font>";
                }else {
                    str << "<font color=\"#ffffff\">" << getEntry(j,i) << "</font>";
                }
            }
        }
        str << "<br>";
    }
    str << "</body></html>";
    QString buffer;
    buffer = str.str().c_str();
    return buffer;
}

/*******************************************************************************************************************************
                                    SETTER
********************************************************************************************************************************/

/* setBlocked
    Setzt das Feld auf blockiert
    @param x: Die Position x
    @param y: Die y Position
*/
void RobotsWorld::setBlocked(const int x, const int y){
    if( x<0) qDebug() << "x ist in setBlocked zu klein";
    if( y<0) qDebug() << "y ist in setBlocked zu klein";
    if (x>=sizeX) qDebug() << "x ist in setBlocked zu groß";
    if (y>=sizeY) qDebug() << "y ist in setBlocked zu groß";
    world[x][y] = true;
}
/* setBlocked
    Setzt das Feld auf blockiert
    @param x: Die Position x
    @param y: Die y Position
*/
void RobotsWorld::setPassable(const int x, const int y){
    if( x<0) qDebug() << "x ist in setPassable zu klein";
    if( y<0) qDebug() << "y ist in setPassable zu klein";
    if (x>=sizeX) qDebug() << "x ist in setPassable zu groß";
    if (y>=sizeY) qDebug() << "y ist in setPassable zu groß";
    world[x][y] = false;
}
/* setUnreachableToBlocked
    setzt alle Felder die nicht erreichbar sind auf Blockiert
*/
void RobotsWorld::setUnreachableToBlocked(){
    for(int y=1; y<sizeX; ++y){
        if(isBlocked(0,y-1)){
            setBlocked(0,y);
        }
    }
    for(int x=1; x<sizeX; ++x){
        if(isBlocked(x-1,0)){
            setBlocked(x,0);
        }
    }
    for(int y=1; y<sizeY; ++y){
        for(int x=1; x<sizeX; ++x){
            if(isBlocked(x-1,y) && isBlocked(x,y-1)){
                setBlocked(x,y);
            }
        }
    }
}
/* setImpasseToBlock
  setzt alle Sackgassen auf Blockiert

  */
void RobotsWorld::setImpasseToBlock(){
    for(int y=sizeY-2; y>=0; --y){
        for(int x=sizeX-2; x>=0; --x){
            if(isBlocked(x+1,y) && isBlocked(x,y+1)){
                setBlocked(x,y);
            }
        }
    }
}
/* setEntry
    @param x die x koordinate
    @param y die y koordinate
    @param entry der wert
  */
void RobotsWorld::setEntry(const int x, const int y, bool entry){
    if( x<0) qDebug() << "x ist in getEntry zu klein";
    if( y<0) qDebug() << "y ist in getEntry zu klein";
    if (x>=sizeX) qDebug() << "x ist in getEntry zu groß";
    if (y>=sizeY) qDebug() << "y ist in getEntry zu groß";
    world[x][y] = entry;
}

/*************************************************************************************************************************************
                                    Test und submit
*************************************************************************************************************************************/
/* CreateTest
    Erstellt ein 4x4 großes Feld
    Das ist wie folgt aufgebaut
    0111
    0100
    0011
    1011
*/
void RobotsWorld::CreateTest(){
    for(unsigned int i=0; i<sizeX;++i){
        delete world[i];
    }

    sizeX = 4;
 // SizeY setzen
    sizeY = 4;
// CountMIn setzen
    countMinSteps = 2;
 //Count Max
    countMaxSteps = 2;

// Welt ein neues ausmass geben
    world = new bool*[sizeY];
    for(unsigned int i=0; i< sizeY; ++i){
        world[i] = new bool[sizeX];
    }

    world[0][0] = false;
    world[0][1] = false;
    world[0][2] = false;
    world[0][3] = false;

    world[1][0] = false;
    world[1][1] = false;
    world[1][2] = false;
    world[1][3] = false;

    world[2][0] = false;
    world[2][1] = false;
    world[2][2] = true;
    world[2][3] = false;

    world[3][0] = false;
    world[3][1] = false;
    world[3][2] = false;
    world[3][3] = false;
}

void RobotsWorld::ReadFromData(){
    QString Source;
    std::fstream datei;
    datei.open("/home/fettpet/Downloads/Runaway Robot.htm", std::ios::in);
    if(datei.is_open()) {
        QMessageBox t;
        t.setText("hu");
        t.exec();
    }
    std::string stringbuffer;
    for(int i=0; i<10000; ++i){
        datei >> stringbuffer;
        Source.append(stringbuffer.c_str());
    }
// SizeX setzen
    QString buffer;
    buffer = Source.mid(Source.indexOf("FVboardX=")+9);
    buffer = buffer.mid(0,buffer.indexOf("&"));
    sizeX = buffer.toInt();
    std::cerr << "Größe von sizeX:" << buffer.size();
 // SizeY setzen
    buffer = Source.mid(Source.indexOf("FVboardY=")+9);
    buffer = buffer.mid(0,buffer.indexOf("&"));
    sizeY = buffer.toInt();
     std::cerr << "Größe von sizeY:" << buffer.size();
// CountMIn setzen
    buffer = Source.mid(Source.indexOf("FVinsMin=")+9);
    buffer = buffer.mid(0,buffer.indexOf("&"));
    countMinSteps = buffer.toInt();
     std::cerr << "Größe von minsteps:" << buffer.size();
 //Count Max
    buffer = Source.mid(Source.indexOf("FVinsMax=")+9);
    buffer = buffer.mid(0,buffer.indexOf("&"));
    countMaxSteps = buffer.toInt();
     std::cerr << "Größe von MaxSteps:" << buffer.size();

// Welt ein neues ausmass geben
    world = new bool*[sizeY];
    for(unsigned int i=0; i< sizeY; ++i){
        world[i] = new bool[sizeX];
    }



// Feldinformationen

    buffer = Source.mid(Source.indexOf("FVterrainString=")+16);
    buffer = buffer.mid(0,buffer.indexOf("&"));
 std::cerr << "Größe vom Feld:" << buffer.size() << std::endl;

    for(unsigned int x=0; x<sizeX;++x){
        for(unsigned int y=0; y<sizeY;++y){

            if(buffer.at(sizeX*x+y) == '.'){
                setPassable(y,x);
            }else {
                setBlocked(y,x);
            }
        }
    }
}

/* Ließt die Weltbeschreibung aus dem Internet aus
  setzt die variablen wie es gegeben ist
  @param user Der Username
  @param pass Das Passwort des Users
*/

void RobotsWorld::ReadFromInternet(const QString& user,const QString& pass){
    DownloadManager Inet;
    QString Url;
    Url = "http://www.hacker.org/runaway/index.php?name=" + user + "&password=" + pass + "&";
    Inet.doDownload(Url);
    QEventLoop Loop;
    QObject::connect(&Inet, SIGNAL(finished()), &Loop, SLOT(quit()));
    Loop.exec();
    QString Source = Inet.getSource().value(0);
    for(unsigned int i=0; i<sizeX;++i){
        delete world[i];
    }


    Inet.SaveSourceToData("/home/fettpet/test.txt");
// SizeX setzen
    QString buffer;
    buffer = Source.mid(Source.indexOf("FVboardX=")+9);
    buffer = buffer.mid(0,buffer.indexOf("&"));
    sizeX = buffer.toInt();
    std::cerr << "Größe von sizeX:" << buffer.size();
 // SizeY setzen
    buffer = Source.mid(Source.indexOf("FVboardY=")+9);
    buffer = buffer.mid(0,buffer.indexOf("&"));
    sizeY = buffer.toInt();
     std::cerr << "Größe von sizeY:" << buffer.size();
// CountMIn setzen
    buffer = Source.mid(Source.indexOf("FVinsMin=")+9);
    buffer = buffer.mid(0,buffer.indexOf("&"));
    countMinSteps = buffer.toInt();
     std::cerr << "Größe von minsteps:" << buffer.size();
 //Count Max
    buffer = Source.mid(Source.indexOf("FVinsMax=")+9);
    buffer = buffer.mid(0,buffer.indexOf("&"));
    countMaxSteps = buffer.toInt();
     std::cerr << "Größe von MaxSteps:" << buffer.size();

// Welt ein neues ausmass geben
    world = new bool*[sizeY];
    for(unsigned int i=0; i< sizeY; ++i){
        world[i] = new bool[sizeX];
    }



// Feldinformationen

    buffer = Source.mid(Source.indexOf("FVterrainString=")+16);
    buffer = buffer.mid(0,buffer.indexOf("&"));
 std::cerr << "Größe vom Feld:" << buffer.size() << std::endl;

    for(unsigned int x=0; x<sizeX;++x){
        for(unsigned int y=0; y<sizeY;++y){

            if(buffer.at(sizeX*x+y) == '.'){
                setPassable(y,x);
            }else {
                setBlocked(y,x);
            }
        }
    }
}

/* Ließt die Weltbeschreibung aus dem Internet aus
   setzt die variablen wie es gegeben ist
   @param solution Die Lösung
   @param user Der Username
   @param pass Das Passwort des Users
*/

void RobotsWorld::submitSolution(const QString& solution, const QString& user, const QString& pass) const{
    DownloadManager Inet;
    QString URL ="http://www.hacker.org/runaway/index.php?name=" + user + "&password=" + pass;
    URL += "&path=" + solution;
    Inet.doDownload(URL);
    QEventLoop Loop;
    QObject::connect(&Inet, SIGNAL(finished()), &Loop, SLOT(quit()));
    Loop.exec();
}

/* Testet pb der gegebene Pfad aus dem Labirinth rausführt
   @param path der Pfad
   @return true der Pfad ist valide
            false er Pfad ist nicht richtig
*/

bool RobotsWorld::testPath(const QString &path)const{

    if(path.size() > countMaxSteps) {
        std::cerr << "Pfad ist zu lang: " << path.toStdString().c_str();
        return false;
    }
    if(path.size() < countMinSteps){
        std::cerr << "Pfad ist zu kurz: " << path.toStdString().c_str();
        return false;
    }


    int robotX = 0, robotY = 0, currentStepCount =0;
    while(true){
        if(path.at(currentStepCount % path.size()) == 'R'){
            robotX++;
            if(robotX >= sizeX){
                return true;
            }
        }else if((path.at(currentStepCount % path.size()) == 'D')){
            robotY++;

            if(robotY >= sizeY){
                return true;
            }
        } else{
            return false;
        }
        if(isBlocked(robotX,robotY)){
            return false;
        }
        currentStepCount++;
    }
}

QString dec2bin(unsigned long int n){
    const int size=sizeof(n)*8;
    QString res;
    bool s=0;
    for (int a=0;a<size;a++){
        bool bit=n>>(size-1);
        if (bit)
            s=1;
        if (s)
            res.push_back(bit+'0');
        n<<=1;
    }
    if (!res.size())
        res.push_back('0');
    return res;
}

/* Implementiert einen Primitiven Bruteforce algo
   @return einen Validen Pfad als QString
*/

QString RobotsWorld::calculateBruteForce(){
    //QStringList noWays = findWaysNotToGo();
    long unsigned int startway = std::pow(2,countMinSteps);
    long unsigned int endway = std::pow(2,countMaxSteps+1)-1;
    std::cout << "Start:" << startway << "ende:" << endway << std::endl;
    for(long unsigned int start = startway; start < endway; ++start){
        QString path = dec2bin(start);
        path.replace("0","R");
        path.replace("1", "D");

        if(testPath(path)){
            return path;
        }
    }
    for(long unsigned int start = startway; start < endway; ++start){
        QString path = dec2bin(start);
        path.replace("0","D");
        path.replace("1", "R");

        if(testPath(path)){
            return path;
        }
    }
    return "Error";
}

/* Schreibt die Aktuelle Welt in eine Datei
    @param fileName Erwartet den dateiname
*/

void RobotsWorld::WriteToData(const QString& fileName) const{
    std::fstream datei;
    datei.open(fileName.toStdString().c_str(), std::ios::out);
    datei << "sizeX=" <<sizeX << std::endl;
    datei << "sizeY=" << sizeY << std::endl;
    datei << "CountMinSteps=" << countMinSteps << std::endl;
    datei << "CountMaxSteps=" << countMaxSteps << std::endl;
    for(int j=0;j<sizeX;++j){
        for(int i=0;i<sizeY; ++i){
            datei << getEntry(i,j);
        }
        datei << std::endl;
    }
    datei.close();
}
/* ließt die Welt aus einer Dati aus
    @param fileName Erwartet den dateiname
*/

void RobotsWorld::ReadFromData(const QString& fileName){
    std::fstream datei;
    std::string buffer;
    QString line;
    datei.open(fileName.toStdString().c_str(), std::ios::in);

// SizeX
    datei >> buffer;
    line = buffer.c_str();
    line= line.mid(line.indexOf("=")+1);
    sizeX = line.toInt();

// SizeY
    datei >> buffer;
    line = buffer.c_str();
    line= line.mid(line.indexOf("=")+1);
    sizeY = line.toInt();

// MinSteps
    datei >> buffer;
    line = buffer.c_str();
    line= line.mid(line.indexOf("=")+1);
    countMinSteps = line.toInt();

// MaxSteps
    datei >> buffer;
    line = buffer.c_str();
    line= line.mid(line.indexOf("=")+1);
    countMaxSteps = line.toInt();

// Welt ein neues ausmass geben
    world = new bool*[sizeY];
    for(unsigned int i=0; i< sizeY; ++i){
        world[i] = new bool[sizeX];
    }
// Damit ich alten Quelltext mitbenutzen kann
    line = "";
    for(int i=0;i<sizeY;++i){
        datei >>buffer;
        line.append(buffer.c_str());
    }

    for(unsigned int x=0; x<sizeX;++x){
        for(unsigned int y=0; y<sizeY;++y){
            if(line.at(sizeX*x+y) == '0'){
                setPassable(y,x);
            }else {
                setBlocked(y,x);
            }
        }
    }

    datei.close();
}
/*
  liefert alle Punkte die in numberSteps blockieren


  */
QVector<point> RobotsWorld::getPointThereBlockAtTheBegining(const unsigned int numberSteps)const{
    QVector<point> result;
    for(int x=1; x<=numberSteps; ++x){
        for(int y=1; y<=numberSteps-x;++y){
            if(isBlocked(x,y)){
                point add;
                add.x = x;
                add.y = y;
                result.push_back(add);
            }
        }
    }
    return result;
}

/********************************************************************************************************
                        operator
**********************************************************************************************///////////
RobotsWorld& RobotsWorld::operator= (RobotsWorld const& rhs){
    sizeX = rhs.sizeX;
    sizeY = rhs.sizeY;
    countMinSteps = rhs.countMinSteps;
    countMaxSteps = rhs.countMaxSteps;
    world = new bool*[sizeY];
    for(unsigned int i=0; i< sizeY; ++i){
        world[i] = new bool[sizeX];
    }

    for(int i=0; i< sizeX; ++i)
        for(int j=0; j < sizeY; ++j){
            world[i][j] = rhs.world[i][j];
        }
}
