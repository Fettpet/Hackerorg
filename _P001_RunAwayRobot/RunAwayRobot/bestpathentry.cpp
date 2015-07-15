#include "bestpathentry.h"

BestPathEntry::BestPathEntry()
{
    count = 0;
    stepCount = 0;
    path = "";
}

BestPathEntry::BestPathEntry(const BestPathEntry& k){
    count = k.count;
    stepCount = k.stepCount;
    path = k.path;
    posX = k.posX;
    posY = k.posY;
}

BestPathEntry::BestPathEntry(const QString& path){
    count =0;
    stepCount = 0;
    posX = 0;
    posY = 0;
    this->path = path;
    for(int i=0; i<path.size(); ++i){
        if(path[i] == 'D'){
            posY++;
        } else{
            posX++;
        }
    }
}

BestPathEntry::BestPathEntry(const char* path){
    count =0;
    stepCount = 0;
    posX = 0;
    posY = 0;
    this->path = path;
    for(int i=0; i<this->path.size(); ++i){
        if(this->path[i] == 'D'){
            posY++;
        } else{
            posX++;
        }
    }
}

bool BestPathEntry::operator==(const BestPathEntry &t) const{
    if(t.path != path) return false;
    if(t.count != count) return false;
    if(t.stepCount != stepCount) return false;
    return true;
}
bool BestPathEntry::operator==(BestPathEntry &t) const{
    if(t.path != path) return false;
    if(t.count != count) return false;
    if(t.stepCount != stepCount) return false;
    return true;
}

bool BestPathEntry::operator<(const BestPathEntry &t) const{
    return (count < t.count);
}
bool BestPathEntry::operator<(BestPathEntry &t) const{
    return (count < t.count);
}

BestPathEntry& BestPathEntry::operator = (const BestPathEntry& src){
    count = src.count;
    path = src.path;
    stepCount = src.stepCount;
    posX = src.posX;
    posY = src.posY;
}

bool BestPathEntry::isReady(){
    return (stepCount == path.size());
}

std::ostream& operator<< (std::ostream& result,BestPathEntry &val){
    result << "Path:" << val.path.toStdString().c_str() << " Count:" << val.count << " Steps:" << val.stepCount;
    return result;
}
