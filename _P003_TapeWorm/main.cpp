#include <iostream>
#include <World/world.h>
#include <fstream>
#include <Praprozessor/recolor.h>
#include <Praprozessor/easywormstohome.h>
#include <Heuristics/astarheuristics.h>
#include <sstream>
#include <Calculation/astarcalculation.h>
#include <boost/timer.hpp>
#include <Calculation/depthfirst.h>
#include <Heuristics/easywormheuristics.h>
#include <Calculation/PDDL/pddlsolution.h>
#include <Calculation/dfs_astar.h>
#include <string>
#define USER "user"
#define PASSWORD "password"
#define DEBUG 0
#define DEPTH
//#define PDDL
#ifdef PDDL
int main(){
    world w;
    w.ReadFromHtmlData("/home/fettpet/Dropbox/Programmieren/_P004_TapeWorm/Level/3.html");

    std::cout << w <<std::endl;

    pddlSolution sol(w);
    sol.createDomainFile("/tmp/domain.txt");
    sol.createProblemFile("/tmp/problem.txt");
    //    std::fstream data;
    //    data.open("/home/fettpet/tapewormStatistik.html", std::ios::app | std::ios::out);
    /*
        data << "<html><body><table>" << std::endl;
        data << "<tr><th>Level</th><th>Groesse</th><th>Wuermer</th><th>Wurmteile</th><th>Zeit</th>";
        data << "</tr>";
        */
}

#endif

/**************************************************************************************************
 *                                      Normal
 * ***********************************************************************************************/



#if DEBUG == 0
int main()
{

    std::fstream data;
    data.open("/home/fettpet/tapewormStatistik.html", std::ios::app | std::ios::out);

    data << "<html><body><table border = \"2\">" << std::endl;
    data << "<tr><th>Level</th><th>Groesse</th><th>Wuermer</th><th>Wurmteile</th><th>Zeit</th><th>visited Nodes</th>";
    data << "<th>start depth</th> <th> solution depth</th> <th>Not expand</th>";
    data << "</tr>";


    for(int i=0; i<1200; ++i){
        std::stringstream str;
        str << "curl --data \"name=" << USER << "&password=" << PASSWORD << "\" -o /tmp/source.html http://www.hacker.org/tapeworm/index.php";
       std::system(str.str().c_str());
       WorldDeep w;

       w.ReadFromHtmlData("/tmp/source.html");
       w.createBlockedMap();
     //  w.ReadFromHtmlData("/home/fettpet/Dropbox/Programmieren/_P004_TapeWorm/Level/3.html");
       std::stringstream copyStream;
       copyStream << "cp /tmp/source.html /home/fettpet/Dropbox/Programmieren/_P004_TapeWorm/Level/" << w.getWormMap().getLevel() << ".html";
       std::system(copyStream.str().c_str());
       if(w.getWormMap().getLevel() == 1286) break;

       std::stringstream sol;
       sol << "lvl=" << w.getWormMap().getLevel() << "&sol=";
       boost::timer time;
       uint WormCount(w.getWormCount());
       Recolor::getValidWormsNewColors(w);
       uint countOpen(0), countClose(0);
       sol << EasyWormsToHome::getEasyWormsToHome(w, countOpen, countClose);
       //std::cout << w;
       Heuristics::AStarHeuristics h(w);
       h.createDistanceGraph();

       std::cout << "Start with Deep: " << h(&w) << std::endl;
       DFS_AStar calc;

       sol <<  calc.calc(w, h);

       std::cout << sol.str() << std::endl;
       data << "<tr><th>" << w.getWormMap().getLevel() << "</th><th>(" << w.getSizeX() << ", " << w.getSizeY() << ")</th><th>";
       data << WormCount << "</th><th>";
       uint wurmPartCount(0);
       for(uint i=0; i<w.getWormCount(); ++i){
           wurmPartCount += w.getWorm(i).getLength();
       }
       data << wurmPartCount << "</th><th>" << time.elapsed() << "</th><th>" << calc.getCreatedStates() << "</th><th>" << calc.getStartDepth()<< "</th>";
       data << "<th>" << calc.getMaxDepth() << "</th><th>" << calc.getNotExpand() << "</th></tr>" << std::endl;
       std::stringstream outstr;
       outstr << "curl --data \"name=" << USER << "&password=" << PASSWORD <<"&" <<  sol.str() << "\""
              << " -o /tmp/error.html http://www.hacker.org/tapeworm/index.php";
       std::cout << outstr.str();
       std::system(outstr.str().c_str());

    }
    //*/
    data.close();
    return 0;
}



#endif
#endif

