#include <iostream>
#include "fibonacci_heap.hpp"

using namespace std;

int main(int argc, char* argv[]) {

  pq::fibonacci_heap fh;


  for (size_t i = 0; i < 65; i++) {
    fh.push(std::make_pair(i,i));
  }


  fh.pop();
  

  //cout << fh.top() << endl;



  fh.print();
  
  //cout << fh.top() << endl;

  //cout << fh.empty() << endl;

  //while (!fh.empty())
  //  fh.pop();

  //cout << fh.empty() << endl;
}
