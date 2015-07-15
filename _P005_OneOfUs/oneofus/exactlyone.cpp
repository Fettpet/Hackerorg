#include "exactlyone.h"

/**************************************************************************************************
 *                                      Exactly One trival
 * ***********************************************************************************************/
int ExactlyOne::getExactlyOne(std::set<int>& vars, std::stringstream& result ){
    std::stringstream clause;
    int countClausel(0);
// pairwaise to (-x1 or -x2)
    for( std::set< int >::const_iterator it1 = vars.begin() ; it1 != vars.end() ; ++it1 )
    {

        clause << *it1 << " ";
        bool found(false);
        for( std::set< int >::const_iterator it2 = it1 ; it2 != vars.end() ; ++it2 )
        {
            if(!found) {
                found = true;
                continue;
            }
            countClausel++;
            result << "-" << *it1 << " -" << *it2 << " 0" << std::endl;
        }

    }


    if( !clause.str().empty()){
        countClausel++;
        result << clause.str() << "0" << std::endl;
    }

    return countClausel;
}
