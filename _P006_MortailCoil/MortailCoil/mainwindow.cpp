#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>
#include <QTime>
#include <sstream>
#include "solver.h"
#include "splitter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::DoIt(){

    for(int i=0; i<100; ++i){
        world t;
        QTime timer;

        t.LoadFromInternet("Fettpet", "hahn181991");
        timer.start();
        bool found=false;
        for(int x=0; x<t.getSizeX(); ++x){
            for(int y=0; y<t.getSizeY(); ++y){
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
        std::stringstream str;
        str << "Zeit für Lösung" << timer.elapsed();
        ui->textEdit->append(str.str().c_str());
        t.SubmitSolution("Fettpet","hahn181991");
    }
}

/* Der Fehler liegt irgentwo hier
   Die ImpassListe wird nicht ordnungsgemäß gelöscht
*/
void MainWindow::Draw(){
    world t;
    t.LoadFromInternet("Fettpet", "hahn181991");

    ui->textEdit->setText(t.getTheWorldAsHTMLString());
    if(!t.checkRowCanGoOn(1)){
        ui->textEdit->append("OnRowCanGoOn(1)");
    }
    if(!t.checkColumnCanGoOn(1)){
         ui->textEdit->append("OnColumnCanGoOn(1)");
    }
    ui->textEdit->append(t.getSolution());
    Splitter split;
    split.exec(t);
    QMessageBox a;
    a.setText(split.getTheSplitAsString());
    a.exec();
}
/*
void MainWindow::Draw(){
    world t;
    t.LoadFromInternet("Fettpet", "hahn181991");
    t.setStart(1,0);
    t.doAMove(Left);
    ui->textEdit->setText(t.getTheWorldAsHTMLString());
}
*/
