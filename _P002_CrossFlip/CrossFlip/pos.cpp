#include "pos.h"
#include <ostream>
#include <QString>
/**************************************************************************************************
                                Konstruktoren
************************************************************************************************///
/** Standart Konstruktor
  Setzt x und y auf 0
*/
Pos::Pos(){
    x = 0;
    y = 0;
}
/**
   Konstruktor über x, y
   @param x: Die X koordinate des Position
   @param y: Die Y Koordinate der Position
*/
Pos::Pos(int x, int y){
    this->x = x;
    this->y = y;
}
/** Der Kopierkonstruktor
  @param Pos old: Die Position, die Kopiert werden soll
*/
Pos::Pos(const Pos& old){
    this->x = old.x;
    this->y = old.y;
}
/**
  @param Die Position als String. Sollte folgenden Aufbau besitzen
  (x|y)
  */
Pos::Pos(const QString& t){
    this->x = t.mid(t.indexOf("(")+1, t.indexOf("|") - t.indexOf("(")-1).toInt();
    this->y = t.mid(t.indexOf("|")+1, t.indexOf(")") - t.indexOf("|")-1).toInt();
}

/**************************************************************************************************
                                Getter
************************************************************************************************///
/**
  @return int x: Gibt die X Koordinate der Position als Interger zurück
*/
int Pos::getX()const{
    return x;
}
/**
  @return int y: Gibt die Y Koordinate der Position als Integer zurück
*/
int Pos::getY()const{
    return y;
}
/**************************************************************************************************
                                Setter
************************************************************************************************///
/**
  @param x: Setzt den X wert auf die gegebene Koordinate
*/
void Pos::setX(const int x){
    this->x = x;
}
/**
   @param y: Setzt den Y Wert auf die gegebene Koordinate
*/
void Pos::setY(const int y){
    this->y = y;
}

/**************************************************************************************************
                                Distanz berechnung
************************************************************************************************///
/**
    @param x1: Die X Koordinate der ersten Position
    @param y1: Die Y Koordinate der ersten Position
    @param x2: Die X Koordinate der Zweiten Position
    @param y2: Die Y Koordinate der zweiten Position
    @return Die Distance Zwischen den 2 Koordinaten
*/
float Pos::calcDist(const int x1, const int y1, const int x2, const int y2){
    return std::sqrt( std::pow(x2-x1,2) + std::pow(y2-y1,2));
}
/**
    @param p1: Die Koordinaten der ersten Position
    @param p2: Die Koordinaten der zweiten Position
    @return Die Distance Zwischen den 2 Koordinaten
*/
float Pos::calcDist(const Pos& p1, const Pos& p2){
    return calcDist(p1.getX(), p1.getY(), p2.getX(), p2.getY());
}
/**
    @param p1: Die Koordinaten der Position
    @return Die Distance Zwischen den 2 Koordinaten
*/
float Pos::calcDist(const Pos& p1)const{
    return calcDist(p1.x, p1.y);
}
/**
    @param x: Die X Koordinate der Position
    @param y: Die Y Koordinate der Position
    @return Die Distance Zwischen den 2 Koordinaten
*/
float Pos::calcDist(const int x, const int y)const{
    return Pos::calcDist(x,y,this->x, this->y);
}
/**************************************************************************************************
                                Operatoren
************************************************************************************************///
/**
   @param old: Das Element was Kopiert werden soll
*/
Pos& Pos::operator=(const Pos& old){
    this->x = old.x;
    this->y = old.y;
    return *this;
}
/**
  testet ob die 2 Positionen Identisch sind.
  @param other: die Zweite Koordiante
  @return true: Falls die Positionen identisch sind
          false: Falls die Positionen unterschiedlich sind
*/
bool Pos::operator==(const Pos& other)const{
    return ((x == other.x) && (y == other.y));
}
/**
  Überladen der Standart ausgabe
*/

std::ostream& operator<<(std::ostream& out, const Pos& p){
    out << "(" << p.getX() << "|" << p.getY() << ")";
    return out;
}

bool Pos::operator<(const Pos& other)const{
    if(x>other.x) return false;
    if(x<other.x) return true;
    if(y>=other.y) return false;
    return true;
}

QString Pos::toString()const{
    QString result = "(%1|%2)";
    result = result.arg(x).arg(y);
    return result;
}
