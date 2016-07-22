#ifndef WORLD_H
#define WORLD_H
#include <QVector>
#include <QString>
#include <utility>
#include <QObject>
#include <set>
#include "fieldentry.h"
#include "move.h"
#include "downloadmanager.h"





class world
{
public:
    world();
    world(const world& t);
    // Gibt an ob die Lösung Richtig ist
    bool isValid();
    // Gibt den Lösungsstring zurück
    QString getSolution();

    void LoadFromInternet(QString user, QString pass);
    void SubmitSolution(QString user, QString pass);
    QString getTheWorldAsHTMLString();

    unsigned int getSizeX() const;
    unsigned int getSizeY() const;
    int getValue(int x, int y) const;
    FieldEntry getField(int x, int y) const;
    unsigned int getCountImpasses() const;
    QVector<std::pair<int, int> > getImpasseList();
    //
    move doAMove(direction dir);

    void OnMoveBack();
    void OnMoveBackControll();
    // selbsterklärrend
    void clear();

    bool setStart(int x, int y);
    void setValue(int x, int y, long unsigned value);
    bool isBlocked(int x, int y) const;

    bool isOneWay(int x, int y) const;
    bool isOneWayBegin(int x, int y) const;
    bool isImpasse(int x, int y) const;
    bool isUnpassable(int x, int y) const;

    // Backtracking algo
    bool backtracking();
    bool backtracking(const move& Dir);
    // überprüft ob man noch nach Links und rechts gehen kann
    bool checkRowCanGoOn(int x);
    bool checkColumnCanGoOn(int y);

    bool operator==(const world& t);
    world& operator=(const world& t);
private:
    /* Die Feldgröße x und y
    */
    unsigned int sizeX, sizeY;
    /* Die Aktuelle Position des Steines
    */
     int CurrentPosX, CurrentPosY;
    /* 0 bedeutet Frei, 1 geblockt und ab der 2 das dort der Coil langgekommen ist
       2 ist der Start und 3 Das Feld was dannach besucht wurde usw....
    */
    QVector< QVector<FieldEntry> > MortailWorld;

    /*Zählt wie viele Felder auf einer Spalte geblockt sind
    */
    QVector< long int> OnColumnBlock;
    void setOnColumnBlock();

    /*Zählt wie viele Felder in einer Zeile geblockt sind
    */
    QVector< long int> OnRowBlock;
    void setOnRowBlock();

    /* Eine Liste mit allen möglichen Sackgassen
      Wenn es mehr als 2 gibt, kann es nicht zu einer lösung kommen, außer wenn der Stein noch nicht gesetzt wurde.
      Dann ist einer Der 2 Sackgassen der Start und der andere das ende
    */
    QVector< std::pair<int, int> > ImpasseList;
    /* MoveListe
       eine Liste die Alle Bewegungen des Coil speichert.
    */
    QVector<move> moveList;
    /* Solution List
      Eine Liste die alle Bisher errechnetetn lösungen enthält
    */
    QVector< QVector<move> > solutionList;
    QString generateSolutionString(int nr);

    void setImpasseList();
    void CheckImpasseList();
    /* hier sind die einzelnen Moves
    */
    bool moveUp();
    bool moveDown();
    bool moveRight();
    bool moveLeft();

    /* geht einen Schritt in diese Richtung zurück
    */
    void moveUpBack(const move& Dir);
    void moveDownBack(const move& Dir);
    void moveRightBack(const move& Dir);
    void moveLeftBack(const move& Dir);
    /* geht einen schritt zurück und schaut links und rechts nach Sackgassen
      Diese werden entfernt
    */
    void moveUpBackControll(const move& Dir);
    void moveDownBackControll(const move& Dir);
    void moveRightBackControll(const move& Dir);
    void moveLeftBackControll(const move& Dir);

    void RemoveImpasseList(int x, int y);
    void InsertImpasseList(int x, int y);
    void InsertImpasseList(std::pair<int, int> val);



};

#endif // WORLD_H
