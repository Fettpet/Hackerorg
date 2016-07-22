#include "regiondfs.h"
#include <iostream>
/**************************************************************************************************
 *                                  Constructor
 * ***********************************************************************************************/
Solver::regionDFS::regionDFS(const World::Region& reg):
    currentRow(0), currentCol(0),
    region(reg)
{
    numberDeadEnds = reg.getNumberDeadEnd();
    numberUnblockedFields = reg.getNumberUnblocked();
    numberUnblockedPassages = reg.getNumberPassages();

}

Solver::regionDFS::regionDFS(const regionDFS& other):
    currentRow(other.currentRow), currentCol(other.currentCol),
    curMove(other.curMove),
    currentConditions(other.currentConditions),
    movelist(other.movelist),
    numberDeadEnds(other.numberDeadEnds),
    numberUnblockedFields(other.numberUnblockedFields),
    region(other.region),
    numberUnblockedPassages(other.numberUnblockedPassages)
{

}

/**************************************************************************************************
 *                                   Getter
 * ***********************************************************************************************/
inline uint Solver::regionDFS::getCurrentRow() const{
    return currentRow;
}

inline uint Solver::regionDFS::getCurrentCol() const{
    return currentCol;
}

/**************************************************************************************************
 *                                  Solving
 * ***********************************************************************************************/
Solver::Data::SavedMoveSequence
Solver::regionDFS::startSolving(const Solver::Data::Pos< uint> pos)
{
    return startSolving(pos.getCol(), pos.getRow());
}

Solver::Data::SavedMoveSequence
Solver::regionDFS::startSolving
(const uint col, const uint row)
{
    movelist.clear();
    movelist.push_back(Solver::Data::Move(col, row, Solver::Data::Up));
    curMove = movelist.back();

    while(!movelist.empty())
    {


        curMove = movelist.back();
        movelist.pop_back();
        if(doMove())
        {
            movelist.push_back(curMove);
            movelist.push_back(Solver::Data::Move(curMove.destination.getCol(), curMove.destination.getRow(), Solver::Data::Up));
            // 1st Case all blocked
            // 2nd Case Passage and at least one passage unblocked
            if(region.getNumberUnblocked() == 0 ||
              (region.isPassage(curMove.destination) && numberUnblockedPassages != 0)
              )
            {

                Solver::Data::SavedMoveSequence result = Solver::Data::SavedMoveSequence(row, col, true);
                movelist.pop_back();
                result.Moves = movelist;
                result.destionation = curMove.destination;
                return result;
            }
        } else
        {
            movelist.push_back(curMove);
            bool found = false;
            while(!movelist.empty() && !found)
            {
                curMove = movelist.back();
                backTrack();
                movelist.pop_back();
                curMove.destination = curMove.start;
                switch(curMove.direction)
                {
                case Solver::Data::Up:
                    found = true;
                    curMove.direction = Solver::Data::Right;
                    movelist.push_back(curMove);
                    break;
                case Solver::Data::Right:
                    found = true;
                    curMove.direction = Solver::Data::Down;
                    movelist.push_back(curMove);
                    break;
                case Solver::Data::Down:
                    found = true;
                    curMove.direction = Solver::Data::Left;
                    movelist.push_back(curMove);
                    break;
                case Solver::Data::Left:
                    break;
                }
            }
        }
    }

    return Solver::Data::SavedMoveSequence(row, col, false);
}

/**************************************************************************************************
 *                      Moves
 * ***********************************************************************************************/


bool Solver::regionDFS::doMoveDown()
{
//    //std::cout << "Do Move Down" << std::endl;
    bool result=false;
    uint col(curMove.start.getCol()), row(curMove.start.getRow());
    while(!region.isBlocked(col, row+1))
    {
        result = true;
        region.setBlocked(col, row+1);
        ++row;
    }
    if(result == true){
        if(region.isPassage(col, row))
        {
            numberUnblockedPassages--;
        }
        curMove.destination = Position(col, row);
    }
    return result;
}

bool Solver::regionDFS::doMoveUp()
{
    //std::cout << "Do Move Up" << std::endl;
    bool result=false;
    uint col(curMove.start.getCol()), row(curMove.start.getRow());

    while(!region.isBlocked(col, row-1))
    {
        result = true;
        region.setBlocked(col, row-1);
        --row;
    }

    if(result == true){
        if(region.isPassage(col, row))
        {
            numberUnblockedPassages--;
        }
        curMove.destination = Position(col, row);
    }
    return result;
}

bool Solver::regionDFS::doMoveLeft()
{
    //std::cout << "Do Move Left" << std::endl;
    bool result = false;
    uint col(curMove.start.getCol()), row(curMove.start.getRow());
    while(!region.isBlocked(col-1, row))
    {
        result = true;
        region.setBlocked(col-1, row);
        --col;
    }
    if(result == true){
        if(region.isPassage(col, row))
        {
            numberUnblockedPassages--;
        }
        curMove.destination = Position(col, row);
    }
    return result;
}

bool Solver::regionDFS::doMoveRight()
{
    //std::cout << "Do Move Right" << std::endl;
    bool result = false;
    uint col(curMove.start.getCol()), row(curMove.start.getRow());
    while(!region.isBlocked(col+1, row)){
        region.setBlocked(col+1, row);
        result = true;
        ++col;
    }
    if(result == true){
        if(region.isPassage(col, row))
        {
            numberUnblockedPassages--;
        }
        curMove.destination = Position(col, row);
    }
    return result;
}

/**
    testet nicht ob die aktuelle Position blockiert ist.
    Bewegt den coil in eine Richtung, welche in curMove spezifiziert ist.
 * @brief Solver::regionDFS::doMove
*/
bool Solver::regionDFS::doMove()
{

    uint row(curMove.start.getRow()), col(curMove.start.getCol());
    region.setBlocked(col, row);
    if(region.isPassage(col, row))
    {
        numberUnblockedPassages--;
    }
    switch(curMove.direction){
        case Solver::Data::Down:
            return doMoveDown();
        case Solver::Data::Up:
            return doMoveUp();
        case Solver::Data::Right:
            return doMoveRight();
        case Solver::Data::Left:
            return doMoveLeft();
    }


    return false;
}

void Solver::regionDFS::backTrack()
{
    uint i;
    if(curMove.destination == curMove.start)
        return;
    if(region.isPassage(curMove.destination))
    {
        numberUnblockedPassages++;
    }
// first case: Right undo
    if(curMove.direction == Solver::Data::Right)
    {
        for(i=curMove.start.getCol(); i<curMove.destination.getCol(); ++i)
        {
            region.setUnblocked(i,curMove.start.getRow());
        }
    }else
// second case: Left undo
    if(curMove.direction == Solver::Data::Left)
    {
        for(i=curMove.start.getCol(); i>curMove.destination.getCol(); --i)
        {
            region.setUnblocked(i, curMove.start.getRow());
        }
    }else
// first case: Up undo
    if(curMove.direction == Solver::Data::Up)
    {
        for(i=curMove.start.getRow(); i>curMove.destination.getRow(); --i)
        {
            region.setUnblocked(curMove.start.getCol(), i);
        }
    }else
// first case: Right undo
    if(curMove.direction == Solver::Data::Down)
    {
        for(i=curMove.start.getRow(); i<curMove.destination.getRow(); ++i)
        {
            region.setUnblocked(curMove.start.getCol(), i);
        }
    }
    region.setUnblocked(curMove.destination.getCol(), curMove.destination.getRow());
    if(region.isPassage(curMove.destination.getCol(), curMove.destination.getRow()))
    {
        numberUnblockedPassages++;
    }
}


void Solver::regionDFS::reset()
{

    uint numberMoves = movelist.size();
    for(uint i=0; i < numberMoves; ++i)
    {
        curMove = movelist[movelist.size()-1-i];
        backTrack();

    }

    movelist.clear();
}
