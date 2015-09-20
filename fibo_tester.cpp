#include <iostream>
#include "fibonacci_heap.hpp"

using namespace std;

int main(int argc, char* argv[]) {

  pq::fibonacci_heap<int> fh;
  cout << fh.size() << endl;
  fh.push(10);
  cout << fh.size() << endl;
  fh.push(20);
  cout << fh.top() << endl;
  fh.pop();
  fh.pop();
  cout << fh.top() << endl;
  fh.push(5);
  fh.push(80);

  cout << fh.top() << endl;

  fh.pop();

  cout << fh.top() << endl;

  cout << fh.empty() << endl;

  while (!fh.empty())
    fh.pop();

  cout << fh.empty() << endl;
}
