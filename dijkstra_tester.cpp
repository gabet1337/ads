#include "binary_heap.hpp"
#include "priority_queue.hpp"
#include "fibonacci_queue.hpp"
#include "dijkstra.hpp"
#include "test_lib.hpp"
#include <vector>
#include <iostream>

using namespace std;

std::size_t TEST_SIZE = 9;

void test_easy(vvii &adjList) {

  adjList.resize(TEST_SIZE, vii());

  for (size_t i = 0; i < TEST_SIZE-1; i++) {
    adjList[i].push_back(ii(i+1,0));
  }

  // insert missing edges
  size_t v_ins = ((2*((TEST_SIZE/3)*(TEST_SIZE/3))+((TEST_SIZE-1)/3))-(TEST_SIZE-1))/TEST_SIZE;

  if (v_ins > 0) {
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

size_t edge_counter(vvii &adjList) {

  size_t count = 0;
  
  for (size_t i = 0; i < adjList.size(); i++) {
    count = count + adjList[i].size();
  }

  //std::cout << "count: " << count << std::endl;
  return count;
  
}

void test_hard(vvii &adjList) {

  adjList.resize(TEST_SIZE, vii());

  int h = (TEST_SIZE / 3);

  //int x = 1000;
  int x = 8000; // TEST_SIZE;

  for (int i = 0; i < h; i++) {
    for (int j = 0; j < h; j++) {
      if (i == 0) { // layer 0 -> 0
	if (j == 0) {
	  //adjList[0*h+j].push_back(ii(0*h+j+1,x*x+h+1));
	  adjList[0*h+j].push_back(ii(0*h+j+1,x*x));
	}
	else if (j < h-1) {
	  //adjList[0*h+j].push_back(ii(0*h+j+1,0));
	  adjList[0*h+j].push_back(ii(0*h+j+1,0));
	}
	for (int k = 0; k < h; k++) { // layer 0 -> 1
	  if (j == 0) {
	    //adjList[0*h+j].push_back(ii(1*h+k,x+k));
	    adjList[0*h+j].push_back(ii(1*h+k,x+k));
	  }
	}
	if (j > 0)
	  for (int k = 0; k < h; k++) { // layer 0 -> 2
	    //adjList[0*h+j].push_back(ii(2*h+k,x-2*j));
	    adjList[0*h+j].push_back(ii(2*h+k,x-2*j));
	}
      } else if (i == 1) { // layer 1 - > 2
	for (int k = 0; k < h; k++) {
	  //adjList[1*h+j].push_back(ii(2*h+k,2*x*x-2*j));
	  adjList[1*h+j].push_back(ii(2*h+k,x*x+2*x-2*j));
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
  
  cout << "start connecting" << endl;
  
  test::random rand;
  
  vi components;

  vi c;
  c.resize(TEST_SIZE,0);

  cout << "all is good" << endl;
  
  for (size_t i = 0; i < adjList.size(); i++) {
    if (c[i] == 1) continue;
    color(i, adjList, c);
    components.push_back(i);
  }

  cout << "no problems..." << endl;
  
  if (components.size() > 1)
    for (size_t i = 0; i < components.size(); i++)
      adjList[components[i]].push_back(ii(components[(i+1)%components.size()],rand.next(100)));

  cout << "wtf??" << endl;
  
  //std::cout << "Connected: " << components.size() << std::endl;
  
}

void test_rand(vvii &adjList) {

  adjList.resize(TEST_SIZE, vii());
  
  size_t v_ins = ((2*((TEST_SIZE/3)*(TEST_SIZE/3))+((TEST_SIZE-1)/3)))/TEST_SIZE;

  test::random rand;

  if (v_ins > 0) {
  
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
  }
  
  connect(adjList);
  
}

void test_rand_3_edges(vvii &adjList) {

  adjList.resize(TEST_SIZE, vii());
  
  size_t v_ins = 20; // ((2*((TEST_SIZE/3)*(TEST_SIZE/3))+((TEST_SIZE-1)/3)))/TEST_SIZE;

  test::random rand;

  if (v_ins > 0) {
  
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
  }

  connect(adjList);
  
}

struct results {
  long long time, papi;
  uint64_t page_faults;
  size_t TEST_SIZE, TEST_RUNS, NUM_DECREASE_KEYS;
  bool is_random;
  results(long long _time, long long _papi, uint64_t _page_faults,
          size_t _TEST_SIZE, size_t _TEST_RUNS, bool _is_random, size_t _NUM_DECREASE_KEYS)
    : time(_time), papi(_papi), page_faults(_page_faults),
      TEST_SIZE(_TEST_SIZE), TEST_RUNS(_TEST_RUNS), NUM_DECREASE_KEYS(_NUM_DECREASE_KEYS),
      is_random(_is_random)
       {}
};

void print_results(results r, string file) {
  // long long time, papi;
  // uint64_t page_faults;
  // size_t TEST_SIZE, TEST_RUNS;
  // bool is_random;
  ofstream f(file, ios::app|ios::out);
  f << "#size\ttime\tpapi\tpfs\tndk" << endl;
  f << r.TEST_SIZE << "\t"
    << r.time/r.TEST_RUNS << "\t"
    << r.papi/r.TEST_RUNS << "\t"
    << r.page_faults/r.TEST_RUNS << "\t"
    << r.NUM_DECREASE_KEYS
    << endl;
  
}

pair<results,results> test_sssp(size_t TEST_SIZE, size_t TEST_RUNS, vvii &AdjList) {

  test::pagefaults pf;
  test::clock c;
  int events[1] = {PAPI_BR_CN};
  long long values[1];
  test::PAPI p(events, values, 1);
  
  long long bh_clock = 0, fq_clock = 0;
  long long bh_br = 0, fq_br = 0;
  uint64_t bh_pf = 0, fq_pf = 0;
  size_t bh_dk = 0, fq_dk = 0;
  
  vi dist_bh, dist_fh;

  for (size_t i = 0; i < TEST_RUNS; i++) {
  
    pq::binary_heap bh(TEST_SIZE);
    pq::fibonacci_queue fq(TEST_SIZE);

    p.start();
    c.start();
    pf.start();
    bh_dk += dijkstra(bh,AdjList, dist_fh, TEST_SIZE, 0);
    pf.stop();
    c.stop();
    p.stop();
    bh_clock+=c.count();
    bh_br += values[0];
    bh_pf += pf.count();

    p.start();
    c.start();
    pf.start();
    fq_dk += dijkstra(fq, AdjList, dist_fh, TEST_SIZE, 0);
    pf.stop();
    c.stop();
    p.stop();
    fq_clock+=c.count();
    fq_br += values[0];
    fq_pf += pf.count();
    
  }
  
  cout << "\tbh\tfq" << endl;
  cout << "hc\t" << bh_br / TEST_RUNS << "\t" << fq_br / TEST_RUNS << endl;
  cout << "ti\t" << bh_clock / TEST_RUNS << "\t" << fq_clock /TEST_RUNS << endl;
  cout << "pf\t" << bh_pf / TEST_RUNS << "\t" << fq_pf /TEST_RUNS << endl;
  cout << "dk\t" << bh_dk / TEST_RUNS << "\t" << fq_dk / TEST_RUNS << endl;
  
  return { results(bh_clock, bh_br, bh_pf, TEST_SIZE, TEST_RUNS, false, bh_dk),
      results(fq_clock, fq_br, fq_pf, TEST_SIZE, TEST_RUNS, false, fq_dk)};
}

//test powers of 2
void test_powers_of_2_rand() {

  for (size_t i = 3; i < 16; i++) {

    TEST_SIZE = (1<<i);
    
    vvii AdjList;
    //test_easy(AdjList);
    //test_hard(AdjList);
    test_rand(AdjList);
    
    cout << "TESTING DIJKSTRA RANDOMLY ON SIZE |V]=" << TEST_SIZE << " |E|=" << edge_counter(AdjList) << endl;

    pair<results,results> temp = test_sssp(TEST_SIZE, 5, AdjList);    

    print_results(temp.first, "res/dijkstra/sssp_random_bh.dat");
    print_results(temp.second, "res/dijkstra/sssp_random_fq.dat");
  }
  
}

void test_powers_of_2_chain() {

  for (size_t i = 3; i < 16; i++) {

    TEST_SIZE = (1<<i);
    
    vvii AdjList;
    test_easy(AdjList);
    //test_hard(AdjList);
    //test_rand(AdjList);
    
    cout << "TESTING DIJKSTRA CHAIN ON SIZE |V]=" << TEST_SIZE << " |E|=" << edge_counter(AdjList) << endl;

    pair<results,results> temp = test_sssp(TEST_SIZE, 5, AdjList);    

    print_results(temp.first, "res/dijkstra/sssp_chain_bh.dat");
    print_results(temp.second, "res/dijkstra/sssp_chain_fq.dat");
  }
  
}

void test_powers_of_2_heavy() {

  for (size_t i = 3; i < 16; i++) {

    TEST_SIZE = (1<<i);
    
    vvii AdjList;
    //test_easy(AdjList);
    test_hard(AdjList);
    //test_rand(AdjList);
    
    cout << "TESTING DIJKSTRA HEAVY ON |V]=" << TEST_SIZE << " |E|=" << edge_counter(AdjList) << endl;

    pair<results,results> temp = test_sssp(TEST_SIZE, 5, AdjList);    

    print_results(temp.first, "res/dijkstra/sssp_heavy_bh.dat");
    print_results(temp.second, "res/dijkstra/sssp_heavy_fq.dat");
  }
  
}

void test_powers_of_2_rand_3_edges() {

  for (size_t i = 18; i < 19; i++) {

    TEST_SIZE = (1<<i);
    
    vvii AdjList;
    test_rand_3_edges(AdjList);
    cout << "TESTING DIJKSTRA CHAIN ON RANDOM_3_E |V]=" << TEST_SIZE << " |E|=" << edge_counter(AdjList) << endl;

    pair<results,results> temp = test_sssp(TEST_SIZE, 2, AdjList);    

    //print_results(temp.first, "res/dijkstra/sssp_heavy_bh.dat");
    //print_results(temp.second, "res/dijkstra/sssp_heavy_fq.dat");
  }
  
}

int main() {

  test_powers_of_2_rand_3_edges();
  
  //TEST_SIZE = 100000000;
  
  //vvii AdjList;
  //test_rand_3_edges(AdjList);
  
  //print(AdjList);
  
  //test_powers_of_2_rand();
  //test_powers_of_2_chain();
  //test_powers_of_2_heavy();
  
  //vvii AdjList;
  //test_easy(AdjList);
  //test_hard_illegal(AdjList);
  //test_hard(AdjList);
  //test_rand(AdjList);
  //print(AdjList);

  //test_powers_of_2();

  //vi dist_bh, dist_fh;

  //edge_counter(AdjList);
  
  //pq::binary_heap bh(TEST_SIZE);
  //dijkstra(bh, AdjList, dist_bh, TEST_SIZE, 0);

  //std::cout << "got here";
  
  //pq::fibonacci_queue fq(TEST_SIZE);
  //dijkstra(fq, AdjList, dist_fh, TEST_SIZE, 0);

  //std::cout << dist_fh[TEST_SIZE-1] << std::endl;
  
  //std::cout << "... and here";
  
  return 0;
  
}

