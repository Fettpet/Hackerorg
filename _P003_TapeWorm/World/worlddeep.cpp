#include "worlddeep.h"
#include <Heuristics/astarheuristics.h>

std::vector<std::set< world> > WorldDeep::visitedList;
std::vector< boost::dynamic_bitset<> > WorldDeep::blocked;
/**************************************************************************************************
 *                              Constructor and Destructor
 * ***********************************************************************************************/
WorldDeep::WorldDeep(): world()
{

}

WorldDeep::WorldDeep(const WorldDeep& other):
    world(other), MoveContainer(other.MoveContainer)
{}

void WorldDeep::createBlockedMap(){
    blocked.clear();
    for(uint x = 0; x < getSizeX(); ++x){
        boost::dynamic_bitset<> buffer(getSizeY());
        blocked.push_back(buffer);
    }

    for(uint x=0; x< getSizeX(); ++x)
        for(uint y=0; y < getSizeY(); ++y){
            blocked[x][y] |= (getWormMap().getTheHomeFor(x,y) == 1);
        }
    for(const Worm& w: worms){
        for(const WormPart& p: w.getWormParts()){
            blocked[p.getPosX()][p.getPosY()] = true;
        }
    }
    std::cout <<"blocked Map" << std::endl;
    for(uint y=0; y < getSizeY(); ++y){
        for(uint x=0; x< getSizeX(); ++x){
            std::cout << blocked[x][y];
        }
        std::cout << std::endl;
    }
}

/**************************************************************************************************
 *                              Moves
 * ***********************************************************************************************/
void WorldDeep::getAllMoves(MoveInDeepContainer& con, Heuristics::AStarHeuristics& heur, uint deep){
    for(uint i=0; i<worms.size(); ++i){
        NeighborContainer* neighbor;
        neighbor = getNeighborFields(worms[i].getHead());
        WormPart old(worms[i].getTail());
        for(const WormPart& t: *neighbor){
            if(!isBlocked(t, i)){
                MoveInDeep buffer(worms[i].getHead(), t, i, true, old);
                move(buffer);
                bool found(false);
                for(int i=deep+1; i<visitedList.size(); ++i){
                    if(visitedList[i].find(*this) != visitedList[i].end()){
                        oneMoveBack();
                        found = true;
                        break;
                    }
                }
                if(found) continue;
                buffer.costToDest = heur(this);

                if(buffer.costToDest <= deep)
                    con.insert(buffer);
                oneMoveBack();
            }
        }

        neighbor = getNeighborFields(worms[i].getTail());
        old = worms[i].getHead();
        for(const WormPart& t: *neighbor){
            if(!isBlocked(t, i)){
                MoveInDeep buffer(worms[i].getTail(), t, i, false,old);
                move(buffer);
                bool found(false);
                for(uint i=deep+1; i<visitedList.size(); ++i){
                    if(visitedList[i].find(*this) != visitedList[i].end()){
                        oneMoveBack();
                        found = true;
                        break;
                    }
                }
                if(found) continue;
                buffer.costToDest = heur(this);

                if(buffer.costToDest <= deep)
                    con.insert(buffer);
                oneMoveBack();
            }
        }
    }
}

void WorldDeep::move(const MoveInDeep& m){

    if((m.destiny == worms[m.wormNumber].getHead()) || (m.destiny == worms[m.wormNumber].getTail())){

    } else {

        blocked[m.destiny.getPosX()].flip(m.destiny.getPosY());
        if(m.movedHead){
            blocked[worms[m.wormNumber].getTail().getX()].flip(worms[m.wormNumber].getTail().getY());
        } else {
             blocked[worms[m.wormNumber].getHead().getX()].flip(worms[m.wormNumber].getHead().getY());
        }

    }
    MoveContainer.push_back(m);
    worms[m.wormNumber].move(m.destiny, m.movedHead);
}

void WorldDeep::oneMoveBack(){
    MoveInDeep  *m = &MoveContainer[MoveContainer.size()-1];
    worms[m->wormNumber].move(m->old, 1-m->movedHead);
    blocked[m->destiny.getPosX()].flip(m->destiny.getPosY());
    if(m->movedHead){
        blocked[worms[m->wormNumber].getTail().getX()].flip(worms[m->wormNumber].getTail().getY());
    } else {
         blocked[worms[m->wormNumber].getHead().getX()].flip(worms[m->wormNumber].getHead().getY());
    }
    MoveContainer.pop_back();
}

std::string WorldDeep::getPath() const{
    std::stringstream str;
    for(const auto& t: MoveContainer){
        str << "m" << (int)t.start.getX() <<"." << (int)t.start.getY()
            << "-" << (int)t.destiny.getPosX() << "." << (int)t.destiny.getPosY();
    }

    return str.str();
}

void WorldDeep::clearMoves(){
    MoveContainer.clear();
}

bool WorldDeep::isBlocked(const WormPart& part, uint worm)const{
    if(worms[worm].getHead() == part) return false;
    if(worms[worm].getTail() == part) return false;
    return blocked[part.getPosX()][part.getPosY()];
}

std::ostream& operator<<(std::ostream& out, const MoveInDeep& move) {
    out << "Move: " << move.start  << " -> " << move.destiny;
    if(move.movedHead){
        out << " Head ";
    } else {
        out << " Tail ";
    }
    out << "cost " << move.costToDest;
    return out;
}


