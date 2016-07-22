#ifndef FIELDENTRY_H
#define FIELDENTRY_H
#include <utility>
#include <QVector>
/* Klassenbeschreibung FieldEntry
   Diese Klasse repräsentiert einen Punkt auf dem Spielfeld
   Dieser Punkt kann, muss aber nicht Jumps enthalten.
   Jumps sind sprünge, wodurch man direkt auf ein anderes Feld springen kann.

*/
class FieldEntry
{
public:
    FieldEntry();
    FieldEntry(int v);
    int getValue()const;
    int getNumberJumpsTo()const;
    int getNumberJumpsFrom() const;
    std::pair<int, int> getJumpTo(int number) const;
    std::pair<int, int> getJumpFrom(int number) const;

    void setValue(const int value);
    void addJumpTo(const int x, const int y);
    void addJumpFrom(const int x, const int y);
    void removeJumpFrom(const int x, const int y);
    void removeJumpTo(const int x, const int y);

    bool containsJumpTo(const int x, const int y) const;
    bool containsJumpFrom(const int x, const int y) const;

    operator int() {return value;}
private:
    int value;
    QVector< std::pair<int, int> > JumpToList, JumpFromList;

};

#endif // FIELDENTRY_H
