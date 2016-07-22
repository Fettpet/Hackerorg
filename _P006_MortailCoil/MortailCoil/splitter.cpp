#include "splitter.h"
#include <QMessageBox>
#include <iostream>
Splitter::Splitter()
{
}


void Splitter::exec(const world& input){
    worlds.clear();
    worlds.push_back(input);
    createEntraceList();
    deleteEntracesTheyEndInTheSameField();
    deleteEntracesNeighbour();

    testTheEntraces();

    world t;
    t = worlds[0];
    for(int x=0; x<t.getSizeX(); ++x){
        for(int y=0; y<t.getSizeY(); ++y){
            t.setValue(x,y,1);
        }
    }
    for(int i=1; i<worlds.size(); ++i)
        for(int x=0; x<t.getSizeX(); ++x){
            for(int y=0; y<t.getSizeY(); ++y){
                if(worlds[i].getValue(x,y) == 0){
                    t.setValue(x,y,0);
                }
            }
        }
}

/**************************************************************************************************
                              Entraces
**************************************************************************************************/

/**
  Testet alle Entraces
*/
void Splitter::testTheEntraces(){
    for(int i=0; i<Entrance.size(); ++i){
        if(!worlds[0].isBlocked(Entrance[i].first+1, Entrance[i].second)){
            std::pair<int, int> point = std::make_pair(Entrance[i].first+1, Entrance[i].second);
            world buffer = testEntrace(worlds[0], point);
            if(!worlds.contains(buffer)){
                worlds.push_back(buffer);
            }
        }
        if(!worlds[0].isBlocked(Entrance[i].first-1, Entrance[i].second)){
            std::pair<int, int> point = std::make_pair(Entrance[i].first-1, Entrance[i].second);
            world buffer = testEntrace(worlds[0], point);
            if(!worlds.contains(buffer)){
                worlds.push_back(buffer);
            }
        }
        if(!worlds[0].isBlocked(Entrance[i].first, Entrance[i].second+1)){
            std::pair<int, int> point = std::make_pair(Entrance[i].first, Entrance[i].second+1);
            world buffer = testEntrace(worlds[0], point);
            if(!worlds.contains(buffer)){
                worlds.push_back(buffer);
            }
        }
        if(!worlds[0].isBlocked(Entrance[i].first, Entrance[i].second-1)){
            std::pair<int, int> point = std::make_pair(Entrance[i].first, Entrance[i].second-1);
            world buffer = testEntrace(worlds[0], point);
            if(!worlds.contains(buffer)){
                worlds.push_back(buffer);
            }
        }
    }

}

/**
    @param t Die Welt die aufgesplittet werden soll
    @param point Der Punkt an dem Angefangen werden soll
*/
world Splitter::testEntrace(const world& t, std::pair<int, int> point){
    world result(t);
    QVector<std::pair<int, int> > PointsIWas, PointsIMustVisit;
    PointsIMustVisit.push_back(point);
    while(!PointsIMustVisit.isEmpty()){
        std::pair<int, int> current;
        current = PointsIMustVisit[0];
        PointsIWas.push_back(current);
        PointsIMustVisit.remove(0);
        std::pair<int, int> buffer = std::make_pair(current.first+1, current.second);
        if(!t.isBlocked(buffer.first, buffer.second)){
            if(!PointsIWas.contains(buffer)&&
              (!PointsIMustVisit.contains(buffer))){
                if(!Entrance.contains(buffer)){
                    PointsIMustVisit.push_back(buffer);
                } else {
                    PointsIWas.push_back(buffer);
                }
            }
        }
        buffer = std::make_pair(current.first-1, current.second);
        if(!t.isBlocked(buffer.first, buffer.second)){
            if(!PointsIWas.contains(buffer)&&
              (!PointsIMustVisit.contains(buffer))){
                if(!Entrance.contains(buffer)){
                    PointsIMustVisit.push_back(buffer);
                } else {
                    PointsIWas.push_back(buffer);
                }
            }
        }
        buffer = std::make_pair(current.first, current.second+1);
        if(!t.isBlocked(buffer.first, buffer.second)){
            if(!PointsIWas.contains(buffer)&&
              (!PointsIMustVisit.contains(buffer))){
                if(!Entrance.contains(buffer)){
                    PointsIMustVisit.push_back(buffer);
                } else {
                    PointsIWas.push_back(buffer);
                }
            }
        }
        buffer = std::make_pair(current.first, current.second-1);
        if(!t.isBlocked(buffer.first, buffer.second)){
            if(!PointsIWas.contains(buffer)&&
              (!PointsIMustVisit.contains(buffer))){
                if(!Entrance.contains(buffer)){
                    PointsIMustVisit.push_back(buffer);
                } else {
                    PointsIWas.push_back(buffer);
                }
            }
        }
    }
    for(int x=0; x<result.getSizeX(); ++x){
        for(int y=0; y<result.getSizeY(); ++y){
            result.setValue(x,y,1);
        }
    }
    for(int i=0; i<PointsIWas.size(); ++i){
        result.setValue(PointsIWas[i].first, PointsIWas[i].second,0);
    }
    return result;
}

/**************************************************************************************************
                        Funktionen für die Entrace Liste
**************************************************************************************************/

/** Erstellt eine Liste, die alle Eingänge enthält

*/
void Splitter::createEntraceList(){
    if(worlds.isEmpty()) return;
    for(int x=0; x<worlds[0].getSizeX(); ++x)
        for(int y=0; y<worlds[0].getSizeY(); ++y){
            if(worlds[0].isOneWayBegin(x,y)){
                Entrance.push_back(std::make_pair(x,y));
            }
        }
}

/**
    Löscht alle Einträge aus der EntraceListe die im selben Feld enden, in dem sie beginnen
*/
void Splitter::deleteEntracesTheyEndInTheSameField(){
    for(int i=0;i<Entrance.size(); ++i){
        QVector< std::pair<int, int> > IWasPoint, IMustVisitPoints;
        if(!worlds[0].isBlocked(Entrance[i].first+1, Entrance[i].second)){
            IMustVisitPoints.push_back( std::make_pair(Entrance[i].first+1, Entrance[i].second));
        } else if(!worlds[0].isBlocked(Entrance[i].first-1, Entrance[i].second)){
            IMustVisitPoints.push_back( std::make_pair(Entrance[i].first-1, Entrance[i].second));
        } else if(!worlds[0].isBlocked(Entrance[i].first, Entrance[i].second+1)){
            IMustVisitPoints.push_back( std::make_pair(Entrance[i].first, Entrance[i].second+1));
        } else if(!worlds[0].isBlocked(Entrance[i].first, Entrance[i].second-1)){
            IMustVisitPoints.push_back( std::make_pair(Entrance[i].first, Entrance[i].second-1));
        }

        while(!IMustVisitPoints.isEmpty()){
            std::pair<int, int> current = IMustVisitPoints.first();
            IMustVisitPoints.remove(0);
            IWasPoint.push_back(current);
            if(worlds[0].isOneWayBegin(current.first, current.second)){
                continue;
            }
// Nach Rechts
            if(!worlds[0].isBlocked(current.first+1, current.second)){
                if(!IMustVisitPoints.contains(std::make_pair(current.first+1, current.second)) &&
                  (!IWasPoint.contains(std::make_pair(current.first+1, current.second)))){
                        IMustVisitPoints.push_back(std::make_pair(current.first+1, current.second));

                } else if(worlds[0].isOneWayBegin(current.first+1, current.second)){
                    IWasPoint.push_back(std::make_pair(current.first+1, current.second));
                }
            }
// Nach Links
            if(!worlds[0].isBlocked(current.first-1, current.second)){
                if(!IMustVisitPoints.contains(std::make_pair(current.first-1, current.second)) &&
                  (!IWasPoint.contains(std::make_pair(current.first-1, current.second)))){
                        IMustVisitPoints.push_back(std::make_pair(current.first-1, current.second));
                } else if(worlds[0].isOneWayBegin(current.first-1, current.second)){
                    IWasPoint.push_back(std::make_pair(current.first-1, current.second));
                }
            }
// Nach Oben
            if(!worlds[0].isBlocked(current.first, current.second-1)){
                if(!IMustVisitPoints.contains(std::make_pair(current.first, current.second-1)) &&
                  (!IWasPoint.contains(std::make_pair(current.first, current.second-1)))){
                        IMustVisitPoints.push_back(std::make_pair(current.first, current.second-1));
                } else if(worlds[0].isOneWayBegin(current.first, current.second-1)){
                    IWasPoint.push_back(std::make_pair(current.first, current.second-1));
                }
            }
// Nach Unten
            if(!worlds[0].isBlocked(current.first, current.second+1)){
                if(!IMustVisitPoints.contains(std::make_pair(current.first, current.second+1)) &&
                  (!IWasPoint.contains(std::make_pair(current.first, current.second+1)))){
                        IMustVisitPoints.push_back(std::make_pair(current.first, current.second+1));
                } else if(worlds[0].isOneWayBegin(current.first, current.second+1)){
                    IWasPoint.push_back(std::make_pair(current.first, current.second+1));
                }
            }
        };
        int sum =0;
        for(int j=0; j<IWasPoint.size(); ++j){
            if(IWasPoint[j] == Entrance[i]){
                sum++;
            }
        }
        if(sum>=2){
            Entrance.remove(i);
            i--;
        }
    }
}

/**
    Löscht alle Eingänge, die direkt nachbarn sind
*/
void Splitter::deleteEntracesNeighbour(){
    for(int i=0; i<Entrance.size();++i){
        for(int j=0; j<Entrance.size(); ++j){

            if((Entrance[i].first+1 == Entrance[j].first) && (Entrance[i].second == Entrance[j].second)){
                Entrance.remove(i);
                i--;
                break;
            }

            if((Entrance[i].first-1 == Entrance[j].first) && (Entrance[i].second == Entrance[j].second)){
                Entrance.remove(i);
                i--;
                break;
            }
            if((Entrance[i].first == Entrance[j].first) && (Entrance[i].second+1 == Entrance[j].second)){
                Entrance.remove(i);
                i--;
                break;
            }
            if((Entrance[i].first == Entrance[j].first) && (Entrance[i].second-1 == Entrance[j].second)){
                Entrance.remove(i);
                i--;
                break;
            }

        }
    }
}


/**************************************************************************************************
                                            Getter
**************************************************************************************************/
/**
  @return Anzahl der Welten die gesplittert wurden
*/
int Splitter::countNumberWorlds()const{
    return worlds.size();
}
/**
  @param Die Welt die zurückgegeben werden soll
  @return Eine Welt
*/
world Splitter::getWorld(int nr)const{
    if(worlds.size() <= nr){
        std::cerr << "getWorld Parameter ist falsch" << nr << std::endl;
    } else{
        return worlds[nr];
    }
}

QString Splitter::getTheSplitAsString()const{
    QVector<QString> colors;
    colors.push_back("<font color=\"#000000\">");
    colors.push_back("<font color=\"#ff0033\">");
    colors.push_back("<font color=\"#aa5500\">");
    colors.push_back("<font color=\"#0000ff\">");
    colors.push_back("<font color=\"#00ff00\">");
    colors.push_back("<font color=\"#ff0000\">");
    colors.push_back("<font color=\"#ffff00\">");
    colors.push_back("<font color=\"#00ffff\">");
    colors.push_back("<font color=\"#ff00ff\">");
    colors.push_back("<font color=\"#880088\">");
    colors.push_back("<font color=\"#880000\">");
    colors.push_back("<font color=\"#008800\">");
    colors.push_back("<font color=\"#000088\">");
    colors.push_back("<font color=\"#888800\">");
    colors.push_back("<font color=\"#008888\">");
    colors.push_back("<font color=\"#5533ff\">");
    colors.push_back("<font color=\"#a2b64f\">");
    colors.push_back("<font color=\"#000000\">");
    colors.push_back("<font color=\"#000000\">");
    colors.push_back("<font color=\"#000000\">");
    colors.push_back("<font color=\"#000000\">");
    colors.push_back("<font color=\"#000000\">");
    colors.push_back("<font color=\"#000000\">");
    colors.push_back("<font color=\"#000000\">");
    colors.push_back("<font color=\"#000000\">");
    colors.push_back("<font color=\"#000000\">");
    std::cout << worlds.size();
    world buffer(worlds[0]);
    for(int x=0; x<buffer.getSizeX(); ++x){
        for(int y=0; y<buffer.getSizeY(); ++y){
            buffer.setValue(x,y,0);
        }
    }
    for(int i=1; i<worlds.size(); ++i){
        for(int x=0; x<buffer.getSizeX(); ++x){
            for(int y=0; y<buffer.getSizeY(); ++y){
                if(worlds[i].getValue(x,y) == 0) buffer.setValue(x,y,i);
            }
        }
    }
    std::stringstream str;
    str << "<html><body>" << std::endl;
    for(int x=0; x<buffer.getSizeX(); ++x){
        for(int y=0; y<buffer.getSizeY(); ++y){

            str << colors[buffer.getValue(x,y)%25].toStdString() << 0 << "</font>";
        }
        str << "<br>" << std::endl;
    }
    str << "</body></html>" << std::endl;

    return str.str().c_str();
}
