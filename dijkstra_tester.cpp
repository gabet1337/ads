#include "binary_heap.hpp"
#include "priority_queue.hpp"
#include "fibonacci_queue.hpp"
#include "dijkstra.hpp"
#include "test_lib.hpp"
#include <vector>
#include <iostream>

std::size_t const TEST_SIZE = 9;

void test_easy(vvii &adjList) {

  adjList.resize(TEST_SIZE, vii());

  for (size_t i = 0; i < TEST_SIZE; i++) {
    adjList[i].push_back(ii(i+1,0));
  }

  // insert missing edges
  size_t v_ins = ((2*((TEST_SIZE/3)*(TEST_SIZE/3))+((TEST_SIZE-1)/3))-(TEST_SIZE-1))/TEST_SIZE;

  test::random rand;
  
  for (size_t i = 0; i < adjList.size(); i++) {

    vi pl;
    pl.resize(TEST_SIZE,-1);

    pl[i] = 1;
    pl[i] = i+1;

    size_t count = 0;
    while(true) {
      int r = rand.next(TEST_SIZE);
      if (pl[r] == -1)
	pl[r] = 1;
      else {
	while(pl[(r++)%TEST_SIZE] == 1);
      }

      adjList[i].push_back(ii(r,1000000000));
      
      count++;
      if (count == v_ins)
	break;
    }
    
  }
  
}

void test_hard_illegal(vvii &adjList) {

  adjList.resize(TEST_SIZE, vii());
  
  int h = (TEST_SIZE / 2);

  size_t hweight = TEST_SIZE;

  for (int i = 0; i < h; i++) {
    if (i == 0) {
      adjList[0].push_back(ii(1,hweight));
    } else if (i < h-1) {
      adjList[i].push_back(ii(i+1,1));
    }
    for (size_t j = h; j < TEST_SIZE; j++) {
      if (i ==0) {
	adjList[i].push_back(ii(j,hweight-1));
      } else {
	adjList[i].push_back(ii(j,-2*i));
      }
    }
  }

}

void print(vvii &adjList) {

  std::cout << "digraph {" << std::endl;
  
  for (size_t i = 0; i < adjList.size(); i++) {
    for (size_t j = 0; j < adjList[i].size(); j++) {
      std::cout << i << " -> " << adjList[i][j].first << " [label=\"" << adjList[i][j].second << """\"]" << std::endl;
    }
  }

  std::cout << "}";
  
}

void edge_counter(vvii &adjList) {

  size_t count = 0;
  
  for (size_t i = 0; i < adjList.size(); i++) {
    count = count + adjList[i].size();
  }

  std::cout << "count: " << count << std::endl;
  
}

void test_hard(vvii &adjList) {

  adjList.resize(TEST_SIZE, vii());

  int h = (TEST_SIZE / 3);

  int x = 1000;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < h; j++) {
      if (i == 0) { // layer 0 -> 0
	if (j == 0) {
	  adjList[0*h+j].push_back(ii(0*h+j+1,x*x+h+1));
	}
	else if (j < h-1) {
	  adjList[0*h+j].push_back(ii(0*h+j+1,0));
	}
	for (int k = 0; k < h; k++) { // layer 0 -> 1
	  if (j == 0) {
	    adjList[0*h+j].push_back(ii(1*h+k,x+k));
	  }
	}
	if (j > 0)
	  for (int k = 0; k < h; k++) { // layer 0 -> 2
	    adjList[0*h+j].push_back(ii(2*h+k,x-2*j));
	}
      } else if (i == 1) { // layer 1 - > 2
	for (int k = 0; k < h; k++) {
	  adjList[1*h+j].push_back(ii(2*h+k,2*x*x-2*j));
	}
      }
    }
  }
  
}
void color(int node, vvii &adjList, vi &c) {

  if (c[node] == 1) return;
  
  c[node] = 1;

  for (size_t i = 0; i < adjList[node].size(); i++) {
      color(adjList[node][i].first, adjList, c);
  }
  
}


void connect(vvii &adjList) {

  test::random rand;
  
  vi components;

  vi c;
  c.resize(TEST_SIZE,0);

  for (size_t i = 0; i < adjList.size(); i++) {
    if (c[i] == 1) continue;
    color(i, adjList, c);
    components.push_back(i);
  }

  if (components.size() > 1)
    for (size_t i = 0; i < components.size(); i++)
      adjList[components[i]].push_back(ii(components[(i+1)%components.size()],rand.next(100)));
  
  std::cout << "Connected: " << components.size() << std::endl;
  
}

void test_rand(vvii &adjList) {

  adjList.resize(TEST_SIZE, vii());
  
  size_t v_ins = ((2*((TEST_SIZE/3)*(TEST_SIZE/3))+((TEST_SIZE-1)/3)))/TEST_SIZE;

  test::random rand;
  
  for (size_t i = 0; i < adjList.size(); i++) {

    vi pl;
    pl.resize(TEST_SIZE,-1);

    pl[i] = 1;

    size_t count = 0;
    while(true) {
      int r = rand.next(TEST_SIZE);
      if (pl[r] == -1)
	pl[r] = 1;
      else {
	while(pl[(r++)%TEST_SIZE] == 1);
      }

      adjList[i].push_back(ii(r%TEST_SIZE,rand.next(100)));
      
      count++;
      if (count == v_ins)
	break;
    }
    
  }
  
  connect(adjList);
  
}

int main() {

  vvii AdjList;
  //test_easy(AdjList);
  //test_hard(AdjList);
  test_rand(AdjList);
  //print(AdjList);
  vi dist_bh, dist_fh;

  //edge_counter(AdjList);
  
  pq::binary_heap bh(TEST_SIZE);
  dijkstra(bh, AdjList, dist_bh, TEST_SIZE, 0);

  //std::cout << "got here";
  
  pq::fibonacci_queue fq(TEST_SIZE);
  dijkstra(fq, AdjList, dist_fh, TEST_SIZE, 0);

  //std::cout << dist_fh[TEST_SIZE-1] << std::endl;
  
  //std::cout << "... and here";
  
  return 0;
  
}

