#include <iostream>
#include "binary_heap.hpp"

using namespace std;

int main() {

  pq::binary_heap<int> pq;
  pq.push(5);
  pq.push(10);
  pq.push(15);
  cout << pq.top() << endl;
  pq.pop();
  cout << pq.top() << endl;
  pq.push(100);
  cout << pq.top() << endl;
  return 0;
}
