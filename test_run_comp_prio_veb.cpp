#include "test_lib.hpp"
#include <papi.h>
#include <iostream>
#include "binary_heap.hpp"
#include "fibonacci_queue.hpp"
#include "van_emde_boas.hpp"
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#define TEST_MAX 16777216
using namespace std;
typedef pair<int,int> ii;

struct results {
  long long time;
  uint64_t page_faults;
  size_t TEST_SIZE, TEST_RUNS, PAPI1, PAPI2, PAPI3;
  bool is_random;
  results(long long _time,
          uint64_t _page_faults,
          size_t _TEST_SIZE, size_t _TEST_RUNS,
          size_t _PAPI1, size_t _PAPI2, size_t _PAPI3)
    : time(_time),
      page_faults(_page_faults),
      TEST_SIZE(_TEST_SIZE), TEST_RUNS(_TEST_RUNS),
      PAPI1(_PAPI1), PAPI2(_PAPI2), PAPI3(_PAPI3)
  {}
};

typedef pair<results,results> rr;
typedef pair<results,pair<results,results> > rrr;

void print_results(results r, string file) {
  // long long time, papi;
  // uint64_t page_faults;
  // size_t TEST_SIZE, TEST_RUNS;
  // bool is_random;
  ofstream f(file, ios::app|ios::out);
  f << "#size\ttime\tpfs\tPAPI1\tPAPI2\tPAPI3" << endl;
  f << r.TEST_SIZE << "\t"
    << r.time/r.TEST_RUNS << "\t"
    << r.page_faults/r.TEST_RUNS << "\t"
    << r.PAPI1/r.TEST_RUNS << "\t"
    << r.PAPI2/r.TEST_RUNS << "\t"
    << r.PAPI3/r.TEST_RUNS << "\t"
    << endl;
  
}

//inserts TEST_SIZE elements into the prio queues TEST_RUNS times
rrr test_insert(size_t TEST_SIZE, size_t TEST_RUNS, bool is_random) {
  test::pagefaults pf;
  test::random r;
  test::clock c;
  int events[3] = {PAPI_BR_CN, PAPI_L2_DCA, PAPI_L2_DCM};
  long long values[3];
  test::PAPI p(events, values, 3);

  vector<int> data;
  data.resize(TEST_SIZE);
  if (is_random)
    for (size_t i = 0; i < TEST_SIZE; i++) data[i] = r.next(TEST_MAX);
  else
    for (size_t i = 0; i < TEST_SIZE; i++) data[i] = TEST_SIZE-i;

  long long bh_clock = 0, fq_clock = 0, veb_clock = 0;
  long long bh_papi[3] = {0}, fq_papi[3] = {0}, veb_papi[3] = {0};
  uint64_t bh_pf = 0, fq_pf = 0, veb_pf = 0;

  for (size_t i = 0; i < TEST_RUNS; i++) {

    pq::binary_heap bh(TEST_SIZE);
    pq::fibonacci_queue fq(TEST_SIZE);
    pq::van_emde_boas veb;
    test::drop_cache();
    p.start();
    c.start();
    pf.start();
    for (size_t j = 0; j < TEST_SIZE; j++) fq.push(ii(data[j],j));
    p.stop();
    c.stop();
    pf.stop();
    fq_clock+=c.count();
    fq_papi[0] += values[0];
    fq_papi[1] += values[1];
    fq_papi[2] += values[2];
    fq_pf += pf.count();
    test::drop_cache();
    p.start();
    c.start();
    pf.start();
    for (size_t j = 0; j < TEST_SIZE; j++) bh.push(ii(data[j],j));
    p.stop();
    c.stop();
    pf.stop();
    bh_clock+=c.count();
    bh_papi[0] += values[0];
    bh_papi[1] += values[1];
    bh_papi[2] += values[2];
    bh_pf += pf.count();
    test::drop_cache();
    p.start();
    c.start();
    pf.start();
    for (size_t j = 0; j < TEST_SIZE; j++) veb.push(ii(data[j],j));
    p.stop();
    c.stop();
    pf.stop();
    veb_clock+=c.count();
    veb_papi[0] += values[0];
    veb_papi[1] += values[1];
    veb_papi[2] += values[2];
    veb_pf += pf.count();
  }
  // results(long long _time,
  //         uint64_t _page_faults,
  //         size_t _TEST_SIZE, size_t _TEST_RUNS,
  //         size_t _PAPI1, size_t _PAPI2, size_t _PAPI3)
  return rrr( results(bh_clock, bh_pf, TEST_SIZE, TEST_RUNS, bh_papi[0], bh_papi[1], bh_papi[2]),
              rr(results(fq_clock, fq_pf, TEST_SIZE, TEST_RUNS, fq_papi[0], fq_papi[1], fq_papi[2]),
                 results(veb_clock, veb_pf, TEST_SIZE, TEST_RUNS, veb_papi[0], veb_papi[1], veb_papi[2])));
}

//test powers of 2
void test_ins() {
  //random:
  for (size_t i = 0; i < 24; i++) {
    size_t s = (1<<i);
    cout << "TESTING INSERTS RANDOMLY ON SIZE: " << s << endl;
    rrr temp = test_insert(s,10,true);
    print_results(temp.first, "res2/inserts/bh_random.dat");
    print_results(temp.second.first, "res2/inserts/fq_random.dat");
    print_results(temp.second.second, "res2/inserts/veb_random.dat");
  }

  //worst case:
  for (size_t i = 0; i < 24; i++) {
    size_t s = (1<<i);
    cout << "TESTING INSERTS WORST CASE ON SIZE: " << s << endl;
    rrr temp = test_insert(s,10,false);
    print_results(temp.first, "res2/inserts/bh_worst.dat");
    print_results(temp.second.first, "res2/inserts/fq_worst.dat");
    print_results(temp.second.second, "res2/inserts/veb_worst.dat");
  }
  
}

//measure total cycles
rrr test_findmin(size_t TEST_SIZE, size_t TEST_RUNS) {
  pq::binary_heap bh(TEST_SIZE);
  pq::fibonacci_queue fq(TEST_SIZE);
  pq::van_emde_boas veb;
  for (size_t i = 0 ; i < TEST_SIZE; i++) {
    bh.push(ii(i,i));
    fq.push(ii(i,i));
    veb.push(ii(i,i));
  }
  int events[2] = {PAPI_TOT_CYC, PAPI_BR_CN};
  long long values[2];
  test::PAPI p(events, values, 2);
  long long bh_tc = 0, fq_tc = 0, veb_tc = 0;
  long long bh_br = 0, fq_br = 0, veb_br = 0;
  for (size_t i = 0; i < TEST_RUNS; i++) {
    p.start();
    bh.top();
    p.stop();
    bh_tc += values[0];
    bh_br += values[1];
    p.start();
    fq.top();
    p.stop();
    fq_tc += values[0];
    fq_br += values[1];
    p.start();
    veb.top();
    p.stop();
    veb_tc += values[0];
    veb_br += values[1];
  }

  return rrr( results(0, 0, TEST_SIZE, TEST_RUNS, bh_tc, bh_br, 0),
              rr(results(0, 0, TEST_SIZE, TEST_RUNS, fq_tc, fq_br, 0),
                 results(0, 0, TEST_SIZE, TEST_RUNS, veb_tc, veb_br, 0)));

}

void test_fm() {
  for (size_t i = 0; i < 24; i++) {
    size_t s = (1<<i);
    rrr temp = test_findmin(s, 100);
    print_results(temp.first, "res2/findmin/bh.dat");
    print_results(temp.second.first, "res2/findmin/fq.dat");
    print_results(temp.second.second, "res2/findmin/veb.dat");
  }

}

rrr test_decrease_key(size_t TEST_SIZE, size_t TEST_RUNS, size_t NUM_DECREASE_KEY, bool is_random) {
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

  int events[3] = {PAPI_BR_CN, PAPI_L2_DCA, PAPI_L2_DCM};
  long long values[3];
  test::PAPI p(events, values, 3);
  test::clock c;
  test::pagefaults pf;
  long long bh_clock = 0, fq_clock = 0, veb_clock = 0;
  long long bh_papi[3] = {0}, fq_papi[3] = {0}, veb_papi[3] = {0};
  uint64_t bh_pf = 0, fq_pf = 0, veb_pf = 0;

  for (size_t i = 0; i < TEST_RUNS; i++) {
    pq::binary_heap bh(TEST_SIZE);
    pq::fibonacci_queue fq(TEST_SIZE);
    pq::van_emde_boas veb;
    for (size_t j = 0; j < TEST_SIZE; j++) {
      bh.push(ii(bh_data[j], j));
      fq.push(ii(fq_data[j], j));
      veb.push(ii(bh_data[j], j));
    }
    //pop to make sure fibonacci queue has consolidated!
    bh.pop(); fq.pop(); veb.pop();
    //first decrease key randomly in the heap
    pf.start();
    c.start(); p.start();
    for (size_t j = 0; j < NUM_DECREASE_KEY; j++) {
      bh.decrease_key(dk[j].first, dk[j].second);
    }
    p.stop(); c.stop(); pf.stop();
    bh_clock += c.count();
    bh_papi[0] += values[0];
    bh_papi[1] += values[1];
    bh_papi[2] += values[2];
    bh_pf += pf.count();

    pf.start();
    c.start(); p.start();
    for (size_t j = 0; j < NUM_DECREASE_KEY; j++) {
      fq.decrease_key(dk[j].first, dk[j].second);
    }
    p.stop(); c.stop(); pf.stop();
    fq_clock += c.count();
    fq_papi[0] += values[0];
    fq_papi[1] += values[1];
    fq_papi[2] += values[2];
    fq_pf += pf.count();

    pf.start();
    c.start(); p.start();
    for (size_t j = 0; j < NUM_DECREASE_KEY; j++) {
      veb.decrease_key(veb.maximum, veb.minimum+j+1);
    }
    veb_clock += c.count();
    veb_papi[0] += values[0];
    veb_papi[1] += values[1];
    veb_papi[2] += values[2];
    veb_pf += pf.count();

  }


  return rrr( results(bh_clock, bh_pf, TEST_SIZE, TEST_RUNS, bh_papi[0], bh_papi[1], bh_papi[2]),
              rr(results(fq_clock, fq_pf, TEST_SIZE, TEST_RUNS, fq_papi[0], fq_papi[1], fq_papi[2]),
                 results(veb_clock, veb_pf, TEST_SIZE, TEST_RUNS, veb_papi[0], veb_papi[1], veb_papi[2])));

}
//test_decrease_key(size_t TEST_SIZE, size_t TEST_RUNS, size_t NUM_DECREASE_KEY, bool is_random) {
void test_dk() {

  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING on input size: " << s << " where we decrease 1 element at random" << endl;
    rrr temp = test_decrease_key(s, 10, 1, true);
    print_results(temp.first, "res2/dk/ndk_random_bh_1.dat");
    print_results(temp.second.first, "res2/dk/ndk_random_fq_1.dat");
    print_results(temp.second.second, "res2/dk/ndk_random_veb_1.dat");
  }

  return;
  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING on input size: " << s << " where we decrease 1 element at worst" << endl;
    rrr temp = test_decrease_key(s, 10, 1, false);
    print_results(temp.first, "res2/dk/ndk_worst_bh_1.dat");
    print_results(temp.second.first, "res2/dk/ndk_worst_fq_1.dat");
    print_results(temp.second.second, "res2/dk/ndk_worst_veb_1.dat");
  }

  size_t TEST_SIZE = (1<<15);
  //random, vary on num of decrease key operations!
  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " DECREASE KEYS randomly" << endl;
    rrr temp = test_decrease_key(TEST_SIZE, 10, s, true);
    print_results(temp.first, "res2/dk/ndk_random_bh.dat");
    print_results(temp.second.first, "res2/dk/ndk_random_fq.dat");
    print_results(temp.second.first, "res2/dk/ndk_random_veb.dat");
  }

  //worst case, vary on num of decrease key operations
  for (size_t i = 0; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " DECREASE KEYS worst case" << endl;
    rrr temp = test_decrease_key(TEST_SIZE, 10, s, false);
    print_results(temp.first, "res2/dk/ndk_worst_bh.dat");
    print_results(temp.second.first, "res2/dk/ndk_worst_fq.dat");
    print_results(temp.second.second, "res2/dk/ndk_worst_veb.dat");
  }

  

}

rrr test_deletemin(size_t TEST_SIZE, size_t TEST_RUNS, size_t NUM_DELETES, bool is_random) {
  vector<int> bh_data, fq_data;
  bh_data.resize(TEST_SIZE);
  fq_data.resize(TEST_SIZE);
  //generate data
  test::random r;
  if (is_random) {
    for (size_t i = 0; i < TEST_SIZE; i++)
      bh_data[i] = fq_data[i] = r.next(TEST_MAX);
  } else {
    for (size_t i = 0; i < TEST_SIZE; i++) {
      bh_data[i] = i;
      fq_data[i] = TEST_SIZE-i;
    }
  }

  int events[3] = {PAPI_BR_CN, PAPI_L2_DCA, PAPI_L2_DCM};
  long long values[3];
  test::PAPI p(events, values, 3);
  test::clock c;
  test::pagefaults pf;
  long long bh_clock = 0, fq_clock = 0, veb_clock = 0;
  long long bh_papi[3] = {0}, fq_papi[3] = {0}, veb_papi[3] = {0};
  uint64_t bh_pf = 0, fq_pf = 0, veb_pf = 0;

  for (size_t t = 0; t < TEST_RUNS; t++) {
    pq::binary_heap bh(TEST_SIZE);
    pq::fibonacci_queue fq(TEST_SIZE);
    pq::van_emde_boas veb;
    for (size_t i = 0; i < TEST_SIZE; i++) {
      bh.push(ii(bh_data[i], i));
      fq.push(ii(fq_data[i], i));
      veb.push(ii(bh_data[i],i));
    }
    // pop one element
    if (NUM_DELETES > 1) { bh.pop(); fq.pop(); veb.pop();}
    pf.start();
    c.start(); p.start();
    for (size_t i = 0; i < (size_t)max(1,((int)NUM_DELETES)-1); i++) {
      bh.pop();
    }
    p.stop(); c.stop(); pf.stop();
    bh_clock += c.count();
    bh_papi[0] += values[0];
    bh_papi[1] += values[1];
    bh_papi[2] += values[2];
    bh_pf += pf.count();
    
    c.start(); p.start(); pf.start();
    for (size_t i = 0; i < max((size_t)1,(NUM_DELETES)-1); i++) {
      fq.pop();
    }
    p.stop(); c.stop(); pf.stop();
    fq_clock += c.count();
    fq_papi[0] += values[0];
    fq_papi[1] += values[1];
    fq_papi[2] += values[2];
    fq_pf += pf.count();

    c.start(); p.start(); pf.start();
    for (size_t i = 0; i < max((size_t)1,(NUM_DELETES)-1); i++) {
      veb.pop();
    }
    p.stop(); c.stop(); pf.stop();
    veb_clock += c.count();
    veb_papi[0] += values[0];
    veb_papi[1] += values[1];
    veb_papi[2] += values[2];
    veb_pf += pf.count();
  }
  
  return rrr( results(bh_clock, bh_pf, TEST_SIZE, TEST_RUNS, bh_papi[0], bh_papi[1], bh_papi[2]),
              rr(results(fq_clock, fq_pf, TEST_SIZE, TEST_RUNS, fq_papi[0], fq_papi[1], fq_papi[2]),
                 results(veb_clock, veb_pf, TEST_SIZE, TEST_RUNS, veb_papi[0], veb_papi[1], veb_papi[2])));
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

  for (size_t i = 1; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " with " << 1 << " DELETE worst data" << endl;
    rrr temp = test_deletemin(s, 10, 1, false);
    print_results(temp.first, "res2/delmin/bh_worst_1.dat");
    print_results(temp.second.first, "res2/delmin/fq_worst_1.dat");
    print_results(temp.second.second, "res2/delmin/veb_worst_1.dat");
  }

  // for (size_t i = 1; i < 22; i++) {
  //   size_t s = (1<<i);
  //   cout << "TESTING : " << s << " with " << 1 << " DELETE random data" << endl;
  //   rrr temp = test_deletemin(s, 5, 1, true);
  //   print_results(temp.first, "res2/delmin/random_del_1_bh.dat");
  //   print_results(temp.second.first, "res2/delmin/random_del_1_fq.dat");
  //   print_results(temp.second.second, "res2/delmin/random_del_1_veb.dat");
  // }

  for (size_t i = 1; i < 23; i++) {
    size_t s = (1<<i);
    cout << "TESTING : " << s << " with " << 2 << " DELETE worst data" << endl;
    rrr temp = test_deletemin(s, 10, 2, false);
    print_results(temp.first, "res2/delmin/bh_worst_2.dat");
    print_results(temp.second.first, "res2/delmin/fq_worst_2.dat");
    print_results(temp.second.second, "res2/delmin/veb_worst_2.dat");
  }

  // for (size_t i = 1; i < 22; i++) {
  //   size_t s = (1<<i);
  //   cout << "TESTING : " << s << " with " << 2 << " DELETE random data" << endl;
  //   rrr temp = test_deletemin(s, 5, 2, true);
  //   print_results(temp.first, "res2/delmin/random_del_2_bh.dat");
  //   print_results(temp.second.first, "res2/delmin/random_del_2_fq.dat");
  //   print_results(temp.second.second, "res2/delmin/random_del_2_veb.dat");
  // }
}

int main() {

  //test_ins();
  //test_fm();
  //test_dk(); //TODO THIS!
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
