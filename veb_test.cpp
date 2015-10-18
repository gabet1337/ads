#include "van_emde_boas.hpp"
#include <iostream>
using namespace std;

void test() {
  pq::van_emde_boas veb;
  cout << veb.predecessor(6) << endl;
  veb.insert(10);
  veb.insert(5);
  veb.insert(15);
  cout << "10 is a member? " << veb.member(10) << endl;
  cout << "7 is a member? " << veb.member(7) << endl;
  cout << "predecessor of 11? " << veb.predecessor(11) << endl;
  cout << "predecessor of 4? " << veb.predecessor(4) << endl;
  cout << veb.minimum << " " << veb.maximum << endl;
  veb.delete_min();
  cout << veb.minimum << " " << veb.maximum << endl;
  veb.delete_min();
  cout << veb.minimum << " " << veb.maximum << endl;
  veb.delete_min();
  cout << veb.minimum << " " << veb.maximum << endl;
}

int main() {
  test();
  return 0;
}
