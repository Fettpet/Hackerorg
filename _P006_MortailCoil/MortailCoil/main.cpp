#include <QtGui/QApplication>
#include "mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QTime>
#include <sstream>
#include "solver.h"
#include "splitter.h"
#include <fstream>

int main()
{

    std::fstream data;
    data.open("/home/fettpet/Dropbox/Programmieren/_P002_MortailCoil/coilstat.html", std::ios::app);
    data << "<html><body>" << std::endl;
    data << "<table border=\"1\"> <tr><td>Level</td><td>Size</td><td>Time</td></tr>";
    for(int i=0; i<130; ++i){
        world t;
        QTime timer;

        t.LoadFromInternet("Fettpet", "hahn181991");
        timer.start();
        bool found=false;
        for(int x=10; x<16; ++x){
            for(int y=4; y<8; ++y){
            std::cerr << "Current Start at: (" << x << ", " << y << ")" << std::endl;
                if(t.setStart(x,y)){
                    found = true;
                    break;
                }
                t.clear();
            }
            if(found){
                    break;
           }
        }
        std::cerr << std::endl << "Zeit für Lösung" << timer.elapsed() << " Lösung: " << t.getSolution().toStdString() << std::endl;
        data << "<tr><td>" << i << "</td><td> (" << t.getSizeX() << ", " << t.getSizeY() << ")" << "</td><td>";
        if(timer.elapsed() > 60000){
            data << (timer.elapsed() % (60 * 60000)) / 60000 << "min ";
        }
        if(timer.elapsed() > 1000){
            data << (timer.elapsed() % 60000) / 1000 << "s ";
        }
        data << timer.elapsed() % 1000 << "ms</th>" << std::endl;
        std::stringstream str;
        str << "Zeit für Lösung" << timer.elapsed();

        t.SubmitSolution("Fettpet","hahn181991");
    }
        data << "</html></body>" << std::endl;
*/
}
