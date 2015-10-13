#include <iostream>
#include "fibonacci_heap.hpp"

using namespace std;

int main(int argc, char* argv[]) {

  pq::fibonacci_heap fh;
  //cout << fh.size() << endl;
  fh.push(std::make_pair(10, 10));
  //cout << fh.size() << endl;
  fh.push(std::make_pair(20, 20));
  //cout << fh.top() << endl;
  fh.push(std::make_pair(30,30));
  fh.push(std::make_pair(40,40));
  fh.pop();
  //fh.pop();
  //cout << fh.top() << endl;
  fh.push(std::make_pair(5,5));
  fh.push(std::make_pair(80,80));
  fh.push(std::make_pair(8,8));
  fh.push(std::make_pair(860,860));
  fh.push(std::make_pair(56,56));
  fh.push(std::make_pair(820,820));
  //cout << fh.top() << endl;

  fh.pop();

  //cout << fh.top() << endl;

  //cout << fh.empty() << endl;

  //while (!fh.empty())
  //  fh.pop();

  //cout << fh.empty() << endl;
}
