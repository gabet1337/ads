#include <iostream>
#include <vector>
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

    size_t n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, 100000000);
    min_distance[source] = 0;
    //previous.clear();
    //previous.resize(n, -1);

    pq::fibonacci_queue vertex_queue(n);

    vertex_queue.push(std::make_pair(min_distance[source],source));

    while (!vertex_queue.empty()) {
		
      int u = vertex_queue.top().second;
      weight_t dist = vertex_queue.top().first;
		
      vertex_queue.pop();
	
      const std::vector<neighbor> &neighbors = adjacency_list[u];
        
      for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin(); neighbor_iter != neighbors.end(); neighbor_iter++) {
            
	int v = neighbor_iter->target;
	int weight = neighbor_iter->weight;
	int distance_through_u = dist + weight;
	    
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
