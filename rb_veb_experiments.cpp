#include "van_emde_boas.hpp"
#include "rb_tree.hpp"
#include "test_lib.hpp"
#include <iostream>

using namespace std;

void test_veb(pq::van_emde_boas &veb) {

  cout << veb.size() << endl;
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
  

void test_priority_queue(priority_queue &veb) {

  cout << veb.size() << endl;
  veb.push(make_pair(10,-1));
  veb.push(make_pair(5,-1));
  veb.push(make_pair(15,-1));

  cout << veb.size() << endl;
  
  cout << "minimum? " << veb.top().first << endl;

  veb.decrease_key(10,2);

  cout << "minimum ? " << veb.top().first << endl;
  
  veb.pop();

  cout << "size ? " << veb.size() << endl;
  cout << "minimum ? " << veb.top().first << endl;
  
  cout << "empty ? " << veb.empty() << endl;

  veb.pop();
  veb.pop();
  
  cout << "empty ? " << veb.empty() << endl;
}

void test_predecessor_queue(predecessor_queue &veb) {

  cout << veb.size() << endl;
  veb.insert(10);
  veb.insert(5);
  veb.insert(15);

  cout << veb.size() << endl;
  
  cout << "minimum? " << veb.find_min() << endl;

  veb.delete_min();

  cout << "size ? " << veb.size() << endl;
  cout << "minimum ? " << veb.find_min() << endl;
  
  cout << "empty ? " << veb.empty() << endl;

  veb.delete_key(10);

  cout << "minimum ? " << veb.find_min() << endl;
  
}

int main() {
  pq::van_emde_boas veb;
  //test_veb(veb);
  //test_priority_queue(veb);
  //test_predecessor_queue(veb);
}
