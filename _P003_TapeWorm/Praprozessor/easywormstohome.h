#ifndef EASYWORMSTOHOME_H
#define EASYWORMSTOHOME_H
#include "World/world.h"
#include <set>
#include <vector>

typedef std::vector<bool> visitedMapInnerContainer;
typedef std::vector< visitedMapInnerContainer> visitedMap;

typedef std::vector<std::vector< uint> > SeperatedWormContainer;
typedef std::set<WormPart> WormPositionContainer;

class EasyWormsToHome
{
public:
    static std::string getEasyWormsToHome(world& w, uint& countOpen, uint& countClose);
    static bool canBringTheWormHome(const world& w, const WormPositionContainer& container, const Worm& wormId);
private:

    static void seperateWorms(world& w, SeperatedWormContainer& result);
    static std::string bringTheWormHome(world& w, const Worm& wurm, uint& countOpen, uint& countClose);
    static std::string bringTheWormHomeDeep(world& w, const Worm& wurm);
    static int getWurm(const SeperatedWormContainer& sep);
    static void findHomeForWorms(const WormMap& w, const uint color, WormPositionContainer& container);

    //static bool BringTheWormOneFieldToHome(world& w, const uint WormId);

};

/**
  1. Würmer Seperiern
  2. Einen Wurm finden, der als einzigster diese Farbe hat
  3. Seine Felder auf der Map finden
  4. Die Umgebung der Felder ansehen.
        Falls jedes entweder geblockt ist oder mindestens 2 Nachbarn frei sind gehts weiter

*/

#endif // EASYWORMSTOHOME_H
