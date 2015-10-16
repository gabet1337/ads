#include "test_lib.hpp"
#include <papi.h>
#include <iostream>
#include "binary_heap.hpp"
#include "fibonacci_queue.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
typedef pair<int,int> ii;

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

typedef pair<results,results> rr;

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

//inserts TEST_SIZE elements into the prio queues TEST_RUNS times
pair<results,results> test_insert(size_t TEST_SIZE, size_t TEST_RUNS, bool is_random) {
  test::pagefaults pf;
  test::random r;
  test::clock c;
  int events[1] = {PAPI_BR_CN};
  long long values[1];
  test::PAPI p(events, values, 1);

  vector<int> data;
  data.resize(TEST_SIZE);
  if (is_random)
    for (size_t i = 0; i < TEST_SIZE; i++) data[i] = r.next();
  else
    for (size_t i = 0; i < TEST_SIZE; i++) data[i] = TEST_SIZE-i;

  long long bh_clock = 0, fq_clock = 0;
  long long bh_br = 0, fq_br = 0;
  uint64_t bh_pf = 0, fq_pf = 0;
  size_t bh_bc = 0;
  for (size_t i = 0; i < TEST_RUNS; i++) {

    pq::binary_heap bh(TEST_SIZE);
    pq::fibonacci_queue fq(TEST_SIZE);

    p.start();
    c.start();
    pf.start();
    for (size_t j = 0; j < TEST_SIZE; j++) fq.push(ii(data[j],j));
    p.stop();
    c.stop();
    pf.stop();
    fq_clock+=c.count();
    fq_br += values[0];
    fq_pf += pf.count();

    p.start();
    c.start();
    pf.start();
    for (size_t j = 0; j < TEST_SIZE; j++) bh.push(ii(data[j],j));
    p.stop();
    c.stop();
    pf.stop();
    bh_clock+=c.count();
    #ifdef BUBBLE_COUNT
    bh_bc += bh.bcount;
    #endif
    bh_br += values[0];
    bh_pf += pf.count();

  }
  
  cout << bh_br << "\t" << fq_br << endl;
  cout << bh_clock << "\t" << fq_clock << endl;
  cout << bh_pf << "\t" << fq_pf << endl;

  return { results(bh_clock, bh_br, bh_pf, TEST_SIZE, TEST_RUNS, is_random, bh_bc),
      results(fq_clock, fq_br, fq_pf, TEST_SIZE, TEST_RUNS, is_random,0)};
}

//test powers of 2
void test_ins() {
  //random:
  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING INSERTS RANDOMLY ON SIZE: " << s << endl;
    rr temp = test_insert(s,100,true);
    print_results(temp.first, "res/inserts/inserts_random_bh_count.dat");
    print_results(temp.second, "res/inserts/inserts_random_fq_count.dat");
  }

  //worst case:
  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING INSERTS WORST CASE ON SIZE: " << s << endl;
    rr temp = test_insert(s,100,false);
    print_results(temp.first, "res/inserts/inserts_worst_bh_count.dat");
    print_results(temp.second, "res/inserts/inserts_worst_fq_count.dat");
  }
  
}

//measure total cycles
pair<results,results> test_findmin(size_t TEST_SIZE, size_t TEST_RUNS) {
  pq::binary_heap bh(TEST_SIZE);
  pq::fibonacci_queue fq(TEST_SIZE);
  for (size_t i = 0 ; i < TEST_SIZE; i++) {
    bh.push(ii(i,i));
    fq.push(ii(i,i));
  }
  int events[1] = {PAPI_TOT_CYC};
  long long values[1];
  test::PAPI p(events, values, 1);
  long long bh_tc = 0, fq_tc = 0;
  for (size_t i = 0; i < TEST_RUNS; i++) {
    p.start();
    bh.top();
    p.stop();
    bh_tc += values[0];
    p.start();
    fq.top();
    p.stop();
    fq_tc += values[0];
  }
  cout << bh_tc << "\t" << fq_tc << " diff: " << abs(bh_tc-fq_tc) << endl;
  return { results(0, bh_tc, 0, TEST_SIZE, TEST_RUNS, false, 0),
      results(0, fq_tc, 0, TEST_SIZE, TEST_RUNS, false, 0)};

}

void test_fm() {
  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    rr temp = test_findmin(s, 10000);
    print_results(temp.first, "res/findmin/bh_cyc.dat");
    print_results(temp.second, "res/findmin/fq_br.dat");
  }

}

pair<results,results> test_decrease_key(size_t TEST_SIZE, size_t TEST_RUNS, size_t NUM_DECREASE_KEY, bool is_random) {
  vector<int> bh_data, fq_data;
  vector<ii> dk;
  bh_data.resize(TEST_SIZE); fq_data.resize(TEST_SIZE);
  dk.resize(NUM_DECREASE_KEY);
  test::random r;
  if (is_random) {
    for (size_t i = 0; i < TEST_SIZE; i++) {
      bh_data[i] = fq_data[i] = r.next();
    }
  } else {
    // keys: from NUM_DECREASE_KEY+1 to NUM_DECREASE_KEY+TEST_SIZE+1
    for (size_t i = 0; i < TEST_SIZE; i++) {
      fq_data[i] = bh_data[i] = 1+i+NUM_DECREASE_KEY;
    }
  }

  for (size_t i = 0; i < NUM_DECREASE_KEY; i++) {
    if (is_random) {
      int x = r.next(TEST_SIZE);
      dk[i] = ii(x, min(bh_data[x], fq_data[x]));
    } else {
      int node = (TEST_SIZE-1)-i;
      if (node < 0)
        while (node < 0) node += TEST_SIZE;
      
      dk[i] = ii(node, NUM_DECREASE_KEY-i);
    }
  }

  int events[1] = {PAPI_BR_CN};
  long long values[1];
  test::PAPI p(events, values, 1);
  test::clock c;
  test::pagefaults pf;
  long long bh_c = 0, fq_c = 0, bh_br = 0, fq_br = 0, bh_pf = 0, fq_pf = 0;
  for (size_t i = 0; i < TEST_RUNS; i++) {
    pq::binary_heap bh(TEST_SIZE);
    pq::fibonacci_queue fq(TEST_SIZE);
    for (size_t j = 0; j < TEST_SIZE; j++) {
      bh.push(ii(bh_data[j], j));
      fq.push(ii(fq_data[j], j));
    }
    //pop to make sure fibonacci queue has consolidated!
    bh.pop(); fq.pop();
    //first decrease key randomly in the heap
    pf.start();
    c.start(); p.start();
    for (size_t j = 0; j < NUM_DECREASE_KEY; j++) {
      bh.decrease_key(dk[j].first, dk[j].second);
    }
    p.stop(); c.stop(); pf.stop();
    bh_c += c.count(); bh_br += values[0]; bh_pf += pf.count();

    pf.start();
    c.start(); p.start();
    for (size_t j = 0; j < NUM_DECREASE_KEY; j++) {
      fq.decrease_key(dk[j].first, dk[j].second);
    }
    p.stop(); c.stop(); pf.stop();
    fq_c += c.count(); fq_br += values[0]; fq_pf += pf.count();
  }
  
  cout << bh_c << "\t" << fq_c << endl;
  cout << bh_br << "\t" << fq_br << endl;
  cout << bh_pf << "\t" << fq_pf << endl;

  return { results(bh_c, bh_br, bh_pf, TEST_SIZE, TEST_RUNS, is_random, NUM_DECREASE_KEY),
      results(fq_c, fq_br, fq_pf, TEST_SIZE, TEST_RUNS, is_random, NUM_DECREASE_KEY)};

}
//test_decrease_key(size_t TEST_SIZE, size_t TEST_RUNS, size_t NUM_DECREASE_KEY, bool is_random) {
void test_dk() {

  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING on input size: " << s << " where we decrease 1 element at worst" << endl;
    rr temp = test_decrease_key(s, 100, 1, false);
    print_results(temp.first, "res/dk/ndk_worst_bh_1.dat");
    print_results(temp.second, "res/dk/ndk_worst_fq_1.dat");
  }
  return;
  size_t TEST_SIZE = (1<<15);
  //random, vary on num of decrease key operations!

  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " DECREASE KEYS randomly" << endl;
    rr temp = test_decrease_key(TEST_SIZE, 100, s, true);
    print_results(temp.first, "res/dk/ndk_random_bh.dat");
    print_results(temp.second, "res/dk/ndk_random_fq.dat");
  }

  //worst case, vary on num of decrease key operations
  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " DECREASE KEYS worst case" << endl;
    rr temp = test_decrease_key(TEST_SIZE, 100, s, false);
    print_results(temp.first, "res/dk/ndk_worst_bh.dat");
    print_results(temp.second, "res/dk/ndk_worst_fq.dat");
  }

}

pair<results,results> test_deletemin(size_t TEST_SIZE, size_t TEST_RUNS, size_t NUM_DELETES, bool is_random) {
  vector<int> bh_data, fq_data;
  bh_data.resize(TEST_SIZE);
  fq_data.resize(TEST_SIZE);
  //generate data
  test::random r;
  if (is_random) {
    for (size_t i = 0; i < TEST_SIZE; i++)
      bh_data[i] = fq_data[i] = r.next();
  } else {
    for (size_t i = 0; i < TEST_SIZE; i++) {
      bh_data[i] = i;
      fq_data[i] = TEST_SIZE-i;
    }
  }

  int events[1] = {PAPI_BR_CN};
  long long values[1];
  test::PAPI p(events, values, 1);
  test::clock c;
  test::pagefaults pf;
  long long bh_c = 0, fq_c = 0, bh_br = 0, fq_br = 0, bh_pf =0, fq_pf = 0;
  size_t bcount = 0;
  for (size_t t = 0; t < TEST_RUNS; t++) {
    pq::binary_heap bh(TEST_SIZE);
    pq::fibonacci_queue fq(TEST_SIZE);
    for (size_t i = 0; i < TEST_SIZE; i++) {
      bh.push(ii(bh_data[i], i));
      fq.push(ii(fq_data[i], i));
    }
    // pop one element
    if (NUM_DELETES > 1) { bh.pop(); fq.pop();}
    pf.start();
    c.start(); p.start();
    for (size_t i = 0; i < (size_t)max(1,((int)NUM_DELETES)-1); i++) {
      bh.pop();
    }
    p.stop(); c.stop(); pf.stop();
    bh_c+=c.count(); bh_br+=values[0]; bh_pf+=pf.count();
    #ifdef BUBBLE_COUNT
    bcount+=bh.bcount;
    #endif
    
    c.start(); p.start(); pf.start();
    for (size_t i = 0; i < max(1,((int)NUM_DELETES)-1); i++) {
      fq.pop();
    }
    p.stop(); c.stop(); pf.stop();
    fq_c+=c.count(); fq_br+=values[0]; fq_pf += pf.count();
  }
  
  cout << bh_c << "\t" << fq_c << endl;
  cout << bh_br << "\t" << fq_br << endl;
  cout << bh_pf << "\t" << fq_pf << endl;
  return { results(bh_c, bh_br, bh_pf, TEST_SIZE, TEST_RUNS, bcount, NUM_DELETES),
      results(fq_c, fq_br, fq_pf, TEST_SIZE, TEST_RUNS, bcount, NUM_DELETES)};
}

//pair<results,results> test_deletemin(size_t TEST_SIZE, size_t TEST_RUNS, size_t NUM_DELETES, bool is_random) {
void test_delmin() {
  // size_t TEST_SIZE = (1<<20);
  //lets try construct and delete everything (:
  // for (size_t i = 1; i < 22; i++) {
  //   size_t s = (1<<i);
  //   cout << "TESTING : " << s << " with " << s-1 << " DELETE random data" << endl;
  //   rr temp = test_deletemin(s, 100, s, true);
  //   print_results(temp.first, "res/delmin/random_del_all_bh.dat");
  //   print_results(temp.second, "res/delmin/random_del_all_fq.dat");
  // }

  // for (size_t i = 1; i < 22; i++) {
  //   size_t s = (1<<i);
  //   cout << "TESTING : " << s << " with " << s-1 << " DELETE worst data" << endl;
  //   rr temp = test_deletemin(s, 100, s, false);
  //   print_results(temp.first, "res/delmin/worst_del_all_bh.dat");
  //   print_results(temp.second, "res/delmin/worst_del_all_fq.dat");
  // }

  for (size_t i = 1; i < 22; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " with " << 1 << " DELETE worst data" << endl;
    rr temp = test_deletemin(s, 50, 1, false);
    print_results(temp.first, "res/delmin/worst_del_1_bh.dat");
    print_results(temp.second, "res/delmin/worst_del_1_fq.dat");
  }

  for (size_t i = 1; i < 22; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " with " << 1 << " DELETE random data" << endl;
    rr temp = test_deletemin(s, 50, 1, true);
    print_results(temp.first, "res/delmin/random_del_1_bh.dat");
    print_results(temp.second, "res/delmin/random_del_1_fq.dat");
  }

    for (size_t i = 1; i < 22; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " with " << 2 << " DELETE worst data" << endl;
    rr temp = test_deletemin(s, 50, 2, false);
    print_results(temp.first, "res/delmin/worst_del_2_bh.dat");
    print_results(temp.second, "res/delmin/worst_del_2_fq.dat");
  }

  for (size_t i = 1; i < 22; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " with " << 2 << " DELETE random data" << endl;
    rr temp = test_deletemin(s, 50, 2, true);
    print_results(temp.first, "res/delmin/random_del_2_bh.dat");
    print_results(temp.second, "res/delmin/random_del_2_fq.dat");
  }
}

int main() {

  //test_ins();
  //test_fm();
  //test_dk();
  test_delmin();

  // cout << "insert randomly" << endl;
  // rr tir = test_insert(100000, 100,true);
  // print_results(tir.first, "res/insert.dat");
  // cout << "insert worst case" << endl;
  // test_insert(100000, 100,false);
  // cout << "find min" << endl;
  // test_findmin(100000, 1);
  // cout << "decrease key randomly" << endl;
  // test_decrease_key(262145, 1000, 100, true);
  // cout << "decrease key worst" << endl;
  // test_decrease_key(262145, 1000, 100, false);
  // cout << "delete min randomly" << endl;
  // test_deletemin(262145, 100, 10000, true);
  // cout << "delete min worst" << endl;
  // test_deletemin(262145, 100, 10000, false);

  return 0;
}
