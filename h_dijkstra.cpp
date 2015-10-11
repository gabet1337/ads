#include <iostream>
#include <vector>
#include <time.h>
#include <list>

#include "fibonacci_queue.hpp"
 
class h_dijkstra {

public:

  h_dijkstra() {
  }

  ~h_dijkstra() {
  }

  typedef int vertex_t;
  typedef int weight_t;
 
  struct neighbor {
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight)
      : target(arg_target), weight(arg_weight) { }
  };
 
  typedef std::vector<std::vector<neighbor> > adjacency_list_t;
 
  void sssp(vertex_t source, const adjacency_list_t &adjacency_list, std::vector<weight_t> &min_distance, std::vector<vertex_t> &previous) {

    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, 100000000);
    min_distance[source] = 0;
    //previous.clear();
    //previous.resize(n, -1);

    pq::fibonacci_queue<int> vertex_queue;

    vertex_queue.push(source, min_distance[source]);

    while (!vertex_queue.empty()) {
		
      vertex_t u = vertex_queue.top();
      weight_t dist = min_distance[u];
		
      vertex_queue.pop();
	
      const std::vector<neighbor> &neighbors = adjacency_list[u];
        
      for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin(); neighbor_iter != neighbors.end(); neighbor_iter++) {
            
	vertex_t v = neighbor_iter->target;
	weight_t weight = neighbor_iter->weight;
	weight_t distance_through_u = dist + weight;
	    
	if (distance_through_u < min_distance[v]) {
	  min_distance[v] = distance_through_u;
	  //previous[v] = u;
	  vertex_queue.decrease_key(v, min_distance[v]);
	}
      }
    }
  }
 
 
  std::list<vertex_t> DijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t> &previous) {
    std::list<vertex_t> path;
    for ( ; vertex != -1; vertex = previous[vertex])
      path.push_front(vertex);
    return path;
  }
 
};
