#include "fieldentry.h"
#include <iostream>
/**************************************************************************************************
                            Konstruktoren
**************************************************************************************************/


FieldEntry::FieldEntry(){
    value = -1;
}
FieldEntry::FieldEntry(int v){
      value = v;
  }

/**************************************************************************************************
                                                    Getter
**************************************************************************************************/
/** gibt den Eintrag des Feldes zurück
  0 bedeutet ein Leeres Feld
  1 ist ein Feld wo ein Stein drauf ist
  >=2 ist ein Feld wo teil des coils drauf ist, war
  @return den Wert des Feldes
*/
int FieldEntry::getValue()const{
    return value;
}

/**
  gibt die Anzahl der JumpsTo an
  @return die anzahl der JumsTo
*/
int FieldEntry::getNumberJumpsTo()const{
    return JumpToList.size();
}
/**
  gibt die Anzahl der JumpsFrom an
  @return die anzahl der JumpsFrom
*/
int FieldEntry::getNumberJumpsFrom() const{
    return JumpFromList.size();
}
/**
  gibt einen Eintrag aus der JumpToListe zurück
  @param number die Nummer des Eintrages
  @return der Eintrag der an Position number steht
*/
std::pair<int, int> FieldEntry::getJumpTo(int number) const{
    if(number < JumpToList.size()){
        return JumpToList[number];
    }
    std::cerr << "number(" << number << ") in getJumpTo ist zu groß. Darf maximal " << JumpToList.size()-1 << " sein" << std::endl;
}
/**
  gibt einen Eintrag aus der JumpFrom  Liste zurück
  @param number die Position des Eintrages
  @return der Punkt, von den der Sprung kommt
*/
std::pair<int, int>  FieldEntry::getJumpFrom(int number) const{
    if(number < JumpToList.size()){
        return JumpToList[number];
    }
    std::cerr << "number(" << number << ") in getJumpFrom ist zu groß. Darf maximal " << JumpToList.size()-1 << " sein" << std::endl;
}

/**************************************************************************************************
                                                    Setter
**************************************************************************************************/

/** Setzt den Wert des Feldes auf value
  @param value Den Wert, den das Feld annehmen soll

*/
void FieldEntry::setValue(const int value){
    this->value = value;
}

/** Legt einen Neuen eintrag mit einem Jumpto an
  @param x die X Koordinate
  @param y die Y Koordinate
*/
void FieldEntry::addJumpTo(const int x, const int y){
    JumpToList.push_back(std::make_pair(x,y));
}
/** Legt einen Neuen eintrag mit einem JumpFrom an
  @param x die X Koordinate
  @param y die Y Koordinate
*/
void FieldEntry::addJumpFrom(const int x, const int y){
    JumpFromList.push_back(std::make_pair(x,y));
}

/** entfernt einen Eintrag aus der JumpFromList
   @param x Die X koordinate
   @param y Die Y koordinate
*/
void FieldEntry::removeJumpFrom(const int x, const int y){
    for(int i=0; i<JumpFromList.size(); ++i){
        if((JumpFromList[i].first ==x) && (JumpFromList[i].second == y)){
            JumpFromList.remove(i);
            i--;
        }
    }
}

/** entfernt einen Eintrag aus der JumpTo
   @param x Die X koordinate
   @param y Die Y koordinate
*/
void FieldEntry::removeJumpTo(const int x, const int y){
    for(int i=0; i<JumpToList.size(); ++i){
        if((JumpToList[i].first ==x) && (JumpToList[i].second == y)){
            JumpToList.remove(i);
            i--;
        }
    }
}



/**************************************************************************************************
                                                Tester
**************************************************************************************************/
/** Schaut nach ob einen Punkt in der Jumpto liste enthalten ist
   @param x Die X koordinate
   @param y Die Y koordinate
   @return
        true, der Punkt ist enthalten
        false, der Punkt ist nicht enthalten
*/
bool FieldEntry::containsJumpTo(const int x, const int y) const{
    for(int i=0; i<JumpToList.size(); ++i){
        if((JumpToList[i].first ==x) && (JumpToList[i].second == y)){
            return true;
        }
    }
    return false;
}
/** Schaut nach ob einen Punkt in der JumpFrom liste enthalten ist
   @param x Die X koordinate
   @param y Die Y koordinate
   @return
        true, der Punkt ist enthalten
        false, der Punkt ist nicht enthalten
*/
bool FieldEntry::containsJumpFrom(const int x, const int y) const{
    for(int i=0; i<JumpFromList.size(); ++i){
        if((JumpFromList[i].first ==x) && (JumpFromList[i].second == y)){
            return true;
        }
    }
    return false;
}
