#include <iostream>
#include "binary_heap.hpp"

using namespace std;

void test(priority_queue<pair<int,int> > &pq) {
  pq.push(make_pair(0,0));
  cout << pq.top().first << endl;
}

int main() {
  pq::binary_heap<pair<int,int> > pq;
  test(pq);
  return 0;
}
