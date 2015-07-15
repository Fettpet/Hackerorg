#include <iostream>
#include "int_gf2.h"
#include <QFile>
#include <QByteArray>
#include <cross.h>
#include "crosstolinearsystem.h"
#include <boost/timer.hpp>
#include "crosstobitset.h"
#include <fstream>
#include <boost/dynamic_bitset.hpp>
#include <QTime>


using namespace std;
using namespace Eigen;

int main(){



   std::fstream data;
   data.open("/home/fettpet/resultBit.html", std::ios::out| std::ios::app);

   for(int i=0; i< 200; ++i){
        std::system("curl --data \"name=fettpet&password=hahn181991\" -o /tmp/source.html http://www.hacker.org/cross/index.php");

        QFile file("/tmp/source.html");
        file.open(QFile::ReadOnly);
        QByteArray t =file.readAll();
        CrossFlip buffer(t);

        std::cout << "Level " << buffer.getLevel() << std::endl;
        std::cout << "Size: (" << buffer.getSizeX() << ", " << buffer.getSizeY() << ")" << std::endl;
        QTime timer;
        timer.restart();
        QString solutionString = CrossToBitset::solve(buffer);
        data << "<tr><th>" << buffer.getLevel() << "</th>"
             << "<th>(" << buffer.getSizeX() << ", " << buffer.getSizeY() << ")</th>"
             << "<th>" << timer.elapsed() / 1000 << "</th></tr>" << std::endl;
        std::stringstream sol;
        sol << "curl --data \"name=fettpet&password=hahn181991&sol=" <<  solutionString.toStdString() << "\""
            << " -o /tmp/error.html http://www.hacker.org/cross/index.php";
        std::system(sol.str().c_str());
   }
//*/
    return 0;
}
