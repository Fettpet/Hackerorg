/**
  Ich benötige eine Datenstruktur um Wege zu Speichern, die nicht gelöst werden können. Ich benötige eine
  Datenstruktur um Wege zu speichern um sie später nicht nochmal zu berechnen
  Wie Speichere ich sackgassen
  Def Sackgasse.
  Eine Situation in der Backtracking betrieben werden muss. Sie besteht aus einem Startpunkt und einem Spielfeldausschnitt.
  Der Ausschnitt besteht aus n Regionen.

  Ein Startpunkt ist immer ein verbindungspunkt zwischen 2 Regionen. Wenn der RegionSolver keine Lösung findet, wird dies als
  Sackgasse in einer Liste aufgenommen. Durch das rekursive, werden Sackgassen durchpropagiert. Dies erfordert aber, dass alle nachfolger
  genauso aussehen. deshalb besteht ein ausschnitt aus mehreren Regionen

  Wie speichere ich
*/

#ifndef WORLDSOLVERBASECLASS_H
#define WORLDSOLVERBASECLASS_H
#include "../World/world.h"
#include "../Solver/solverbaseclass.h"


template<class RegionSolver>
class WorldSolverBaseClass
{
public:

    /**
      Constructor: need to spezify the world, to solve
     * @brief WorldSolverBaseClass
     * @param w
     */
    WorldSolverBaseClass(const World::world& w):
        world(w)
    {
        for(uint i=0; i<getNumberRegions(); ++i)
        {
            regionSolver.push_back(RegionSolver(world.getRegion(i)));

        }
        connectRegions();
    }


    /**
     * @brief solve. Solve the world
     */
    void solve()
    {
        Preprocessing();
        calcAllPossibleStartPositions();

        for(Position& pos: startPositions)
        {
            callAfterNewStartPosDefined();


        }
    }

    /**
     * @brief getSolutionString
     * @return the solutionstring for hacker.org
     */
    std::string getSolutionString();

protected:

    /**
     * @brief getNumberRegions
     * @return the number of regions in the world
     */
    uint getNumberRegions() const
    {
        return world.getNumberRegions();
    }

    virtual void callAfterNewStartPosDefined() = 0;
    virtual void calcAllPossibleStartPositions() = 0;
    virtual void Preprocessing() = 0;
    virtual void connectRegions();
    virtual void insertNewMoveSequence() = 0;

    std::vector<Position> startPositions;

    World::world world;

    std::vector<RegionSolver> regionSolver;
};


class Connection
{
public:


protected:
    uint WorldIDStart, WorldIDDestination;
    Position start, destination;

};

#endif // WORLDSOLVERBASECLASS_H
