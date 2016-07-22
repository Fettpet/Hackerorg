#include "solver.h"
#include <QTime>
#include <iostream>
#include <QMessageBox>
#define DEBUG 0

solver::solver()
{
}

void solver::exec(){
#if DEBUG == 1
    std::cout << "enter exec" << std::endl;
#endif
    world t;
    t.LoadFromInternet("Fettpet","hahn181991");
#if DEBUG == 1
    std::cout << "fertig mit laden" << std::endl;
#endif

    if(t.getCountImpasses() == 0){
#if DEBUG == 1
    std::cout << "Starten von Normal" << std::endl;
#endif
        normal(t);
    } else if(t.getCountImpasses() == 1) {
#if DEBUG == 1
    std::cout << "Starten von Normal, aber mit einer Sackgasse" << std::endl;
#endif
        normal(t);
    } else if(t.getCountImpasses() == 2){
#if DEBUG == 1
    std::cout << "Starten mit 2 Sackgassen" << std::endl;
#endif
        TwoAreGiven(t);
    }
#if DEBUG == 1
    std::cout << "leave exec" << std::endl;
#endif
}

/*
void solver::normal(world& t){

}
*/
void solver::normal(world &t){

    std::cout <<t.setStart(8,8);
}

void solver::oneIsGiven(world& t){

}

void solver::TwoAreGiven(world& t){
    QTime timer;
    timer.start();
    for(int i=0; i<=1; ++i){


        if(t.setStart(t.getImpasseList()[i].first, t.getImpasseList()[i].second)){
            break;
        }
        t.clear();
    }
    if(t.isValid()){
        std::cerr << "Zeit für Lösung" << timer.elapsed() << std::endl;
        t.SubmitSolution("Fettpet","hahn181991");
    } else{
        std::cerr << "Error keine Lösung gefunden in Twoaregiven";
    }
}
