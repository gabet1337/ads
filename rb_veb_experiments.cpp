#include "van_emde_boas.hpp"
#include "rb_tree.hpp"
#include "predecessor_queue.hpp"
#include "test_lib.hpp"
#include <iostream>

using namespace std;

struct results {
  long long time, papi, papi1, papi2;
  uint64_t page_faults;
  size_t TEST_SIZE, TEST_RUNS, NUM_DECREASE_KEYS;
  bool is_random;
  results(long long _time, long long _papi, uint64_t _page_faults,
          size_t _TEST_SIZE, size_t _TEST_RUNS, bool _is_random, size_t _NUM_DECREASE_KEYS,
	  long long _papi1, long long _papi2)
    : time(_time), papi(_papi), papi1(_papi1), papi2(_papi2), page_faults(_page_faults),
      TEST_SIZE(_TEST_SIZE), TEST_RUNS(_TEST_RUNS), NUM_DECREASE_KEYS(_NUM_DECREASE_KEYS),
      is_random(_is_random)
  { }
};

void print_results(results r, string file) {
  // long long time, papi;
  // uint64_t page_faults;
  // size_t TEST_SIZE, TEST_RUNS;
  // bool is_random;
  ofstream f(file, ios::app|ios::out);
  f << "#size\ttime\tpapi\tpfs\tndk\ttca\ttcm" << endl;
  f << r.TEST_SIZE << "\t"
    << r.time/r.TEST_RUNS << "\t"
    << r.papi/r.TEST_RUNS << "\t"
    << r.page_faults/r.TEST_RUNS << "\t"
    << r.NUM_DECREASE_KEYS << "\t"
    << r.papi1/r.TEST_RUNS << "\t"
    << r.papi2/r.TEST_RUNS
    << endl;
  
}

pair<results,results> make_queue(size_t TEST_RUNS, size_t TEST_SIZE) {

  test::pagefaults pf;
  test::clock c;
  int events[1] = {PAPI_TOT_CYC};
  long long values[1];
  test::PAPI p(events, values, 1);
  
  long long rb_clock = 0, veb_clock = 0;
  long long rb_br = 0, veb_br = 0;
  uint64_t rb_pf = 0, veb_pf = 0;

  for (size_t i = 0; i < TEST_RUNS; i++) {

    test::drop_cache();
    p.start();
    c.start();
    pf.start();
    pq::rb_tree *rb = new pq::rb_tree();
    pf.stop();
    c.stop();
    p.stop();
    rb_clock+=c.count();
    rb_br += values[0];
    rb_pf += pf.count();
    delete rb;

    test::drop_cache();
    p.start();
    c.start();
    pf.start();
    pq::van_emde_boas *veb = new pq::van_emde_boas(TEST_SIZE);
    pf.stop();
    c.stop();
    p.stop();
    veb_clock+=c.count();
    veb_br += values[0];
    veb_pf += pf.count();
    delete veb;
    
  }

  cout << "\tRB\tvEB" << endl;
  cout << "hc\t" << rb_br / TEST_RUNS << "\t" << veb_br / TEST_RUNS << endl;
  cout << "ti\t" << rb_clock / TEST_RUNS << "\t" << veb_clock /TEST_RUNS << endl;
  cout << "pf\t" << rb_pf / TEST_RUNS << "\t" << veb_pf /TEST_RUNS << endl;
  //cout << "dk\t" << bh_dk / TEST_RUNS << "\t" << fq_dk / TEST_RUNS << endl;
  
  return { results(rb_clock, rb_br, rb_pf, TEST_SIZE, TEST_RUNS, false, 0, 0, 0),
      results(veb_clock, veb_br, veb_pf, TEST_SIZE, TEST_RUNS, false, 0, 0, 0)};
  
}

void test_make_queue_pow_two() {
  for (size_t i = 3; i <= 24; i++) {
    size_t test_size = (1<<i);
    pair<results,results> res = make_queue(50,test_size);
    print_results(res.first, "res/rbveb/make_queue_pow_two_rb.dat");
    print_results(res.second, "res/rbveb/make_queue_pow_two_veb.dat");    
  }
}

void test_make_queue_pow_two_minus_one() {
  for (size_t i = 3; i <= 24; i++) {
      size_t test_size = (1<<i)-1;
      pair<results,results> res = make_queue(50,test_size);
      print_results(res.first, "res/rbveb/make_queue_pow_two_minus_one_rb.dat");
      print_results(res.second, "res/rbveb/make_queue_pow_two_minus_one_veb.dat");      
  } 
}

pair<results,results> find_min(size_t TEST_RUNS, size_t TEST_SIZE) {

  test::pagefaults pf;
  test::clock c;
  int events[2] = {PAPI_BR_CN, PAPI_TOT_CYC};
  long long values[2];
  test::PAPI p(events, values, 2);
  
  long long rb_clock = 0, veb_clock = 0;
  long long rb_br = 0, veb_br = 0;
  uint64_t rb_pf = 0, veb_pf = 0;
  long long rb_cyc = 0, veb_cyc = 0;
  
  test::random rand;
  
  for (size_t i = 0; i < TEST_RUNS; i++) {
    
    test::drop_cache();
    predecessor_queue *rb = new pq::rb_tree();
    predecessor_queue *veb = new pq::van_emde_boas(16777216);

    for (size_t i = 0; i < TEST_SIZE; i++) {
      int r = rand.next(16777216);
      rb->insert(r);
      veb->insert(r);
    }

    p.start();
    c.start();
    pf.start();

    for (int i = 0; i < 1000; i++) {
      rb->find_min();
    }
    
    pf.stop();
    c.stop();
    p.stop();
    rb_clock+=c.count();
    rb_br += values[0];
    rb_cyc += values[1];
    rb_pf += pf.count();
    delete rb;

    p.start();
    c.start();
    pf.start();

    for (int i = 0; i < 1000; i++) {
      veb-> find_min();
    }
    
    pf.stop();
    c.stop();
    p.stop();
    veb_clock+=c.count();
    veb_br += values[0];
    veb_cyc += values[1];
    veb_pf += pf.count();
    delete veb;
    
  }

  cout << "\tRB\tvEB" << endl;
  cout << "br\t" << rb_br / TEST_RUNS << "\t" << veb_br / TEST_RUNS << endl;
  cout << "cyc\t" << rb_cyc / TEST_RUNS << "\t" << veb_cyc / TEST_RUNS << endl;
  cout << "ti\t" << rb_clock / TEST_RUNS << "\t" << veb_clock /TEST_RUNS << endl;
  cout << "pf\t" << rb_pf / TEST_RUNS << "\t" << veb_pf /TEST_RUNS << endl;
  //cout << "dk\t" << bh_dk / TEST_RUNS << "\t" << fq_dk / TEST_RUNS << endl;
  
  return { results(rb_clock, rb_br, rb_pf, TEST_SIZE, TEST_RUNS, false, 0, rb_cyc, 0),
      results(veb_clock, veb_br, veb_pf, TEST_SIZE, TEST_RUNS, false, 0, veb_cyc, 0)};
  
}

void test_find_min() {
  for (size_t i = 3; i <= 24; i++) {
    size_t test_size = (1<<i);
    pair<results,results> res = find_min(10,test_size);
    print_results(res.first, "res/rbveb/find_min_rb.dat");
    print_results(res.second, "res/rbveb/find_min_veb.dat");    
  }
}

pair<results,results> insert(size_t TEST_RUNS, size_t TEST_SIZE) {

  test::pagefaults pf;
  test::clock c;
  int events[3] = {PAPI_BR_CN, PAPI_L2_TCA, PAPI_L2_TCM};
  long long values[3];
  test::PAPI p(events, values, 3);
  
  long long rb_clock = 0, veb_clock = 0;
  long long rb_br = 0, veb_br = 0;
  long long rb_tca = 0, veb_tca = 0;
  long long rb_tcm = 0, veb_tcm = 0;
  uint64_t rb_pf = 0, veb_pf = 0;

  test::random rand;
  
  for (size_t i = 0; i < TEST_RUNS; i++) {
    
    test::drop_cache();
    predecessor_queue *rb = new pq::rb_tree();

    p.start();
    c.start();
    pf.start();

    for (size_t i = 0; i < TEST_SIZE; i++) {
      int r = rand.next(16777216);
      rb->insert(r);
    }

    pf.stop();
    c.stop();
    p.stop();
    rb_clock+=c.count();
    rb_br += values[0];
    rb_tca += values[1];
    rb_tcm += values[2];
    rb_pf += pf.count();

    delete rb;

    predecessor_queue *veb = new pq::van_emde_boas(16777216);

    p.start();
    c.start();
    pf.start();

    for (size_t i = 0; i < TEST_SIZE; i++) {
      int r = rand.next(16777216);
      veb->insert(r);
    }
    
    pf.stop();
    c.stop();
    p.stop();
    veb_clock+=c.count();
    veb_br += values[0];
    veb_tca += values[1];
    veb_tcm += values[2];
    veb_pf += pf.count();

    delete veb;
    
  }

  cout << "\tRB\tvEB" << endl;
  cout << "br\t" << rb_br / TEST_RUNS << "\t" << veb_br / TEST_RUNS << endl;
  cout << "tca\t" << rb_tca / TEST_RUNS << "\t" << veb_tca / TEST_RUNS << endl;
  cout << "tcm\t" << rb_tcm / TEST_RUNS << "\t" << veb_tcm / TEST_RUNS << endl;
  cout << "ti\t" << rb_clock / TEST_RUNS << "\t" << veb_clock /TEST_RUNS << endl;
  cout << "pf\t" << rb_pf / TEST_RUNS << "\t" << veb_pf /TEST_RUNS << endl;
  //cout << "dk\t" << bh_dk / TEST_RUNS << "\t" << fq_dk / TEST_RUNS << endl;
  
  return { results(rb_clock, rb_br, rb_pf, TEST_SIZE, TEST_RUNS, false, 0, rb_tca, rb_tcm),
      results(veb_clock, veb_br, veb_pf, TEST_SIZE, TEST_RUNS, false, 0, veb_tca, veb_tcm)};
  
}

void test_insert() {
  for (size_t i = 3; i <= 24; i++) {
    size_t test_size = (1<<i);
    pair<results,results> res = insert(10,test_size);
    print_results(res.first, "res/rbveb/insert_rb.dat");
    print_results(res.second, "res/rbveb/insert_veb.dat");    
  }
}

int main() {
  //test_make_queue_pow_two();
  //test_make_queue_pow_two_minus_one();
  test_find_min();
  //test_insert();
}
