#include <iostream>
#include "fibonacci_heap.hpp"

using namespace std;

int main(int argc, char* argv[]) {

  fibonnaci_heap<int> fh;
  fh.push(10);
  fh.push(20);
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
