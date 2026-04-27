/******************************************************************************************************/
/*                                Laurent Hébert-Dufresne (dynamica2013)                              */
/*                          Calculation of mean shortest path from an edgelist                        */
/******************************************************************************************************/
#include <boost/config.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
//elements from boost library
  #include <boost/graph/graph_traits.hpp>
  #include <boost/graph/adjacency_list.hpp>
  #include <boost/random/mersenne_twister.hpp>
  #include <boost/random/uniform_real.hpp>
  #include <boost/random/variate_generator.hpp>
  #include <boost/config.hpp>
  #include <boost/iterator_adaptors.hpp>
  #include <boost/graph/properties.hpp>
  #include <boost/graph/graph_utility.hpp>
  #include <boost/multi_array.hpp>
  #include <boost/graph/johnson_all_pairs_shortest.hpp>


using namespace std;
using namespace boost;

// Declare the graph type and its vertex type
typedef adjacency_list<vecS, vecS, undirectedS, no_property, property< edge_weight_t, int, property< edge_weight2_t, int > > > Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;

template <typename Graph, typename VertexMap>
typename boost::graph_traits<Graph>::vertex_descriptor
add_named_vertex(Graph& g, const std::string& name, VertexMap& vm)
{
    typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename VertexMap::iterator Iterator;

    Vertex v;
    Iterator iter;
    bool inserted;
    boost::tie(iter, inserted) = vm.insert(make_pair(name, Vertex()));
    if(inserted) {
        // The name was unique so we need to add a vertex to the graph
        v = add_vertex(g);
        iter->second = v;
    }
    else {
        // We had alread inserted this name so we can return the
        // associated vertex.
        v = iter->second;
    }
    return v;
}

int main(int argc, char *argv[]) {
	
    //NETWORK
	int N = atoi(argv[1]);
    std::string path1 = argv[2];
    std::string path2 = argv[3];

    //INPUT AND CONSTRUCTION OF STRUCTURES
    //PREPARES INPUT & OUTPUT
    ifstream EdgeFile(path1.c_str());
    ofstream output(path2.c_str());
    if(!EdgeFile.is_open()) std::cout << "SNAFU." << std::endl;
    // Create the graph and a name map that provides access to
    // then actor names.
    Graph g;
    std::set<string> nm;

    // Read the graph from standard input.
    std::map<std::string, Vertex> verts;
    for(int n=0;n<N;++n) {
		char number[5];
		sprintf (number, "%i", n);
		std::string nodename = number;
		add_named_vertex(g, number, verts);
	}
    for(std::string line; std::getline(EdgeFile, line); ) {
        if(line.empty()) continue;
        std::size_t index = line.find_first_of('	');
        std::string first(line, 0, index);
        std::string second(line, index + 1);
        Vertex u = add_named_vertex(g, first, verts);
        Vertex v = add_named_vertex(g, second, verts);
        add_edge(u, v, g);
    }
    const int V = verts.size();
    
    property_map < Graph, edge_weight_t >::type w = get(edge_weight, g);
    graph_traits < Graph >::edge_iterator e, e_end;
	for (boost::tie(e, e_end) = edges(g); e != e_end; ++e) w[*e] = 1;
    
    
	boost::multi_array<int,2> D(boost::extents[V][V]);
	johnson_all_pairs_shortest_paths(g, D);
	
	double count = 0;
	double L = 0;
	for(int q=0; q<V; ++q) {
		for(int l=0; l<V; ++l) {
			if(D[q][l] < 1 || D[q][l] > V) output << numeric_limits<int>::infinity() << "\t";
			else output << D[q][l] << "\t";
		}
		if(q<(V-1)) output << "\n";
	}

    return 0;
}
