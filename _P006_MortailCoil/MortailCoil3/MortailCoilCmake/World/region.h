#ifndef REGION_H
#define REGION_H
#include <fstream>
#include <vector>
#include <boost/dynamic_bitset.hpp>
#include <ostream>
#include "../Solver/Data/pos.h"

typedef unsigned int uint;

namespace World{
    class Region
    {
    public:
        /**
          set all variables to 0
         * @brief Region
         */
        Region();
        /**
          the copy constructor
         * @brief Region
         * @param other
         */
        Region(const Region& other);

        /**
          a region is a part of the graph.
         * @brief Region
         * @param rows The number of rows of the whole graph
         * @param columns The number of columns of the whole graph
         * @param data the input file
         */
        Region(uint columns, uint rows, std::ifstream& data);

        ~Region();

        /**
          Copyoperator
         * @brief operator =
         * @param other
         * @return
         */
        Region& operator=(const Region& other);
        /**
         * @brief swap Swap the blocking state of a vertex from blocked to unblocked and form unblocked to blocked.
         * the variables col and row are the position of the swap
         * @param col
         * @param row
         * @return
         */
        void swap(const uint col, const uint row);

        /**
         * @brief setBlocked the the state of the vertex to blocked;
         * @param col
         * @param row
         */
        void setBlocked(const uint col, const uint row);

        /**
         * @brief setUnblocked set the state of a vertex to unblocked
         * @param col
         * @param row
         */
        void setUnblocked(const uint col, const uint row);
        /**
         * @brief isBlocked check wheter the vertex at position (col, row) is blocked or not
         * @param col
         * @param row
         * @return true if the vertex is blocked and false if it is unblocked
         */
        bool isBlocked(const int col, const int row) const;
        bool isBlocked(const Position& pos) const;

        std::vector<Position > getUnblocked() const;
        /**
         * @brief isPassage check wheter the vertex at position (col, row) is a passage or not
         * @param col
         * @param row
         * @return true if it is a passage, false otherwise
         */
        bool isPassage(const uint col, const uint row) const;
        bool isPassage(const Position& pos) const;

        /**
         * @brief getPassages
         * @return a vector with all passages
         */
        std::vector<Position > getPassages() const;
        /**
         * @brief getNumberPassages
         * @return the number of passages
         */
        uint getNumberPassages() const;
        /**
         * @brief getNumberDeadEnd
         * @return the number of dead ends
         */
        uint getNumberDeadEnd() const;
        /**
         * @brief getColumnCount
         * @return the number of columns in the whole graph
         */
        uint getColumnCount()const;
        /**
         * @brief getRowCount
         * @return the number of rows in the whole graph
         */
        uint getRowCount()const;
        /**
         * @brief getNumberUnblocked
         * @return the number of unblocked fields
         */
        uint getNumberUnblocked()const;

        /**
          After all moves are finished, the current row and column shows where the coil is
         * @brief getCurrentRow
         * @return
         */
        uint getCurrentRow() const;
        uint getCurrentCol() const;
    protected:

/**
    A dead end is a field where 3 neighbours are blocked. The coil must finish or start here.
         * @brief numberDeadEnd
*/
        uint numberDeadEnd;
        /**
          passages are connection points between two regions
         * @brief numberPassages
         */
        uint numberPassages;
        /**
          number of vertexes which are unblocked
         * @brief numberUnblocked
         */
        uint numberUnblocked;
        /**
         * @brief sizeX
         */
        uint rows, cols;
        boost::dynamic_bitset<> blocked;
        boost::dynamic_bitset<> passage;
    };
}

std::ostream& operator <<(std::ostream& out, const World::Region& w);
#endif // REGION_H
