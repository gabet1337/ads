#include "binary_heap.hpp"
#include "priority_queue.hpp"
#include "fibonacci_queue.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <stdlib.h>
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

void stress_test() {
  long long TEST_SIZE = 1000000;
  srand(time(NULL));
  vvii G(TEST_SIZE, vii());
  // vvi F(TEST_SIZE, vi(TEST_SIZE,0));
  for (size_t i = 0; i < 1000000000; i++) {
    int x = rand()%TEST_SIZE, y = rand()%TEST_SIZE;
    int w = rand()%1000;
    w++;
    if (x == y) continue;
    // if (F[x][y] == 0) {
    G[x].push_back(ii(y,w));
    G[y].push_back(ii(x,w));
      // F[x][y] = F[y][x] = w;
    
  }

  // for (int k = 0; k < TEST_SIZE; k++) {
  //   for (int i = 0; i < TEST_SIZE; i++) {
  //     for (int j = 0; j < TEST_SIZE; j++) {
  //       if (F[i][j] > F[i][k] + F[k][j])
  //         F[i][j] = F[i][k] + F[k][j];
  //     }
  //   }
  // }

  pq::binary_heap pq(TEST_SIZE);
  vi dist;
  dijkstra(pq, G, dist, TEST_SIZE, 0);
  
  // for (int i = 0; i < TEST_SIZE; i++) {
  //   // if (dist[i] != F[0][i]) std::cout << "MISMATCH " << dist[i] << " " << F[i][0] << std::endl;
  //   // else std::cout << "MATCH " << dist[i] << " " << F[0][i] << std::endl;
  // }
}

int main() {

  stress_test();
  return 0;
  
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
  pq::binary_heap pq(V);
  //pq::fibonacci_heap<ii> pq2;
  dijkstra(pq, AdjList, dist, V, 0);
  //dijkstra(pq2, AdjList, dist2, V, 0);
  std::cout << dist[4] << std::endl;
  //std::cout << dist2[4] << std::endl;
  return 0;
}
