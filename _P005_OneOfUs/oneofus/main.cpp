#include <iostream>
#include "fieldd.h"
#include "fieldtocnf.h"
#include <fstream>
#include <boost/timer.hpp>
#include <boost/date_time.hpp>
#include <sstream>
#define USER "user"
#define PASSWORD "password"

int main(int argc, char *argv[])
{
    std::ofstream statistics;
    statistics.open("/home/fettpet/Dropbox/Programmieren/_P005_OneOfUs/stats.html");
    statistics << "<html><body ><table border=\"1\">" << std::endl;
    statistics << "<tr><td>Level</td><td>Size</td><td>Colors</td><td>Forms</td><td>time</td></tr>" << std::endl;


    for(int i=0; i<200; ++i){
        std::stringstream str;
        str << "curl --data \"name=" << USER << "&password=" << PASSWORD << "\" -o /tmp/ht.html http://www.hacker.org/oneofus/index.php";
        std::system(str.str().c_str());
        std::fstream inputfile;
        inputfile.open("/tmp/ht.html", std::ios::in);
        std::string line;
        while(!inputfile.eof()){
            std::getline(inputfile, line);
            if(line.find("param name=\"FlashVars\"") != std::string::npos){
                line = line.substr(line.find("param name=\"FlashVars\""));
                line = line.substr(line.find("value")+7);
                line = line.substr(0, line.find("\""));
                break;
            }
        }
        inputfile.close();
        Field t(line);
        FieldToCnf fi;
        fi.doIt(t);
        std::fstream data;
        data.open("/tmp/dimacs.txt", std::ios::out);
        data << "p cnf " << fi.getVarCount() << " " << fi.getClauselCount() << std::endl;
        data << fi.getResult().str();
        data.close();
        boost::posix_time::ptime start, end;
        start = boost::posix_time::microsec_clock::local_time();
        std::system("/home/fettpet/Dokumente/Riss/Riss/riss /tmp/dimacs.txt /tmp/solution");
        end = boost::posix_time::microsec_clock::local_time();
        boost::posix_time::time_duration diff;
        diff = end - start;
        statistics << "<tr><td>" << i << "</td><td>(" << t.getSizeX() << ", " <<t.getSizeY() << ")</td>";
        statistics << "<td>" << t.getColorCount() << "</td><td>" << t.getFormCount() << "</td><td>" << diff.total_milliseconds() << "</td></tr>" << std::endl;
        std::stringstream sol;
        sol << "curl --data \"name=" << USER << "&password=" << PASSWORD << "&path=" <<  fi.getSolutionString("/tmp/solution") << "\""
            << " -o /tmp/error.html http://www.hacker.org/oneofus/index.php";
        std::cout << "SolutionString: " << sol.str().c_str() << std::endl;
        std::system(sol.str().c_str());
    }
    statistics.close();

    return 0;
}
