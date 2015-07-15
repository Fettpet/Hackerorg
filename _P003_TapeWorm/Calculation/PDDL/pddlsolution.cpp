#include "pddlsolution.h"
#include <fstream>


/**************************************************************************************************
 *                                      Domain File
 * *'*********************************************************************************************/
/**
  create a Domain File for a World
 * @brief pddlSolution::createDomainFile
 * @param fileName
*/

void pddlSolution::createDomainFile(const std::string &fileName){
    std::fstream data;
    data.open(fileName, std::ios::out);

    data << "(define (domain tapeworm)" << std::endl;
    data << "   (:requirements :adl :typing)" << std::endl;
    data << std::endl;
/******
*      types
* ****/
    data << "   (:types Worm Wormpart Field Color)" << std::endl;
    data << std::endl;
/*****
 *  constants
 * ***/
    data << "   (:constants" << std::endl;
    data << "       GameField" << std::endl;
    data <<"    )" << std::endl;
/******
*      predicates
* ****/
    data << "   (:predicates" << std::endl;
    data << "       (adj ?field_1 ?field_2 - Field)" << std::endl;
    data << "       (isAtPosition ?wormP - Wormpart ?fieldW - Field)" << std::endl;
    data << "       (hasColorField ?fieldW - Field ?col - Color)" << std::endl;
    data << "       (hasColorWorm ?worm - Worm ?col - Color)" << std::endl;
    data << "       (before ?wormP1 - Wormpart ?wormP2 - Wormpart)" << std::endl;
    data << "       (isHead ?wormP - Wormpart ?w - Worm)" << std::endl;
    data << "       (isTail ?wormP - Wormpart ?w - Worm)" << std::endl;
    data << "       (isPartFromWorm ?wormP - Wormpart ?w - Worm)" << std::endl;
    data << "       (equalColor ?col1 - Color ?col2 - Color)" << std::endl;
    data << "       (layOneFieldWithSameColor ?wp - Wormpart)" << std::endl;
    data << "       (moveable ?w - Worm ?start - Field ?goal - Field)" << std::endl;
    data << "       (notUsed ?w - Worm)" << std::endl;
    data << "       (canMove ?g)" << std::endl;
    data << "       (canNotMove ?g)" << std::endl;
    data << "       (isFree ?f - Field)" << std::endl;
    data << "       (isBlocked ?f - Field)" << std::endl;
    data << "   )" << std::endl;
    data << std::endl;
/******
 *      Actions
 * ****/
    data << "   (:action moveHead" << std::endl;
/// parameter
    data << "       :parameters (" << std::endl;
    data << "                    ?worm - Worm" << std::endl;
    data << "                    ?startField - Field" << std::endl;
    data << "                    ?goalField - Field" << std::endl;
    data << "                    ?Head - WormPart" << std::endl;
    data << "                    ?Tail - WormPart" << std::endl;
    data << "       )" << std::endl;
/// precondition
    data << "       :precondition (and" << std::endl;
    data << "           (canMove GameField)" << std::endl;
    data << "           (adj ?startField ?goalField)" << std::endl;
    data << "           (moveable ?worm ?startField ?goalField)" << std::endl;
    data << "           (isHead ?Head ?worm)" << std::endl;
    data << "           (isTail ?Tail ?worm)" << std::endl;
    data << "           (isAtPosition ?Head ?startField)" << std::endl;
    data << "       )" << std::endl;
    data << "       :effect (and" << std::endl;
    data << "           (canNotMove GameField)" << std::endl;
    data << "           (not (canMove GameField))" << std::endl;
    data << "           (not (isHead ?Head ?worm))" << std::endl;
    data << "           (not (isTail ?Tail ?worm))" << std::endl;
    data << "           (isHead ?Tail ?worm)" << std::endl;
    data << "           (before ?Head ?Tail)" << std::endl;
    data << "           (isAtPosition ?Tail ?goalField)" << std::endl;
    data << "           (not (isFree ?goalField))" << std::endl;
    data << "           (isBlocked ?goalField)" << std::endl;
    data << "           (forall (?tailField - Field)" << std::endl;
    data << "               (when (and" << std::endl;
    data << "                   (isAtPosition ?Tail ?tailField)" << std::endl;
    data << "               )" << std::endl;
    data << "               (and" << std::endl;
    data << "                   (not (isBlocked ?tailField))" <<std::endl;
    data << "                   (isFree ?tailField)" << std::endl;
    data << "               ))" << std::endl;
    data << "           )" << std::endl;
/// SChwanz neu setzen
    data << "           (forall (?WPBeforeTail - Wormpart ?tailField - Field ?colorWorm - Color)" << std::endl;
    data << "               (when ( and" << std::endl;
    data << "                   (before ?Tail ?WPBeforeTail)" << std::endl;
    data << "                   (isAtPosition ?Tail ?tailField)" << std::endl;
    data << "                   (hasColorWorm ?worm ?colorWorm)" << std::endl;
    data << "               )" << std::endl;
    data << "               (and" << std::endl;
    data << "                   (isTail ?WPBeforeTail ?worm)" << std::endl;
    data << "                   (not (isAtPosition ?Tail ?tailField))" << std::endl;
    data << "                   (not (before ?Tail ?WPBeforeTail))" << std::endl;
    data << "               ))" << std::endl;
    data << "           )" << std::endl;
/// Sagen das die neue Position die selbe farbe hat
    data << "           (forall (?colorField - Color ?colorWorm - Color)" << std::endl;
    data << "               (when (and" << std::endl;
    data << "                   (hasColorField ?goalField ?colorField)" << std::endl;
    data << "                   (hasColorWorm ?worm ?colorWorm)" << std::endl;
    data << "                   (equalColor ?colorField ?colorWorm)" << std::endl;
    data << "               )" << std::endl;
    data << "               (and" << std::endl;
    data << "                   (layOneFieldWithSameColor ?Tail)" << std::endl;
    data << "               ))" << std::endl;
    data << "           )" << std::endl;
/// Sagen das die neue Position eine anedere farbe hat
    data << "           (forall (?colorField - Color ?colorWorm - Color)" << std::endl;
    data << "               (when (and" << std::endl;
    data << "                   (hasColorField ?goalField ?colorField)" << std::endl;
    data << "                   (hasColorWorm ?worm ?colorWorm)" << std::endl;
    data << "                   (not (equalColor ?colorField ?colorWorm))" << std::endl;
    data << "               )" << std::endl;
    data << "               (and" << std::endl;
    data << "                   (not (layOneFieldWithSameColor ?Tail))" << std::endl;
    data << "               ))" << std::endl;
    data << "           )" << std::endl;
/// Die alten moveables löschen
    data << "           (forall (?w - Worm ?gamefield - Field ?gameFieldTail - Field)" << std::endl;
    data << "               (when (and" << std::endl;
    data << "                   (moveable ?w ?gameFieldTail ?gamefield)" << std::endl;
    data << "               )" << std::endl;
    data << "               (and" << std::endl;
    data << "                   (not (moveable ?w ?gameFieldTail ?gamefield))" << std::endl;
    data << "               ))" << std::endl;
    data << "           )" << std::endl;
    data << "       )" << std::endl;
    data << "   )" << std::endl;
/********************
 *      Action createMoveableList
 * *******************/
    data << "   (:action setMovables" << std::endl;
    data << "       :parameters ()" << std::endl;
    data << "       :precondition (and (canNotMove GameField))" << std::endl;
    data << "       :effect ( and " << std::endl;
    data << "           (not (canNotMove GameField))" << std::endl;
    data << "           (canMove GameField)" << std::endl;
/// 1. Fall Kopf Leeres Feld
    data << "           (forall (?w - Worm ?wpHead - Wormpart ?gameFieldHead - Field ?GameFieldGoal - Field)" << std::endl;
    data << "               (when (and" << std::endl;
    data << "                   (adj ?gameFieldHead ?GameFieldGoal)" << std::endl;
    data << "                   (isHead ?wpHead ?w)" << std::endl;
    data << "                   (isAtPosition ?wpHead ?gameFieldHead)" << std::endl;
    data << "                   (isFree ?GameFieldGoal)" << std::endl;
    data << "               )( and" << std::endl;
    data << "                   (moveable ?w ?gameFieldHead ?GameFieldGoal)" << std::endl;
    data << "               ))" << std::endl;
    data << "           )" << std::endl;
/// 1. Fall Kopf Leeres Feld
    data << "           (forall (?w - Worm ?wpTail - Wormpart ?gameFieldTail - Field ?GameFieldGoal - Field)" << std::endl;
    data << "               (when (and" << std::endl;
    data << "                   (adj ?gameFieldTail ?GameFieldGoal)" << std::endl;
    data << "                   (isTail ?wpTail ?w)" << std::endl;
    data << "                   (isAtPosition ?wpTail ?gameFieldTail)" << std::endl;
    data << "                   (isFree ?GameFieldGoal)" << std::endl;
    data << "               ) (and" << std::endl;
    data << "                   (moveable ?w ?gameFieldTail ?GameFieldGoal)" << std::endl;
    data << "               ))" << std::endl;
    data << "           )" << std::endl;
/// 3. Fall Kopf auf Schwanz
    data << "           (forall (?w - Worm ?wpTail - Wormpart ?wpHead - Wormpart ?gameFieldHead - Field ?GameFieldTail - Field)" << std::endl;
    data << "               (when (and" << std::endl;
    data << "                   (adj ?gameFieldHead ?GameFieldTail)" << std::endl;
    data << "                   (isHead ?wpHead ?w)" << std::endl;
    data << "                   (isTail ?wpTail ?w)" << std::endl;
    data << "                   (isAtPosition ?wpHead ?gameFieldHead)" << std::endl;
    data << "                   (isAtPosition ?wpTail ?gameFieldTail)" << std::endl;
    data << "               ) (and" << std::endl;
    data << "                   (moveable ?w ?gameFieldHead ?gameFieldTail)" << std::endl;
    data << "                   (moveable ?w ?gameFieldTail ?gameFieldHead)" << std::endl;
    data << "               ))" << std::endl;
    data << "           )" << std::endl;


    data << "       )" << std::endl;
    data << "   )" << std::endl;
    data << ")";
    data.close();
}


/**************************************************************************************************
 *                          Problem file
 * ***********************************************************************************************/

void pddlSolution::createProblemFile(const std::string &fileName){
    std::fstream data;
    data.open(fileName, std::ios::out);
    data << "(define (problem TapeWormProblem)" << std::endl;
    data << "   (:domain tapeworm)" << std::endl;
/************
 *  Objects
 * *********/
    data << "   (:objects" << std::endl;
// Worms
    data << "       ";
    for(Worm t:w.getWorms()){
        data << "worm_" << t.getId() << " ";
    }
    data << "- Worm" << std::endl;

// WormParts
    for(Worm t:w.getWorms()){
        data << "       ";
        for(uint i=0; i<t.getLength(); ++i){
            data << "wormPart_" << t.getId() << "_" << i << " ";
        }
        data << "- WormPart" << std::endl;
    }
// Colors
    std::set<int> colors;

    for(Worm t:w.getWorms()){
        colors.insert(t.getColor());
    }
    data << "       ";
    for(int c: colors){
        data << "color_" << c << " ";
    }
    data << "- Color" << std::endl;
// Fields
    for(uint x=0; x<w.getSizeX(); ++x){
        data << "       ";
        for(uint y=0; y<w.getSizeY(); ++y){
            data << "Field_" << x << "_" <<y << " ";
        }
        data << "- Field" << std::endl;
    }
    data << "   )" << std::endl;
/***************
 *  init
 * ************/
    data << "   (:init" << std::endl;
    data << "       (canMove GameField)" << std::endl;
// Is Free bzw is blocked

    for(uint x=0; x<w.getSizeX(); ++x){
        for(uint y=0; y<w.getSizeY(); ++y){
            bool blocked(false);
            for(Worm wu:w.getWorms()){
                if(wu.contains(WormPart(x, y))) blocked = true;
            }
            if(blocked){
                data << "       (isBlocked Field_" << x << "_" << y << ")" << std::endl;
            } else {
                data << "       (isFree Field_" << x << "_" << y << ")" << std::endl;
            }
        }
    }
// is Part
    for(Worm t: w.getWorms()){
        data << "       ";
        for(uint i=0; i<t.getLength(); ++i){
            data << "(isPartFromWorm wormPart_" << t.getId() << "_" << i << " worm_" << t.getId() << ") ";
        }
        data << std::endl;
    }
// Adjazent
    for(uint x=0; x<w.getSizeX(); ++x){
        for(uint y=0; y<w.getSizeY(); ++y){
            if(w.getWormMap().isBlocked(x,y)) continue;
            NeighborContainer* neighbors;
            neighbors = w.getNeighborFields(x, y);
            data << "       ";
            for(const WormPart& t: *neighbors){
                if(w.getWormMap().isBlocked(t.getPosX(), t.getPosY())) continue;
                data << "(adj Field_" << x << "_" << y << " Field_" << (int)t.getPosX() << "_" << (int)t.getPosY() << ") ";
            }
            data << std::endl;
        }
    }
//  (isAtPosition ?wormP - Wormpart ?fieldW - Field)
    for(Worm t: w.getWorms()){
        data << "       ";
        for(uint i=0; i<t.getLength(); ++i){
            data << "(isAtPosition wormPart_" << t.getId() << "_" << i << " Field_" << (int)t.getPart(i).getPosX() << "_" << (int)t.getPart(i).getPosY() << ") ";
        }
        data << std::endl;
    }

// (hasColorField ?fieldW - Field ?col - Color)
    for(uint x=0; x<w.getSizeX(); ++x){
        for(uint y=0; y<w.getSizeY(); ++y){
            if(w.getWormMap().getTheHomeFor(x,y) > 1)
                data << "       (hasColorField Field_" << x << "_" << y << " color_" << w.getWormMap().getTheHomeFor(x,y) << ")" << std::endl;
        }
    }
// (hasColorWorm ?worm - Worm ?col - Color)
    for(Worm t: w.getWorms()){
        data << "       (hasColorWorm worm_" << t.getId() << " color_" << t.getColor() << ")" << std::endl;
    }
//(before ?wormP1 - Wormpart ?wormP2 - Wormpart)
    for(Worm t: w.getWorms()){
        data << "       ";
        for(uint i=0; i<t.getLength()-1; ++i){
            data << "(before wormPart_" << t.getId() << "_" << i+1 << " wormPart_" << t.getId() << "_"<< i << ") ";
        }
        data << std::endl;
    }
//(isHead ?wormP - Wormpart ?w - Worm)
     for(Worm t: w.getWorms()){
         data << "       (isHead wormPart_" << t.getId() << "_0 worm_" << t.getId() << ")" << std::endl;
     }
//(isTail ?wormP - Wormpart ?w - Worm)
     for(Worm t: w.getWorms()){
         data << "       (isTail wormPart_" << t.getId() << "_" << t.getLength() - 1 << " worm_" << t.getId() << ")" << std::endl;
     }
//(equalColor ?col1 - Color ?col2 - Color)
     for (int c: colors){
        data << "       (equalColor color_" << c << " color_" << c << ")" << std::endl;
     }
//layOneFieldWithSameColor
     for(Worm t: w.getWorms()){
         for(uint i=0; i<t.getLength(); ++i){
            if(w.getWormMap().getTheHomeFor(t.getPart(i).getPosX(), t.getPart(i).getPosY()) == t.getColor()){
                data << "       (layOneFieldWithSameColor wormPart_" << t.getId() << "_" << i << ")" <<std::endl;
            }
         }
     }
//(moveable ?w - Worm ?start - Field ?goal - Field)
    for(uint i=0; i<w.getWormCount(); ++i){
        NeighborContainer* neighbor;
        neighbor = w.getNeighborFields(w.getWorm(i).getHead().getPosX(), w.getWorm(i).getHead().getPosY());
        for(WormPart n: *neighbor){
            if(!w.isBlocked(n, i)){
                data << "       (moveable worm_" << w.getWorm(i).getId() << " Field_" << (int)w.getWorm(i).getHead().getPosX() << "_" << (int)w.getWorm(i).getHead().getPosY()
                     << " Field_" << (int)n.getPosX() << "_" << (int)n.getPosY() << ")" << std::endl;
            }

        }
    }
data << "   )" << std::endl;
/***********
 *  Goal
 * ********/
    data << "   (:goal (and" << std::endl;
    for(Worm t: w.getWorms()){
        for(uint i=0; i<t.getLength(); ++i){
            data << "       (layOneFieldWithSameColor wormPart_" << t.getId() << "_" << i << ")" <<std::endl;
        }
    }

    data << "   ))" << std::endl;
    data << ")" << std::endl;
}
