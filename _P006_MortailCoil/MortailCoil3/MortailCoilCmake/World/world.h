#ifndef WORLD_H
#define WORLD_H
#include <string>
#include <vector>
#include <iostream>
//#include <QImage>
#include <fstream>
#include "region.h"
#include "../Solver/Data/pos.h"

typedef unsigned int uint;

namespace  World {

    class world
/**
    world contains the
          */
    {
    public:
        world();
/**
* @brief readFromFile: Read a world from a input file
* @param file the path to the file
* The file consists of:
* 1. The size of the graph (width and height)
* 2. The graph ( a matrix of size width time height)
*       An one is a blocked field. A two is a passage and a zero is a
*       field what need to be visited.
* 3. The number of the regions
* 4. The regions
* Each region start with the number of passages. The all passages are listen. A passage is a tuple of (x, y) of a coordinate.
* the next number is the number of field that belongs to the passage. then all vertex that belongs to the passage are listen.
*
*/
        virtual void readFromFile(const std::string& file);

        /**
         * @brief getRowCount before this function is called, readFromFile must be called
         * @return the number of rows that the graph has
         */
        uint getRowCount() const;

        /**
         * @brief getColumnCount before this function is called, readFromFile must be called
         * @return the number of columns that the graph has
         */
        uint getColumnCount() const;

        /**
          how many regions are in the world
         * @brief getNumberRegions
         * @return number of regions
         */
        uint getNumberRegions() const;

        /**
         * @brief getRegion
         * @param nr The number of region
         * @return a reference to a region
         */
        World::Region& getRegion(const uint nr);


        /**
         * @brief getRegion
         * @param nr The number of region
         * @return a constant reference to a region
         */
        const World::Region& getRegion(const uint nr) const;

        /**
         * @brief getRegions
         * @return All regions in a vector
         */
        const std::vector<World::Region>& getRegions() const;

        /**
         * @brief isBlocked test wheter a vertex is blocked or not
         * @param r The row in which the vertex is
         * @param c The column in which the vertex is
         * @return true if the vertex is blocked
         *         false if the vertex is unblocked
         */
        bool isBlocked(const uint col, const uint row) const;
        /**
         * @brief transform create a image of the whole world
         * @return
         */
//        QImage transform() const;

        /**
         * @brief getNumberDeadEnds
         * @return number of dead ends in the graph. A dead end is a vertex where three of it neighbors are blocked
         */
        uint getNumberDeadEnds() const;

        /**
         * @brief getDeadEnd
         * @param id the number of the deadend
         * @return returns the ids dead end
         */
        Position getDeadEnd(uint id) const;

        /**
         * @brief getDeadEnds
         * @return A vector contains all passages;
         */
        const std::vector<Solver::Data::Pos< uint > >& getPassages() const;
        /**
         * @brief getDeadEnds
         * @return A vector with all dead ends;
         */
        const std::vector<Solver::Data::Pos< uint > >& getDeadEnds() const;
    protected:
        /**
         * @brief readTheWorld read the world from an input file stream
         * The first two numbers are the number of rows and columns.
         * after that it follows a matrix of size rows times columns. Each element is either 0,1 or 2.
         * A zero is an free field. An one is a blocked filled and a two represent a passage.
         * @param file the file stream
         */
        void readTheWorld(std::ifstream& file);
        /**
         * @brief readTheRegions Read the reagions in a file. The part consists of:
         * 1. The number of the regions
        *  2. The regions
        *   Each region start with the number of passages. The all passages are listen. A passage is a tuple of (x, y) of a coordinate.
        *   the next number is the number of field that belongs to the passage. then all vertex that belongs to the passage are listen.

         * @param file
         */
        void readTheRegions(std::ifstream& file);

        std::vector< World::Region> regions;
        /**
         * @brief deadEndList The list of all dead ends. A dead end is a vertex, where three neighbor vertexes are blocked.
         * In the whole graph there are only two dead ends possible, in the case that the search doesn't start. In the case that the
         * start has begun only one dead end is possible.
         */
        std::vector< Position > deadEndList;
        uint numberDeadEnd;

        /**
         * @brief passageList The list of all passages. A passage is a vertex where two regions are connected.
         */
        std::vector< Position > passageList;
        uint numberPassageList;

        std::vector< bool > blocked;
        /**
         * @brief rows The number of rows in the graph
           @brief cols The number of columns in the graph
         */
        uint rows, cols;
        /**
         * @brief numberRegions number of regions
         */
        uint numberRegions;
    };

}

/**

 * @brief operator<< overload the << operator for output to ostream.
 * @param out: The output stream
 * @param w: The word you like to output
 * @return the changed ostream
 */
std::ostream& operator<<(std::ostream& out, const World::world& w);

#endif // WORLD_H
