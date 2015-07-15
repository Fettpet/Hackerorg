#include "robotsbacktracking.h"
#include <cmath>
#include <QStack>
#include <set>
#include <algorithm>
#include <QList>

/********************************************************************************************************************************
                                Konstruktoren
********************************************************************************************************************************/

robotsbacktracking::robotsbacktracking(RobotsWorld &world){
    world.setUnreachableToBlocked();
    this->world = world;
    WayOut = world.getPointThereCanGoOut();
}

/*********************************************************************************************************************************
                                        Hilfsfunktionen
***********************************************************************************************************************************/


/*

*/
QVector<point>  robotsbacktracking::getAllPointsRestart(){
    QVector<point> result;
    for(int steps=world.getMinSteps(); steps<= world.getMaxSteps(); ++steps){
        for(int countD=0; countD <= steps;++countD){
            if(world.isPassable(steps -countD, countD)){
                point add;
                add.x = steps- countD;
                add.y = countD;
                result.push_back(add);
            }
        }
    }
    return result;
}

/* wayCanGoOutumstelle
    schaut ob der Pfad der geliefert wurde überhaupt nach draußen gelangen kann
    es muss geschaut werden ob sie einen Geschwindigkeitsvorteil bringt, was bezweifelt werden darf
    @param path Der Pfad der überprüft werden soll
    @return true falls er hinausführt
            false falls er nicht hinausführt.
    NOCH NICHT debuggt
*/
bool robotsbacktracking::wayCanGoOut(const QString& path)const {
    unsigned int countD = 0, countR =0;
// die runter und rechts zählen
    for(int i=0; i<path.size(); ++i){
        if(path.at(i) == 'D'){
            countD++;
        } else{
            countR++;
        }
    }

// bestimmen was raus führt
    double right, down;
    right = (double)world.getSizeX() / (double)countR;
    down = (double)world.getSizeY() / (double)countD;

// schauen wie oft der Pfad durchgegangen werden muss
    unsigned int numberIterationen;
    if(right > down){
        numberIterationen = 1.0 / right; // wird abgerundet
    } else {
        numberIterationen = 1.0 / down; // wird abgerundet
    }
    countD *= numberIterationen;
    countR *= numberIterationen;
    for(int i=0; i< path.size(); ++i){
        if(path.at(i) =='D'){
            countD++;
        } else{
            countR++;
        }
        if((countD == world.getSizeX()) || (countR == world.getSizeY())){
            point add;
            add.x = countR;
            add.y = countD;
            return WayOut.contains(add);
        }
    }
    return false;
}

/* canIgnore
  Berechnet wie viele Pfade nicht überprüft werden müssen
  @param Den aktuellen Pfad
  @return gibt an wie viele möglichkeiten übersprungen werden können

*/
BigInteger robotsbacktracking::canIgnore(const QString& path)const {

}


/* calculateMinMaxSteps
    Setzt die Minimalen und Maximalen Werte fest, wie oft R bzw D in einem Path vorkommen muss
    @param steps Die anzahl er Steps in path
    @return void
*/
void robotsbacktracking::calculateMinMaxSteps(const unsigned int steps){
    unsigned int minRightOut = world.getSizeX(), maxRightOut = 0, minDownOut = world.getSizeY(), maxDownOut = 0;
    for(int i=0;i<WayOut.size(); ++i){
        if(WayOut[i].x == world.getSizeX()){
            if(WayOut[i].y > maxRightOut){
                maxRightOut = WayOut[i].y;
            }
            if(WayOut[i].y < minRightOut){
                minRightOut = WayOut[i].y;
            }
        } else{
            if(WayOut[i].x > maxDownOut){
                maxDownOut = WayOut[i].x;
            }
            if(WayOut[i].x < maxDownOut){
                maxDownOut = WayOut[i].x;
            }
        }
    }

    if(minDownOut > maxDownOut){
        // Es muss nur Rechts überprüft werden
        // Nach rechts bestimmen


        // Min
        unsigned int stepsToGo;
        stepsToGo = world.getSizeX() + maxRightOut;
        minStepR = world.getSizeX() / std::ceil((double)stepsToGo / (double)steps);
        // max
        stepsToGo = world.getSizeX() + minRightOut;
        maxStepR = world.getSizeX() / ((stepsToGo / steps));
       // nach links
        minStepD = steps - maxStepR;
        maxStepD = steps - minStepR;
        std::cerr << "minDown > maxDown" << std::endl;
    } else if(minRightOut > maxRightOut){
        // Es muss nur Unten überprüft werden
        // Nach untenbestimmen
        // Min
        unsigned int stepsToGo;
        stepsToGo = world.getSizeY() + maxDownOut;
        minStepD = world.getSizeY() / std::ceil((double)stepsToGo / (double)steps);
        // max
        stepsToGo = world.getSizeY() + minDownOut;
        maxStepD = world.getSizeY() / (stepsToGo / steps);
       // nach Rechts
        minStepR = steps - maxStepD;
        maxStepR = steps - minStepD;
        std::cerr << "minRight > maxRight" << std::endl;
    } else {
        unsigned int stepsToGo;
        stepsToGo = world.getSizeX() + maxRightOut;
        minStepR = world.getSizeX() / std::ceil((double)stepsToGo / (double)steps);
        // max
        stepsToGo = world.getSizeX() + minRightOut;
        maxStepR = world.getSizeX() / (stepsToGo / steps);

        stepsToGo = world.getSizeY() + maxDownOut;
        minStepD = world.getSizeY() / std::ceil((double)stepsToGo / (double)steps);
        // max
        stepsToGo = world.getSizeY() + minDownOut;
        maxStepD = world.getSizeY() / (stepsToGo / steps);
    }
    std::cerr << "Anzahl Schritte: " << steps << std::endl;
    std::cerr << "DownMin: " << minDownOut << " DownMax: " << maxDownOut << " RightMax: " << maxRightOut << " Rightmin: " << minRightOut << std::endl;
    std::cerr << "MaxD" << maxStepD << " MinD" << minStepD << " maxStepR " << maxStepR << " minstepR" << minStepR << std::endl;
}

/************************************************************************************************************************
                    Alle Permutationen durchgehen
************************************************************************************************************************/
QString robotsbacktracking::calculate(){
    QVector<point> list;
    list = getAllPointsRestart();
    for(int i=0; i<list.size(); ++i){
        QString path = calculateAllPermutationsBacktracking(list.value(i));
        if(!path.isEmpty()) return path;
        std::cerr << i << " geschafft" << " anz:" << list.size() <<  " x: " << list[i].x << " y: " << list[i].y << std::endl;
    }
    return "error";
}

/* calculateNewRobotsWorld
    schreibt die späteren wiederholdung in den ersten Quadranten
    @param countD Die anzahl der Schritte nach unten im pfad
    @param countR Die anzahl der Schritte nach Rechts im Pfad161
    @return die neue Welt
  */
RobotsWorld robotsbacktracking::calculateNewRobotsWorld(unsigned int countD, unsigned int countR){
    RobotsWorld result(world);

    unsigned int n =1;
    do{
        for(int x=0; x<= countD; ++x){
            for( int y=0; y<= countR; ++y){
                if((result.getSizeX() > x + countR*n) && (result.getSizeY() > y + countD*n)){
                   // std::cout << "x: " << x << " y: " << y << " w(x,y) " << result.getEntry(x,y);
                   // std::cout << " x+CountR" << x + countR*n << " y+countD " << y+countD*n << " w(x,y): " <<  result.getEntry((x +countR*n), y + countD*n) << std::endl;
                    result.setEntry(x, y, result.getEntry(x,y) | result.getEntry( x+countR*n, y+countD*n));
                }
            }
        }
        ++n;
    } while((countR * (n-150) > result.getSizeX()) && (countD * (n-150) > result.getSizeY()));
 //   if(result.isPassable(0,0))
 //        result.showMessageBox();
    for(int x=0; x < countR; ++x){
        for(int y=countD+1; y<2*countD; ++y){
            result.setBlocked(x,y);
        }
    }

    for(int y=0; y < countD; ++y)
        for(int x=countR+1; x<countR*2; ++x)
            result.setBlocked(x,y);
    result.setUnreachableToBlocked();
    result.setImpasseToBlock();
    if(result.isPassable(0,0)){
        point t;
        t.x = countD;
        t.y = countR;
       // std::cerr << result.getField().toStdString();
    }
    return result;
}

/*
    Schaut alle möglichen Permutationen durch, ob ein Valider Pfad dabei ist
    @param countD anzahl der Schritte nach unten
    @param countR anzahl der Schritte nach rechts
    @return einen Validen Pfad, falls es einen gibt, oder einen leeren String falls nicht
*/
QString robotsbacktracking::calculateAllPermutationsBacktracking(unsigned int countD, unsigned int countR){
    RobotsWorld currentWorld = calculateNewRobotsWorld(countR, countD);
    currentWorld.setUnreachableToBlocked();
    currentWorld.setImpasseToBlock();

    if(currentWorld.isBlocked(0,0)) return "";
  //  currentWorld.showMessageBox();
    unsigned int steps = countD + countR;

    BestPathEntry current;
    unsigned int anz=0;
    // Die ersten zwei in ToDo laden
    if(currentWorld.isPassable(1,0)){
        current.path = "R";
        current.posX = 1;
        current.posY = 0;
    } else if(currentWorld.isPassable(0,1)){
        current.path = "D";
        current.posX = 0;
        current.posY = 1;
    } else {
        return "";
    }

    while(1){
        //std::cerr << current.path.toStdString() << std::endl;
        if(current.path.size() == steps){
            //std::cerr << current.path.toStdString() << std::endl;
            if(currentWorld.testPath(current.path)) return current.path;
                unsigned int countDown = 0, countRight = 0;
                bool chance = false;
                for(int i=current.path.size(); i>=0; --i){
                    if(current.path[i] == 'R'){
                        countDown = 0;

                        countRight = 0;
                        for(int j=0; j< i; ++j){
                            if(current.path[j] == 'R') countRight++;
                            if(current.path[j] == 'D') countDown++;
                        }
                        if(currentWorld.isPassable(countRight, countDown+1)){
                            current.path = current.path.mid(0,i) + 'D';
                            current.posX = countRight;
                            current.posY = countDown+1;
                            chance = true;
                            break;
                        }
                    }
                }

                if(!chance) return "";

                continue;


        }
        bool chance = false;
        if(current.posX + 1 < currentWorld.getSizeX())
            if(currentWorld.isPassable(current.posX+1, current.posY)){
                current.path += 'R';
                current.posX++;
                continue;
            }
        if(current.posY + 1 < currentWorld.getSizeY())
            if(currentWorld.isPassable(current.posX, current.posY + 1)){
                current.path += 'D';
                current.posY++;
                continue;
            }

        chance = false;

        for(int i=current.path.size(); i>=0 ; --i){
            unsigned int countRight = 0, countDown = 0;
            if(current.path[i] == 'R'){
                for(int j=0; j< i; ++j){
                    if(current.path[j] == 'R') countRight++;
                    if(current.path[j] == 'D') countDown++;
                }
            }
            if(currentWorld.isPassable(countRight, countDown+1)){
                current.path = current.path.mid(0,i) + 'D';
                current.posX = countRight;
                current.posY = countDown+1;
                chance = true;
                break;
            }
        }
        if(!chance) return "";
    }

    return "";
}

QString robotsbacktracking::calculateAllPermutationsBacktracking(point t){
    return calculateAllPermutationsBacktracking(t.x, t.y);
}
QString robotsbacktracking::testAllPermutations(unsigned int countD, unsigned int countR){
    RobotsWorld currentWorld = calculateNewRobotsWorld(countR, countD);
    currentWorld.setUnreachableToBlocked();
    currentWorld.setImpasseToBlock();

    if(currentWorld.isBlocked(0,0)) return "";
    unsigned int steps = countD +countR;
    char f[steps], *const fn = f + sizeof(f) / sizeof(*f);

    for(int i=0; i<countD; ++i){
        f[i] = 'D';
    }
    for(int i=countD; i<countD+countR; ++i){
        f[i] = 'R';
    }

    do
    {
        QString buffer = f;
        buffer = buffer.mid(0, steps);
        if(currentWorld.testPath(buffer)) return buffer;
    }
    while(std::next_permutation(f, fn));


    return "";
}

QString robotsbacktracking::testAllPermutations(point t){
    return testAllPermutations(t.x, t.y);
}

 QString robotsbacktracking::BackTracking(){


     BestPathEntry current;
     unsigned int anz=0;
     // Die ersten zwei in ToDo laden
     if(!world.isBlocked(1,0)){
         current.path = "R";
         current.posX = 1;
         current.posY = 0;
     } else if(world.isPassable(0,1)){
         current.path = "D";
         current.posX = 0;
         current.posY = 1;
     } else {
         return "error";
     }

     while(1){
        // std::cerr << current.path.toStdString() << std::endl;
         if(current.path.size() >= world.getMinSteps()){

             if(world.testPath(current.path)) return current.path;
             if(current.path.size() == world.getMaxSteps()){
                 unsigned int countDown = 0, countRight = 0;
                 bool chance = false;
                 for(int i=current.path.size(); i>=0; --i){
                     if(current.path[i] == 'R'){
                         countDown = 0;

                         countRight = 0;
                         for(int j=0; j< i; ++j){
                             if(current.path[j] == 'R') countRight++;
                             if(current.path[j] == 'D') countDown++;
                         }
                         if(world.isPassable(countRight, countDown+1)){
                          //   std::cerr << "i:" << i << std::endl;
                             current.path = current.path.mid(0,i) + 'D';
                             current.posX = countRight;
                             current.posY = countDown+1;
                             chance = true;
                             break;
                         }
                     }
                 }

                 if(!chance) return "";

                 continue;

             }
         }
         bool chance = false;
         if(current.posX + 1 < world.getSizeX())
             if(world.isPassable(current.posX+1, current.posY)){
                 current.path += 'R';
                 current.posX++;
                 continue;
             }
         if(current.posY + 1 < world.getSizeY())
             if(world.isPassable(current.posX, current.posY + 1)){
                 current.path += 'D';
                 current.posY++;
                 continue;
             }

         chance = false;

         for(int i=current.path.size(); i>=0 ; --i){
             unsigned int countRight = 0, countDown = 0;
             if(current.path[i] == 'R'){
                 for(int j=0; j< i; ++j){
                     if(current.path[j] == 'R') countRight++;
                     if(current.path[j] == 'D') countDown++;
                 }
             }
             if(world.isPassable(countRight, countDown+1)){
                 current.path = current.path.mid(0,i) + 'D';
                 current.posX = countRight;
                 current.posY = countDown+1;
                 chance = true;
                 break;
             }
         }
         if(!chance) return "";
     }

     return "";
 }
