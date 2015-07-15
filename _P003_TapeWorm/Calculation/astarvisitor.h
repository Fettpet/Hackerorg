#ifndef ASTARVISITOR_H
#define ASTARVISITOR_H
#include <boost/graph/astar_search.hpp>
#include <boost/graph/adjacency_list.hpp>

// Defining Graph Stucture
typedef boost::adjacency_list< boost::listS,
                               boost::listS,
                               boost::undirectedS> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor   edge_t;

//boost::astar_search_no_init(const~VertexListGraph &g, graph_traits<~VertexListGraph>::vertex_descriptor s, AStarHeuristic h, AStarVisitor vis, PredecessorMap predecessor, CostMap cost, DistanceMap distance, WeightMap weight, ColorMap color, VertexIndexMap index_map, CompareFunction compare, CombineFunction combine, CostInf, CostZero zero)
class AStarVisitor: public boost::default_astar_visitor
{
public:
    AStarVisitor();
    //void examine_vertex(Vertex &u, Graph &g);

};

#endif // ASTARVISITOR_H
