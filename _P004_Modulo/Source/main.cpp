//  ./ff -f ./problem.txt -o ./domain.txt


#include <iostream>
#include <sstream>
#include <fstream>
#include "Modulo/field.h"
#include "Modulo/game.h"
#include "Solver/Sat/mod2tosat.h"
#include "Solver/Sat/mod4tosat.h"
#include "Solver/Sat/mod3tosat.h"
#include "Solver/Sat/tosathalf.h"
#include "Solver/Sat/tosat.h"
#include "Solver/Sat/tosatdebug.h"
#include "Solver/solver.h"
#include "IO/IO.h"
#include "Complextest/statistic.h"
#include "Solver/Planning/pddladl.h"
#include "Solver/Planning/pddladlconst.h"
#include "Solver/Planning/pddlstrips.h"
#include "Solver/Planning/pddladlnumerics.h"
#include "Solver/Planning/pddlstripsconst.h"
#include "Solver/tabusearch.h"
#include "Solver/deepsearch.h"
#include <boost/timer.hpp>
#include <omp.h>
#include <cstdlib>
#define USER "user"
#define PASSWORD "password"


int main(){
    std::fstream data;
    IO::gotoLevel(USER, PASSWORD, 0);
    data.open("/home/fettpet/ausgDeepSearch.html", std::ios::out);
    data << "<html><body><table border=\"1\">" << std::endl;
    data << "<tr><th>Level</th><th>Size</th><th>Tiles</th><th>Modulo</th><th>Time</th></tr>" << std::endl;
    for(uint lev = 0; lev < 62; ++lev){
        bool solved(false);
        boost::timer timer;
        Solver::TabuSearch::setNewGame();
        Game::Game gameField(IO::readGameFromHackerOrg(USER, PASSWORD));
        std::cout << gameField << std::endl;
        Solver::DeepSearch deep(gameField);
        Game::Game solution = deep.getSolution();

        data << "<tr><th>" << lev << "</th><th>(" << gameField.getField().getSizeX() << ", " << gameField.getField().getSizeY() << ")</th><th>" << gameField.getTileCount()
             << "</th><th>" << (int)solution.getField().getMod() << "</th><th>" << timer.elapsed() << "</th></tr>" << std::endl;
        IO::writeSolutionToHacker(USER, PASSWORD, solution.getHackerOrgSolution());
    }
}

/*
int main(){
    std::fstream data;
    IO::gotoLevel(USER, PASSWORD, 0);
    data.open("/home/fettpet/ausgTabuSearch.html", std::ios::out);
    data << "<html><body><table border=\"1\">" << std::endl;
    data << "<tr><th>Level</th><th>Size</th><th>Tiles</th><th>Modulo</th><th>Time</th></tr>" << std::endl;
    for(uint lev = 0; lev < 62; ++lev){
        bool solved(false);
        boost::timer timer;
        Solver::TabuSearch::setNewGame();
        Game::Game gameField(IO::readGameFromHackerOrg(USER, PASSWORD));
#pragma omp parallel for
        for(int core=0; core<8; ++core){
            Solver::TabuSearch tabu(gameField, rand());
//            std::cout << gameField << std::endl;
//            std::cout << "Heuristic: " << tabu.calcHeuristic() << std::endl;
//            tabu.printCurrentValues();
            Game::Game solution = tabu.getSolution();
            if(!solved && solution.getField().getSizeX() > 0){
                solved = true;
                data << "<tr><th>" << lev << "</th><th>(" << gameField.getField().getSizeX() << ", " << gameField.getField().getSizeY() << ")</th><th>" << gameField.getTileCount()
                     << "</th><th>" << (int)solution.getField().getMod() << "</th><th>" << timer.elapsed() << "</th></tr>" << std::endl;
                IO::writeSolutionToHacker(USER, PASSWORD, solution.getHackerOrgSolution());
                Solver::TabuSearch::setFinished();
            }
        }

   }

}


/*
void help(){
    std::cout << "Usage: modulo [option] -i inputfile" <<std::endl;
    std::cout << "-i\t inputfile \t file of input modulogame" << std::endl;
    std::cout << "-o\t outputfile \t file where the result is written" << std::endl;
    std::cout << "-t\t encodedfile \t file where the encoding is stored" << std::endl;
    std::cout << "-s\t solver \t define the solver" << std::endl;
    std::cout << "-c\t         \t convert the problem without solving" << std::endl;
    std::cout << "-m\t methode \t define the encoding possiblities:" << std::endl;
    std::cout << "\t -m sss \t SAT encoding with sorting networks" << std::endl;
    std::cout << "\t -m sskv \t SAT encoding with karnaugh maps" << std::endl;
    std::cout << "\t -m psug \t STRIPS encoding ungrounded" << std::endl;
    std::cout << "\t -m psg \t STRIPS encoding grounded" << std::endl;
}


int main(int argc, char** argv){
    std::vector< std::string > arguments;
    std::cout << "Argc: " << argc << std::endl;
    for(int i=0; i<argc; ++i){
        arguments.push_back(argv[i]);
        std::cout << arguments[i] << std::endl;
    }
    std::string inputfile, outputfile, tmpfile;
    std::string solver, methode;
    bool convertOnly(false);
    for(int i=1; i<argc; ++i){
// Images Input
        if(arguments[i].find("--help") != std::string::npos){
            help();
            exit(0);
        }
        if(arguments[i].find("-c") != std::string::npos){
            convertOnly = true;
        }
        if(arguments[i].find("-i") != std::string::npos){
            if(!inputfile.empty()){
                std::cout << "Multiple times defined inputfile" << std::endl;
                exit(1);
            }
            inputfile = arguments[i+1];
            i+=1;
            continue;
        }
        if(arguments[i].find("-o") != std::string::npos){
            if(!outputfile.empty()){
                std::cout << "Multiple times defined outputfile" << std::endl;
                exit(1);
            }
            outputfile= arguments[i+1];
            i+=1;
            continue;
        }
        if(arguments[i].find("-t") != std::string::npos){
            if(!tmpfile.empty()){
                std::cout << "Multiple times defined encoding file" << std::endl;
                exit(1);
            }
            tmpfile= arguments[i+1];
            i+=1;
            continue;
        }
        if(arguments[i].find("-s") != std::string::npos){
            if(!solver.empty()){
                std::cout << "Multiple times defined solver" << std::endl;
                exit(1);
            }
            solver= arguments[i+1];
            i+=1;
            continue;
        }
        if(arguments[i].find("-m") != std::string::npos){
            if(!methode.empty()){
                std::cout << "Multiple times defined methode" << std::endl;
                exit(1);
            }
            methode= arguments[i+1];
            i+=1;
            continue;
        }
    }
    if(inputfile.empty()){
        std::cout << "No inputfile specified" << std::endl;
        help();
        exit(1);
    }
    if(outputfile.empty()){
        outputfile = inputfile;
    }
    if(tmpfile.empty() && !convertOnly){
        tmpfile="/tmp/tmpfileSolver.txt";
    } else if (tmpfile.empty() && convertOnly){
        std::cout << "No encoding file specified";
        exit(1);
    }
    if(methode.empty()){
        methode = "sss";
    }
    if(solver.empty()){
        if((methode == "sskv") || (methode == "sss")){
            solver = "./Solver/riss";
        }else{
            solver = "./Solver/M";
        }

    }

    Game::Game gameField(IO::readGameFromFile(inputfile));
    if(methode == "sss"){
        Solver::tosat sol(gameField, solver);
        sol.createDimacs(tmpfile);
        if(convertOnly) exit(0);
        Game::Game solved = sol.getSolution();
        IO::writeGameToFile(solved, outputfile);
    } else if(methode == "sskv"){
        if(gameField.getField().getMod() == 2){
             Solver::Mod2ToSat sol(gameField, solver);
             sol.createDimacs(tmpfile);
             if(convertOnly) exit(0);
             Game::Game solved = sol.getSolution();
             IO::writeGameToFile(solved, outputfile);
        } else  if(gameField.getField().getMod() == 3){
            Solver::Mod3ToSat sol(gameField, solver);
            sol.createDimacs(tmpfile);
            if(convertOnly) exit(0);
            Game::Game solved = sol.getSolution();
            IO::writeGameToFile(solved, outputfile);
        } else if(gameField.getField().getMod() == 4){
            Solver::Mod4ToSat sol(gameField, solver);
            sol.createDimacs(tmpfile);
            if(convertOnly) exit(0);
            Game::Game solved = sol.getSolution();
            IO::writeGameToFile(solved, outputfile);
        } else {
            std::cout << "KV Encoding supports only modulo value 2, 3, 4. Sorry" << std::endl;
            exit(1);
        }
    } else if( methode == "psug"){
        Solver::PddlAdlNumerics sol(gameField, solver);
        std::stringstream domain, problem;
        domain << tmpfile << ".domain";
        problem << tmpfile << ".problem";
        sol.createDomainFile(domain.str());
        sol.createProblemFile(problem.str());
        if(convertOnly) exit(0);
        Game::Game solved = sol.getSolution();
        IO::writeGameToFile(solved, outputfile);
    } else if(methode == "psg"){
        Solver::PddlStrips sol(gameField, solver);
        std::stringstream domain, problem;
        domain << tmpfile << ".domain";
        problem << tmpfile << ".problem";
        sol.createDomainFile(domain.str());
        sol.createProblemFile(problem.str());
        if(convertOnly) exit(0);
        Game::Game solved = sol.getSolution();
        IO::writeGameToFile(solved, outputfile);
    } else {
        std::cout << "methode is not supportet";
        help();
    }


}
/*
//#define BOUNDLE
#define RELEASE
//#define PDDL
#define DIRECT

#ifndef BOUNDLE

#ifndef RELEASE
int main(){
    uint lev = 0;
    std::stringstream str;
    str << "/home/fettpet/Dropbox/Programmieren/_P018_Modulo/Level/Level_" << lev << ".txt";

    Game::Game gameField(IO::readGameFromFile(str.str()));

      //  std::cout << gameField;
    Game::Game solved;
    Solver::PddlAdlNumerics sol(gameField, "/home/fettpet/Downloads/seq-sat/seq-sat-bfs-f/at_bfs_f");
    solved = sol.getSolution();
    std::cout << "LEVEL: " << lev << std::endl;
    std::cout << solved << std::endl;


    IO::writeGameToFile(solved, str.str());

    return 0;
}
#endif


#ifdef RELEASE
#ifdef DIRECT
int main()
{
    uint timeDimacs, timeSolution;
    uint countVar, countClausel, countVarBegin;
    std::fstream data;

    data.open("/home/fettpet/ausgDirectencoding.html", std::ios::out);
    data << "<html><body><table border=\"1\">" << std::endl;
    data << "<tr><th>Level</th><th>Size</th><th>Tiles</th><th>Modulo</th><th>Time Dimacs</th><th>Time Solution</th>"
         << "<th>Vars</th><th>Vars tseitin</th><th>Clausel</th></tr>" << std::endl;
    data.close();

    //uint lev = 12;
    for(int lev=0; lev<1; ++lev){
        std::stringstream str;
        str << "/home/fettpet/Dropbox/Programmieren/_P018_Modulo/Level/Level_" << lev << ".txt";

        Game::Game gameField(IO::readGameFromFile("/home/fettpet/Dokumente/Modulo/Level/Level_30_1.txt"));
        std::cout << gameField << std::endl;

        IO::writeGameToFile(gameField, str.str());
      //  std::cout << gameField;
        Game::Game solved;
        if(gameField.getField().getMod() == 2){
             Solver::Mod2ToSat sol(gameField, "/home/fettpet/Dokumente/Riss/Riss/riss");
             solved = sol.getSolution();
             timeDimacs = sol.getNeededTimeForCreateDimacs();
             timeSolution = sol.getNeededTimeForSatSolving();
             countVar = sol.getCountVars();
             countVarBegin = sol.getCountVarsBegin();
             countClausel = sol.getCountClausel();
        } else  if(gameField.getField().getMod() == 3){
            Solver::Mod3ToSat sol(gameField, "/home/fettpet/Dokumente/Riss/Riss/riss");
            solved = sol.getSolution();
            timeDimacs = sol.getNeededTimeForCreateDimacs();
            timeSolution = sol.getNeededTimeForSatSolving();
            countVar = sol.getCountVars();
            countVarBegin = sol.getCountVarsBegin();
            countClausel = sol.getCountClausel();
        } else if(gameField.getField().getMod() == 4){
            Solver::Mod4ToSat sol(gameField, "/home/fettpet/Dokumente/Riss/Riss/riss");
            solved = sol.getSolution();
            timeDimacs = sol.getNeededTimeForCreateDimacs();
            timeSolution = sol.getNeededTimeForSatSolving();
            countVar = sol.getCountVars();
            countVarBegin = sol.getCountVarsBegin();
            countClausel = sol.getCountClausel();
        }
        data.open("/home/fettpet/ausgDirectencoding.html", std::ios::out | std::ios::app);
        data << "<tr><th>" << lev << "</th><th>("
             << gameField.getField().getSizeX() << ", " << gameField.getField().getSizeY() << ") </th>"
             << "<th>" << gameField.getTileCount() << "</th>"
             << "<th>" << (int)gameField.getField().getMod() << "</th>"
             << "<th>" << timeDimacs << "</th><th>";
        if(timeSolution > 60000){
            data << (timeSolution % (60 * 60000)) / 60000 << "min ";
        }
        if(timeSolution > 1000){
              data << (timeSolution % 60000) / 1000 << "s ";
        }
        data << timeSolution % 1000 << "ms</th>"
             << "<th>" << countVarBegin << "</th>"
             << "<th>" << countVar   << "</th>"
             << "<th>" << countClausel << "</th></tr>" << std::endl;
        data.close();

        IO::writeGameToFile(solved, str.str());
    }

    data.open("/home/fettpet/ausg.html", std::ios::out | std::ios::app);
    data <<"</table></body></html>" << std::endl;
    data.close();
    return 0;
}
#endif // DIRECT

#ifndef DIRECT
#ifndef PDDL
int main()
{

    uint timeDimacs, timeSolution;
    uint countVar, countClausel, countVarBegin;
    std::fstream data;

    data.open("/home/fettpet/ausgSortRiss.html", std::ios::out);
    data << "<html><body><table border=\"1\">" << std::endl;
    data << "<tr><th>Level</th><th>Size</th><th>Tiles</th><th>Modulo</th><th>Time Dimacs</th><th>Time Solution</th>"
         << "<th>Vars</th><th>Vars tseitin</th><th>Clausel</th></tr>" << std::endl;
    data.close();

    //uint lev = 12;
    for(int lev=0; lev<62; ++lev){
        std::stringstream str;
        str << "/home/fettpet/Dropbox/Programmieren/_P018_Modulo/Level/Level_" << lev << ".txt";

     //   Game::Game gameField(IO::readGameFromFile("/home/fettpet/Dropbox/Programmieren/_P018_Modulo/Level/Level_0.txt"));


        IO::writeGameToFile(gameField, str.str());
      //  std::cout << gameField;
        Game::Game solved;
        Solver::tosat sol(gameField, "/home/fettpet/Dropbox/Programmieren/_P018_Modulo/Solver/riss3g-src/binary/riss");
        solved = sol.getSolution();
        complextest::statistic stat;
   //     stat.fieldAnalysis(gameField);
        timeDimacs = sol.getNeededTimeForCreateDimacs();
        timeSolution = sol.getNeededTimeForSatSolving();
        countVar = sol.getCountVars();
        countVarBegin = sol.getCountVarsBegin();
        countClausel = sol.getCountClausel();

        data.open("/home/fettpet/ausgSortRiss.html", std::ios::out | std::ios::app);
        data << "<tr><th>" << lev << "</th><th>("
             << gameField.getField().getSizeX() << ", " << gameField.getField().getSizeY() << ") </th>"
             << "<th>" << gameField.getTileCount() << "</th>"
             << "<th>" << (int)gameField.getField().getMod() << "</th>"
             << "<th>" << timeDimacs << "</th><th>";
        if(timeSolution > 60000){
            data << (timeSolution % (60 * 60000)) / 60000 << "min ";
        }
        if(timeSolution > 1000){
              data << (timeSolution % 60000) / 1000 << "s ";
        }
        data << timeSolution % 1000 << "ms</th>"
             << "<th>" << countVarBegin << "</th>"
             << "<th>" << countVar   << "</th>"
             << "<th>" << countClausel << "</th></tr>" << std::endl;
        data.close();


       // solved.executeTest();
        std::cout << solved.getField() << std::endl;
    }

    data.open("/home/fettpet/ausg.html", std::ios::out | std::ios::app);
    data <<"</table></body></html>" << std::endl;
    data.close();
    return 0;
}


#endif // pddl
#endif // NOT DIRECT

#ifdef PDDL

int main(){
    std::fstream data;
    data.open("/home/fettpet/ausgADLComplete.html", std::ios::out);
    data << "<html><body><table border=\"1\">" << std::endl;
    data << "<tr><th>Level</th><th>Size</th><th>Tiles</th><th>Modulo</th><th>Time Problem</th><th>Time Domain</th><th>Time Solution</th></tr>" <<std::endl;
    data.close();

    //uint lev=1;
    for(uint lev=0; lev<20;++lev){
        std::stringstream str;
        str << "/home/fettpet/Dropbox/Programmieren/_P018_Modulo/Level/Level_" << lev << ".txt";

        //Game::Game gameField = IO::readGameFromFile(str.str());
        IO::writeGameToFile(gameField, str.str());

          //  std::cout << gameField;
        Game::Game solved;
        Solver::PddlAdlNumerics sol(gameField,"/home/fettpet/Dropbox/Programmieren/_P018_Modulo/Solver/M");
        solved = sol.getSolution();
        std::cout << solved << std::endl;
        std::cout << solved.getHackerOrgSolution() << std::endl;
        IO::writeGameToFile(solved, str.str());


        data.open("/home/fettpet/ausgADLComplete.html", std::ios::out | std::ios::app);
        data << "<tr><th>" << lev << "</th><th>("
             << gameField.getField().getSizeX() << ", " << gameField.getField().getSizeY() << ") </th>"
             << "<th>" << gameField.getTileCount() << "</th>"
             << "<th>" << (int)gameField.getField().getMod() << "</th>"
             << "<th>" << sol.getNeededTimeProblemCreate() << "</th>"
             << "<th>" << sol.getNeededTimeDomainCreate() << "</th><th>";
        uint timeSolution = sol.getNeededTimeSolverCreate();
        if(timeSolution > 60000){
            data << (timeSolution % (60 * 60000)) / 60000 << "min ";
        }
        if(timeSolution > 1000){
              data << (timeSolution % 60000) / 1000 << "s ";
        }
        data << timeSolution % 1000 << "ms</th></tr>" << std::endl;
        data.close();
    }
    return 0;
}

#endif // pddl
#endif //REALEASE
#endif // BOUNDLE
#define COUNT 10
#define MAXLEVEL 38
#define COUNTDIFFLEVEL 100
#define DIFFLEVEL 32
#define FOLDER "/home/fettpet/Dokumente/Modulo/"
#ifdef BOUNDLE
/*
int main(){

    for(int level=0; level<=3; ++level)
        for(int count =10; count <30; ++count){
            std::stringstream fileSatDirect, fileSatHalfSorter, fileSatSorter, fileStripsProblem, fileStripsDomain, fileStripsConstDomain, fileStripsConstProblem, fileLevel;
            std::stringstream fileADLConstDomain, fileADLConstProblem, fileADLDomain, fileADLProblem;
// Download Level


            fileLevel << FOLDER << "NeueLevel/Level_" << level << "_" << count << ".txt";
            //Game::Game gameField(IO::readGameFromFile(fileLevel.str()));
            IO::writeGameToFile(gameField, fileLevel.str());

            fileStripsConstProblem << FOLDER << "PDDLNum/PDDLNumProblem_"<< level << "_" << count << ".txt";
            fileStripsConstDomain << FOLDER << "PDDLNum/PDDLNumDomain_"<< level << "_" << count << ".txt";
            Solver::PddlAdlNumerics solStripsConst(gameField, "");
            solStripsConst.createDomainFile(fileStripsConstDomain.str());
            solStripsConst.createProblemFile(fileStripsConstProblem.str());
// Level to Sorter
            Solver::tosat solSatSorter(gameField, "");
            fileSatSorter << FOLDER << "NeueSat/DimacsSorter_" << level << "_" << count << ".dimacs";
            solSatSorter.createDimacs(fileSatSorter.str());
// Level to Sat Direct
            fileSatDirect << FOLDER << "NeueSat/DimacsDirect_" << level << "_" << count << ".dimacs";
            if(gameField.getField().getMod() == 2){
                Solver::Mod2ToSat solMod2(gameField, "/home/fettpet/Downloads/binary/plingeling");
                solMod2.createDimacs(fileSatDirect.str());
            } else  if(gameField.getField().getMod() == 3){
                Solver::Mod3ToSat solMod3(gameField, "/home/fettpet/Downloads/binary/plingeling");
                solMod3.createDimacs(fileSatDirect.str());
            } else if(gameField.getField().getMod() == 4){
                Solver::Mod4ToSat solMod3(gameField, "/home/fettpet/Downloads/binary/plingeling");
                solMod3.createDimacs(fileSatDirect.str());
            }
        }
/*
// Level to Sorter

// Level to Half Sorter
            Solver::tosat solSatSorterHalf(gameField, "");
            fileSatHalfSorter << FOLDER << "SatSorterHalf/DimacsSorterHalf_" << level << "_" << count << ".dimacs";
            solSatSorterHalf.createDimacs(fileSatHalfSorter.str());

// Level to Strips Const
            fileStripsConstProblem << FOLDER << "PDDLStripsConst/PddlStripsConstProblem_"<< level << "_" << count << ".txt";
            fileStripsConstDomain << FOLDER << "PDDLStripsConst/PddlStripsConstDomain_"<< level << "_" << count << ".txt";
            Solver::PddlStripsConst solStripsConst(gameField, "");
            solStripsConst.createDomainFile(fileStripsConstDomain.str());
            solStripsConst.createProblemFile(fileStripsConstProblem.str());
// Level to Strips
            fileStripsProblem << FOLDER << "PDDLStrips/PddlStripsProblem_"<< level << "_" << count << ".txt";
            fileStripsDomain << FOLDER << "PDDLStrips/PddlStripsDomain_"<< level << "_" << count << ".txt";
            Solver::PddlStrips solStrips(gameField, "");
            solStrips.createDomainFile(fileStripsDomain.str());
            solStrips.createProblemFile(fileStripsProblem.str());
// Level to ADL Const
            fileADLConstProblem << FOLDER << "PDDLADLConst/PddlADLConstProblem_"<< level << "_" << count << ".txt";
            fileADLConstDomain << FOLDER << "PDDLADLConst/PddlADLConstDomain_"<< level << "_" << count << ".txt";
            Solver::PddlADLConst solADLConst(gameField, "");
            solADLConst.createDomainFile(fileADLConstDomain.str());
            solADLConst.createProblemFile(fileADLConstProblem.str());
// Level to ADL
            fileADLProblem << FOLDER << "PDDLADL/PddlADLProblem_"<< level << "_" << count << ".txt";
            fileADLDomain << FOLDER << "PDDLADL/PddlADLDomain_"<< level << "_" << count << ".txt";
            Solver::PddlStrips solADL(gameField, "");
            solADL.createDomainFile(fileADLDomain.str());
            solADL.createProblemFile(fileADLProblem.str());
        }

    for(int count =0; count <COUNTDIFFLEVEL; ++count){
        std::stringstream fileSat, fileTest;

        Solver::tosat sol(gameField, "");
        fileTest << FOLDER << "DiffLevel/LevelDiff_" << DIFFLEVEL << "_" << count << ".txt";
        IO::writeGameToFile(gameField, fileTest.str());
        fileSat << FOLDER << "DiffSat/DimacsSorterDiff_" << DIFFLEVEL << "_" << count << ".dimacs";
        sol.createDimacs(fileSat.str());
    }
    for(int count =0; count <COUNTDIFFLEVEL; ++count){
        std::stringstream fileSat, fileTest;

        Solver::tosat sol(gameField, "");
        fileTest << FOLDER << "DiffLevel/LevelDiff_" << 38 << "_" << count << ".txt";
        IO::writeGameToFile(gameField, fileTest.str());
        fileSat << FOLDER << "DiffSat/DimacsSorterDiff_" << 38 << "_" << count << ".dimacs";
        sol.createDimacs(fileSat.str());
    }

    return EXIT_SUCCESS;

}
//

int main(){
    std::fstream data;
    data.open("/home/fettpet/stat.txt", std::ios::out);
    data << "CountTiles TimePlanner timeSat" << std::endl;
    for(int level=0; level<=MAXLEVEL; ++level)
        for(int count =0; count <COUNT; ++count){
            std::stringstream fileSatDirect, fileSatHalfSorter, fileSatSorter, fileStripsProblem, fileStripsDomain, fileStripsConstDomain, fileStripsConstProblem, fileLevel;
            std::stringstream fileADLConstDomain, fileADLConstProblem, fileADLDomain, fileADLProblem;

            fileLevel << FOLDER << "Level/Level_" << level << "_" << count << ".txt";
            Game::Game gameField(IO::readGameFromFile(fileLevel.str()));

            Solver::PddlAdlNumerics solver(gameField, "");
            fileStripsDomain << FOLDER << "PDDLNum/Domain_" << level << "_" << count << ".txt";
            fileStripsProblem << FOLDER << "PDDLNum/Problem_" << level << "_" << count << ".txt";
            solver.createDomainFile(fileStripsDomain.str());
            solver.createProblemFile(fileStripsProblem.str());
            //solSatSorterHalf.getCountVarsBegin();
            fileSatDirect << "/projects/p_sat/CNF/WithoutSource/Modulo/SatSorterHalf/DimacsSorterHalf_" << level << "_" << count << ".cnf.gz";
            std::fstream input;
            input.open("/home/fettpet/Dokumente/Ergebnisse/riss5.csv", std::ios::in);
            double timeSat, timePlanner;
            while(!input.eof()){
                std::string buffer;
                std::getline(input, buffer);
                if(buffer.find(fileSatDirect.str()) != std::string::npos){
                    buffer = buffer.substr(buffer.find(" ")+1);
                    buffer = buffer.substr(buffer.find(" ")+1);
                    buffer = buffer.substr(buffer.find(" ")+1);
                    buffer = buffer.substr(0, buffer.find(" "));
                    timeSat = std::atof(buffer.c_str());
                }
            }
            input.close();

            input.open("/home/fettpet/Dokumente/Ergebnisse/Mad.csv", std::ios::in);
            fileStripsProblem << "/projects/p_sat/Planning/Modulo/PddlStripsConstDomain_" << level << "_" << count << ".txt";
            while(!input.eof()){
                std::string buffer;
                std::getline(input, buffer);
                if(buffer.find(fileStripsProblem.str()) != std::string::npos){
                 //   std::cout << buffer;
                    buffer = buffer.substr(buffer.find(" ")+1);
                    buffer = buffer.substr(buffer.find(" ")+1);
                    buffer = buffer.substr(buffer.find(" ")+1);
                    buffer = buffer.substr(0, buffer.find(" "));
                    timePlanner = std::atof(buffer.c_str());
                }
            }
            data << gameField.getTileCount() << " " << timePlanner << " " << timeSat << std::endl;
        }
}
//*/
//#endif
