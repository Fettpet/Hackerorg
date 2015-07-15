#include "robotstree.h"
#include <cstdlib>
#include <iostream>

RobotsTree::RobotsTree(){}

RobotsTree::RobotsTree(const RobotsWorld& world){
    int x=0, y=0;
    this->before = NULL;
    this->nextD = 0;
    this->nextR = 0;
    if(world.getSizeX() > 1)
        if(!world.getEntry(1,0)){
            RobotsTree *tree;
            tree = new RobotsTree(world, "R",1,0);
            this->nextR = tree;
        }
    if(world.getSizeY() > y)
        if(!world.getEntry(0,1)){
            RobotsTree *tree;
            tree = new RobotsTree(world, "D", 0,1);
            this->nextD = tree;
        }

}

RobotsTree::RobotsTree(const RobotsWorld& world, QString path, int x, int y){
    this->before = NULL;
    this->nextD = 0;
    this->nextR = 0;
    if(world.getSizeX() > x+1)
        if(!world.getEntry(x+1,y)){
            RobotsTree *tree;
            tree = new RobotsTree(world, path + "R",x+1,y);
            this->nextR = tree;
        }
    if(world.getSizeY() > y+1)
        if(!world.getEntry(x,y+1)){
            RobotsTree *tree;
            tree = new RobotsTree(world, path + "D", x, y+1);
            this->nextD = tree;
        }
}

QStringList RobotsTree::toStringList(){
    static int x=0, y=0;
    QStringList result;
    unsigned int sizeResultD = 0;
    if(nextD == NULL){
        result.append("");
        sizeResultD = 1;
    } else{
        result.append(nextD->toStringList());
        for(int i=0; i< result.size();++i){
            result[i] = "D" + result[i];
        }
        sizeResultD = result.size();
    }

    if(nextR == NULL){
        if(result.size() != 1)
            if(result.contains(""))
                result.append("");
    } else {
        result.append(nextR->toStringList());
        for(int i=sizeResultD; i< result.size();++i){
            result[i] = "R" + result[i];
        }
        sizeResultD = result.size();
    }
    return result;
}

QStringList RobotsTree::toStringList(int minLength){
    QStringList result = toStringList();
    for(int i=0;i<result.size(); ++i){
        if(result.value(i).size() <minLength){
            result.removeAt(i);
            i--;
        }
    }
    return result;
}

 unsigned int RobotsTree::maxDeepth(){
     unsigned int R,D;
     if(nextD != NULL){
         D = nextD->maxDeepth();
     } else {
         D = 0;
     }
     if(nextR != NULL){
         R = nextR->maxDeepth();
     } else {
         R = 0;
     }
     return std::max(D,R)+1;
 }
