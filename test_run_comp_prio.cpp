#include "test_lib.hpp"
#include <papi.h>
#include <iostream>
#include "binary_heap.hpp"
#include "fibonacci_queue.hpp"
#include <vector>
using namespace std;
typedef pair<int,int> ii;
//inserts TEST_SIZE random elements into the prio queues TEST_RUNS times
void test_insert_random(size_t TEST_SIZE, size_t TEST_RUNS) {

  test::random r;
  test::clock c;
  int events[1] = {PAPI_BR_MSP};
  long long values[1];
  test::PAPI p(events, values, 1);

  vector<int> data;
  data.resize(TEST_SIZE);
  for (size_t i = 0; i < TEST_SIZE; i++) data[i] = r.next();

  long long bh_clock = 0, fq_clock = 0;
  long long bh_br = 0, fq_br = 0;
  for (size_t i = 0; i < TEST_RUNS; i++) {

    pq::binary_heap bh(TEST_SIZE);
    pq::fibonacci_queue fq(TEST_SIZE);

    p.start();
    c.start(); 
    for (size_t j = 0; j < TEST_SIZE; j++) fq.push(ii(data[j],j));
    c.stop();
    p.stop();
    fq_clock+=c.count();
    fq_br += values[0];


    c.start(); p.start();
    //first insert into binary_heap:
    for (size_t j = 0; j < TEST_SIZE; j++) bh.push(ii(data[j],j));
    c.stop(); p.stop();
    bh_clock+=c.count();
    bh_br += values[0];

  }
  cout << bh_br << " " << fq_br << endl;
  cout << bh_clock << " " << fq_clock << endl;
}


int main() {

  test_insert_random(100000, 100);

  return 0;
}
