#include "crosstolinearsystem.h"
#define DEBUG 1
/**************************************************************************************************
 *                                      Vars
 * ***********************************************************************************************/
Eigen::Matrix<int_gf2, Eigen::Dynamic, Eigen::Dynamic> crossToLinearSystem::A;
Eigen::Matrix<int_gf2, Eigen::Dynamic, 1> crossToLinearSystem::b;
QMap<Pos, int> crossToLinearSystem::PosToInt;
QMap<int, Pos> crossToLinearSystem::IntToPos;



/**************************************************************************************************
 *                                          Main function
 * ***********************************************************************************************/


/*
    Alle Zeilen elemente bis block addiern = 1 - ZellenValue

*/
QString crossToLinearSystem::solveSystem(const CrossFlip& sys){
#if DEBUG == 1
    std::cout << sys << std::endl;
#endif
    createVars(sys);
    createA(sys);
    createB(sys);

    //Eigen::PartialPivLU<Eigen::Matrix<int_gf2, Eigen::Dynamic, Eigen::Dynamic> > ldltOfA(A);
    Eigen::PartialPivLU< Eigen::Matrix<int_gf2, Eigen::Dynamic, Eigen::Dynamic> > lu( A );

    Eigen::Matrix<int_gf2, Eigen::Dynamic, 1> t(lu.solve(b));
    int count(0);
    QString solution;
    for(int y=0; y<sys.getSizeY(); ++y){
        for(int x=0; x<sys.getSizeX(); ++x){


            if(!sys.isBlocked(x,y)){
                solution += t(count, 0).getValue() + '0';
                count++;
            } else {
                solution += '0';
            }
        }
    }
#if DEBUG == 1
    std::cout << "Solution is " << solution.toStdString() << std::endl;
#endif
    return solution;
}



/**************************************************************************************************
 *                  Create Vars
 * ***********************************************************************************************/

int crossToLinearSystem::createVars(const CrossFlip& sys){
    PosToInt.clear();
    IntToPos.clear();
    int count = 0;
    for(int y=0; y<sys.getSizeY(); ++y){
        for(int x=0; x<sys.getSizeX(); ++x){

            if(!sys.isBlocked(x,y)){
#if DEBUG == 1
                std::cout << "(" << x << "," << y << ") = " << count<< std::endl;
#endif
                PosToInt.insert(Pos(x,y), count);
                IntToPos.insert(count, Pos(x,y));
                count++;
            }
        }
    }
    return count;
}

/**************************************************************************************************
 *                                  create linear System
 * ***********************************************************************************************/

void crossToLinearSystem::createA(const CrossFlip& t){
    const int countVar(IntToPos.size());
#if DEBUG == 1
    std::cout << "Count Var" << countVar << std::endl;
#endif
    Eigen::Matrix<int_gf2, Eigen::Dynamic, Eigen::Dynamic> _A;

    _A.resize(countVar, countVar);
    for(int i=0; i< countVar; ++i){
        for(int j=0; j<countVar; ++j){
            _A(i,j) = 0;
        }
    }
    for(int x=0; x< t.getSizeX(); ++x){
        for(int y=0; y<t.getSizeY(); ++y){
            if(t.isBlocked(x,y)) continue;

// nach Rechts

            for(int xx = x; xx >= 0; --xx){
                if(t.isBlocked(xx, y)) break;

                _A(PosToInt[Pos(x,y)], PosToInt[Pos(xx, y)]) = 1;
            }

// Nach unten
            for(int xx = x+1; xx< t.getSizeX(); ++xx){
                if(t.isBlocked(xx, y)) break;

                _A(PosToInt[Pos(x,y)], PosToInt[Pos(xx, y)]) = 1;
            }

// Nach Links
            for(int yy=y-1; yy >= 0; --yy){
                if(t.isBlocked(x,yy)) break;

                _A(PosToInt[Pos(x,y)], PosToInt[Pos(x, yy)]) = 1;
            }
// Nach Rechts
            for(int yy=y+1; yy < t.getSizeY(); ++yy){
                if(t.isBlocked(x,yy)) break;

                _A(PosToInt[Pos(x,y)], PosToInt[Pos(x, yy)]) = 1;
            }
//*/
        }
    }

    A = _A;
#if DEBUG == 1
    //std::cout << "New A is: " << std::endl << _A << std::endl;

#endif
}

void crossToLinearSystem::createB(const CrossFlip& t){
    const int countVar(IntToPos.size());
#if DEBUG == 1
    std::cout << "count Var in createB " << countVar << std::endl;
#endif
    Eigen::Matrix<int_gf2, Eigen::Dynamic, 1> _b;
// Wieso +1 ?

    _b.resize(countVar, 1);

    for(int i=0; i< countVar; ++i){
        _b(i,0) = 0;
    }

    for(int x=0; x< t.getSizeX(); ++x){
        for(int y=0; y<t.getSizeY(); ++y){
// nach oben
            if(!t.isBlocked(x,y)){
                _b(PosToInt[Pos(x,y)], 0) = t.hasStartValue(x,y);
#if DEBUG == 1
                std::cout << "StartValue: " << Pos(x,y) <<"=" << PosToInt[Pos(x,y)] << " ";
#endif
            }
        }
    }
    b = _b;
#if DEBUG == 1
    //std::cout << "b is " << std::endl << b << std::endl << std::endl;
#endif

}
