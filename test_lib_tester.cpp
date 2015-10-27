#include "test_lib.hpp"
#include <unistd.h>
#include <iostream>
#include <papi.h>

using namespace std;

void test_clock() {
  test::clock c;
  c.start();
  sleep(1);
  c.stop();
  cout << c.count() << endl;
}

void test_papi() {
  int events[2] = {PAPI_TOT_CYC, PAPI_BR_CN};
  long long values[2];
  test::PAPI p(events, values, 2);
  p.start();
  sleep(1);
  p.stop();
  cout << values[0] << " " << values[1] << endl;

}

void test_pf() {
  test::pagefaults pf;
  pf.start();
  for (int i = 0; i < 100; i++) {i++; i--;}
  pf.stop();
  cout << pf.count() << endl;
}

void test_random() {
  test::random r;
  cout << r.next() << " " << r.next() << endl;
  for (int i = 0; i < 100; i++)
    cout << r.next() << endl;
}

void test_drop_cache() {
  test::drop_cache();
}

int main() {

  test_clock();
  test_papi();
  test_pf();
  test_random();
  test_drop_cache();

  return 0;
}
