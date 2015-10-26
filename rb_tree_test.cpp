#include "rb_tree.hpp"
#include <iostream>
using namespace std;

int main() {

  pq::rb_tree rb;
  rb.insert(100);
  rb.insert(1000);
  rb.insert(213);
  rb.insert(12313);
  cout << "suc of 99 " << rb.successor(99) << endl;
  cout << "suc of 100 " << rb.successor(100) << endl;
  cout << "suc of 12313 " << rb.successor(12313) << endl;
  cout << "pre of 12313 " << rb.predecessor(12313) << endl;
  cout << "pre of 100 " << rb.predecessor(100) << endl;
  cout << rb.minimum() << " " << rb.maximum() << endl;
  rb.erase(100);
  cout << rb.minimum() << " " << rb.maximum() << endl;
  rb.erase(12313);
  cout << rb.minimum() << " " << rb.maximum() << endl;

  return 0;
}
