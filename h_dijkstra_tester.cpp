#include <vector>
#include "h_dijkstra.cpp"

int main() {

  h_dijkstra dijk;

  size_t n = 100000;
  size_t max_edges = 100;
  size_t max_weight = 100;

  h_dijkstra::adjacency_list_t adjacency_list(n);

  srand (time(NULL));

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < ((rand() % max_edges) + 1); j++) {
      adjacency_list[i].push_back(h_dijkstra::neighbor(rand() % (n), rand() % max_weight));
    }
  }


  std::vector<h_dijkstra::weight_t> min_distance;
  std::vector<h_dijkstra::vertex_t> previous;
  dijk.sssp(0, adjacency_list, min_distance, previous);
  std::cout << "Distance from 0 to 4: " << min_distance[4] << std::endl;
    
  //std::list<vertex_t> path = DijkstraGetShortestPathTo(4, previous);
  //std::cout << "Path : ";
  //std::copy(path.begin(), path.end(), std::ostream_iterator<vertex_t>(std::cout, " "));
  //std::cout << std::endl;
	
  return 0;

};
