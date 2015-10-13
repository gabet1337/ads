#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <vector>
#include "priority_queue.hpp"

#define INF 1000000000
typedef std::pair<int,int> ii;
typedef std::vector<int> vi;
typedef std::vector<vi> vvi;
typedef std::vector<ii> vii;
typedef std::vector<vii> vvii;

void dijkstra(priority_queue &pq, vvii &AdjList, vi &dist, int V, int s) {
  dist.resize(V);
  dist[s] = 0;
  for (int i = 0; i < V; i++) {
    if (i != s) dist[i] = INF;
    pq.push(ii(dist[i], i));
  }

  while (!pq.empty()) {
    ii front = pq.top(); pq.pop();
    int d = front.first, u = front.second;
    if (d > dist[u]) continue;
    for (auto &v : AdjList[u]) {
      if (dist[u] + v.second < dist[v.first]) {
        dist[v.first] = dist[u] + v.second;
        pq.decrease_key(v.first, dist[v.first]);
      }
    }
  }
}


#endif
