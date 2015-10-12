#include <iostream>
#include "binary_heap.hpp"

using namespace std;

void test(priority_queue &pq) {
  pq.push(make_pair(1,1));
  pq.push(make_pair(2,0));
  pq.push(make_pair(0,2));
  cout << pq.top().second << endl;
  pq.decrease_key(1,0);
  cout << pq.top().second << endl;
  pq.pop();
  cout << pq.top().second << endl;
  pq.pop();
  cout << pq.top().second << endl;
}

int main() {
  pq::binary_heap pq(100);
  test(pq);
  return 0;
}
