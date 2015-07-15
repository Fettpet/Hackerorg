#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
#include <algorithm>
#include "bestpathentry.h"
#include "robotstree.h"
#include "robotsbacktracking.h"
#include <QTime>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}
/*
void MainWindow::start(){
    RobotsWorld world;
    unsigned int level = 504;
    for(int i=level; i<=513; ++i){
        std::stringstream str;
        str << "/home/fettpet/welt" << i << ".txt";
        world.ReadFromInternet("fettpet", "hahn181991");
        world.setUnreachableToBlocked();
        world.setImpasseToBlock();
       // world.showMessageBox();
       // ui->textEdit->setText(world.getField());
        robotsbacktracking back(world);
        QTime timer;
        world.WriteToData(str.str().c_str());
        timer.start();
        QString result;
        result = back.calculate();
        std::stringstream s; s << "Level: " << i << " time" << timer.elapsed() << " path: " << result.toStdString();
        ui->textEdit->append(s.str().c_str());
        world.submitSolution(result, "fettpet", "hahn181991");
    }
}
*/

void MainWindow::start(){

        unsigned int level =465;
        RobotsWorld world;
        std::stringstream str;
        str << "/home/fettpet/welt" << level << ".txt";
        world.ReadFromData();
        robotsbacktracking back(world);
        QTime timer;
        timer.start();
        QString result;
        result = back.calculate();
        world.submitSolution(result, "fettpet", "hahn181991");
        std::cerr << "Level: " << level << " time" << timer.elapsed() << " Lösung: " << result.toStdString().c_str() << std::endl;
}

