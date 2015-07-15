#include "fieldtocnf.h"
#include <fstream>
#include "exactlyone.h"
FieldToCnf::FieldToCnf():
     countVar(0), countClausel(0)
{
}

void FieldToCnf::doIt(const Field& f){
    countClausel =0;
    countVar = 0;
    result.str("");
    result.clear();
    calcTransforms(f);
    calcExactlyOneForAllValues(f);
    calcExactlyOneForAllCells(f);
    for(int x=0; x<f.getSizeX(); ++x)
        for(int y=0; y<f.getSizeY(); ++y){
            calcNextPosForCell(x,y, f);
        }
}

/**************************************************************************************************
 *                                  getCNF
 * ***********************************************************************************************/


void FieldToCnf::calcExactlyOneForAllCells(const Field& f){
    for(int x=0; x<f.getSizeX(); ++x){
        for(int y=0; y<f.getSizeY(); ++y){
            std::set<int> ea;
            for(int v=1; v<=f.getSizeX()*f.getSizeY(); ++v){
                ea.insert(PosToInt[Pos(x,y,v)]);
            }
             countClausel += ExactlyOne::getExactlyOne(ea, result);
        }
    }
}

void FieldToCnf::calcExactlyOneForAllValues(const Field& f){
    for(int v=1; v<=f.getSizeX()*f.getSizeY(); ++v){
        std::set<int> ea;
        for(int x=0; x<f.getSizeX(); ++x){
            for(int y=0; y<f.getSizeY(); ++y){
                    ea.insert(PosToInt[Pos(x,y,v)]);
            }
        }
        countClausel += ExactlyOne::getExactlyOne(ea, result);
    }
}

void FieldToCnf::calcNextPosForCell(short x, short y, const Field& f){
    for(int v=2; v<=f.getSizeX()*f.getSizeY(); ++v){
// Jede Zeile
        result << "-" << PosToInt[Pos(x,y, v-1)] << " ";
        for(int xf=0; xf<f.getSizeX(); ++xf){
            if(x == xf) continue;
            if(f.getColor(x,y) == f.getColor(xf, y)){
                result << PosToInt[Pos(xf,y,v)] << " ";
            } else if(f.getForm(x,y) == f.getForm(xf, y)){
                result << PosToInt[Pos(xf,y,v)] << " ";
            }// else {
             //   result << "-" << PosToInt[Pos(xf,y,v)] < " ";
            //}
        }
// Spalte
        for(int yf=0; yf<f.getSizeX(); ++yf){
            if(y == yf) continue;
            if(f.getColor(x,y) == f.getColor(x, yf)){
                result << PosToInt[Pos(x,yf,v)] << " ";
            } else if(f.getForm(x,y) == f.getForm(x, yf)){
                result << PosToInt[Pos(x,yf,v)] << " ";
            }// else {
             //   result << "-" << PosToInt[Pos(x,yf,v)] < " ";
            //}
        }
        result << "0" << std::endl;
        ++countClausel;
    }
}

/**************************************************************************************************
 *                                  Transformer
 * ***********************************************************************************************/
void FieldToCnf::calcTransforms(const Field& f){
    PosToInt.clear();
    IntToPos.clear();
    IntToPos.push_back(Pos(0,0,0));
    for(int x=0; x<f.getSizeX(); ++x){
        for(int y=0; y<f.getSizeY(); ++y){
            for(int v=1; v<=f.getSizeX()*f.getSizeY(); ++v){
                PosToInt.insert( std::pair<Pos, int>(Pos(x, y, v), IntToPos.size()));
                IntToPos.push_back(Pos(x,y,v));
            }
        }
    }
    countVar = PosToInt.size();
}

/**************************************************************************************************
                                        Getter
**************************************************************************************************/

std::stringstream& FieldToCnf::getResult(){
    return result;
}

int FieldToCnf::getVarCount(){
    return countVar;
}

int FieldToCnf::getClauselCount(){
    return countClausel;
}

std::vector<std::string> split( std::string &s, char delim) {
    std::vector<std::string> elems;

    std::stringstream ss(s);
    std::string item;
    while ( std::getline(ss, item, delim) ) {
        elems.push_back(item);
    }
    return elems;
}

std::string FieldToCnf::getSolutionString(std::string fileName){
    std::fstream fileIn;
    fileIn.open(fileName, std::ios::in);
    std::vector<Pos> out;
    if (fileIn.is_open())
    {
        std::string line;
        std::getline( fileIn, line );

        if( line == "UNSAT" )
        {
            std::cout << line << std::endl;
        }
        else if( line.find("SAT") != std::string::npos )
        {

            std::getline( fileIn, line );
            std::cout << line << std::endl;            std::vector<std::string> solution = split( line, ' ');
            for( std::string s : solution )
            {
                if(s == "v") continue;
                if( s == "0")
                {
                    break;
                }

                // skip negativ vars
                if( s[0] == '-' ) continue;

                int varAsInt = stoi( s );
                std::cout << varAsInt << std::endl;
                Pos varAsString = IntToPos[varAsInt];
                out.push_back(varAsString);
            }
        }

        fileIn.close();
        std::stringstream str;
        for(int i=1; i<=out.size(); ++i){
            for(Pos p : out){
                if(p.getValue() == i){
                    str << p.getX() << "," << p.getY() << "_";
                    break;
                }
            }
        }
        return str.str().substr(0, str.str().size()-1);
    }

    return "";
}


