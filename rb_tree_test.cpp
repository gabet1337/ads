#include "rb_tree.hpp"
#include <iostream>
using namespace std;

void print_test() {
  pq::rb_tree rb;
  for (int i = 1; i < 33; i++) rb.insert(i);
  rb.print();
}

void test_rb(pq::rb_tree &rb) {

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

}

void test_pred_queue(predecessor_queue &rb) {

  rb.insert(100);
  rb.insert(1000);
  rb.insert(213);
  rb.insert(12313);
  cout << "empty ? " << rb.empty() << endl;
  cout << "pre of 12313 " << rb.predecessor(12313) << endl;
  cout << "pre of 100 " << rb.predecessor(100) << endl;
  rb.insert(2);
  rb.delete_min();
  cout << rb.find_min() << endl;
  rb.delete_key(100);
  cout << rb.find_min() << endl;
  rb.delete_key(12313);
  cout << rb.find_min() << endl;

  rb.delete_min();
  cout << rb.size() << endl;
  rb.delete_min();
  
  cout << "empty ? " << rb.empty() << endl;
}


int main() {
  print_test();
  return 0;
  pq::rb_tree rb;

  test_rb(rb);
  test_pred_queue(rb);
  
  return 0;
}
