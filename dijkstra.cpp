#include "binary_heap.hpp"
#include "priority_queue.hpp"
#include "fibonacci_heap.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#define INF 1000000000
typedef std::pair<int,int> ii;
typedef std::vector<int> vi;
typedef std::vector<ii> vii;
typedef std::vector<vii> vvii;

void dijkstra(priority_queue<ii> &pq, vvii &AdjList, vi &dist, int V, int s) {
  dist.assign(V, INF);
  dist[s] = 0;
  pq.push(ii(0,s));
  while (!pq.empty()) {
    ii front = pq.top(); pq.pop();
    int d = front.first, u = front.second;
    if (d > dist[u]) continue;
    for (int j = 0; j < (int)AdjList[u].size(); j++) {
      ii v = AdjList[u][j];
      if (dist[u] + v.second < dist[v.first]) {
        dist[v.first] = dist[u] + v.second;
        pq.push(ii(dist[v.first], v.first));
      }
    }
  }
}

int main() {
  int V = 6;
  vvii AdjList(V, vii());
  AdjList[0].push_back(ii(1,5));
  AdjList[1].push_back(ii(2,1));
  AdjList[1].push_back(ii(3,5));
  AdjList[1].push_back(ii(5,3));
  AdjList[2].push_back(ii(4,1));
  AdjList[3].push_back(ii(4,1));
  AdjList[4].push_back(ii(3,2));
  AdjList[5].push_back(ii(4,2));
  vi dist,dist2;
  pq::binary_heap<ii> pq;
  pq::fibonacci_heap<ii> pq2;
  dijkstra(pq, AdjList, dist, V, 0);
  dijkstra(pq2, AdjList, dist2, V, 0);
  std::cout << dist[4] << std::endl;
  std::cout << dist2[4] << std::endl;
  return 0;
}
