#include "van_emde_boas.hpp"
#include <iostream>
using namespace std;

void test() {
  pq::van_emde_boas veb;
  veb.insert(10);
  veb.insert(5);
  veb.insert(15);
  cout << veb.minimum << " " << veb.maximum << endl;
  veb.delete_min();
  cout << veb.minimum << " " << veb.maximum << endl;
  veb.delete_min();
  cout << veb.minimum << " " << veb.maximum << endl;
  veb.delete_min();
  cout << veb.minimum << " " << veb.maximum << endl;
  int x;
  cin >> x;
}

int main() {
  test();
  int x;
  cin >> x;

  return 0;
}
