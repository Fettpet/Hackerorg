#ifndef CROSSTOLINEARSYSTEM_H
#define CROSSTOLINEARSYSTEM_H
#include <QString>
#include "int_gf2.h"
#include "pos.h"
#include "cross.h"
#include <QMap>

class crossToLinearSystem
{
public:
    crossToLinearSystem();
    static QString solveSystem(const CrossFlip& t);

    static int createVars(const CrossFlip& t);
    static void createA(const CrossFlip& t);
    static void createB(const CrossFlip& t);
private:

// Vars
    static Eigen::Matrix<int_gf2, Eigen::Dynamic, Eigen::Dynamic> A;
    static Eigen::Matrix<int_gf2, Eigen::Dynamic, 1> b;
    static QMap<Pos, int> PosToInt;
    static QMap<int, Pos> IntToPos;
    /*
        int_gf2 C(21), B(10);
        std::log(B);
        std::cout << "A" << C << " B" << B <<  " A+B" << C+B << std::endl;
        Matrix<int_gf2, 3, 3> A;
       // Matrix3f A;
        Matrix<int_gf2, 3, 1> b;
        A <<  1.,2.,3., 4.,5.,6., 7.,8.,10.;
        b << 1., 1., 1.;
        std::cout << "A is" << std::endl << A << std::endl;
        std::cout << "b is " << std::endl <<b << std::endl;

        FullPivLU< Matrix<int_gf2, 3, 3> > lu( A );
        std::cout << lu.solve(b);
        //A.colPivHouseholderQr().solve(b);
       // cout << "The solution is:\n" << x << endl;
       */
};

#endif // CROSSTOLINEARSYSTEM_H
