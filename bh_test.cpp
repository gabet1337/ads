#include <iostream>
#include "binary_heap.hpp"

using namespace std;
typedef pair<int,int> ii;
int main() {

  pq::binary_heap bh(64);

  for (int i = 0; i < 63; i++) {
    bh.push(ii(i,i));
  }

  bh.print();

  return 0;
}
