#ifndef FIBONACCI_QUEUE_HPP
#define FIBONACCI_QUEUE_HPP

#include "priority_queue.hpp"
#include "fibonacci_heap.hpp"
#include <vector>

typedef std::pair<int,int> ii;

namespace pq {
  class fibonacci_queue : public priority_queue {
	
  public:

    fibonacci_queue(std::size_t _max_size);
    ~fibonacci_queue();

    void push(ii k);
    void pop();
    void decrease_key(int v, int k);
    ii top();
    bool empty();
    std::size_t size();
    void print();

  private:
    
    std::vector<fibonacci_heap::Node*> lookup;
    fibonacci_heap fib_heap;

  };

  
  fibonacci_queue::fibonacci_queue(size_t _max_size) {
    lookup.resize(_max_size, nullptr);
  }
  
  
  fibonacci_queue::~fibonacci_queue() {
    lookup.clear();
    lookup.shrink_to_fit();
  }

  
  void fibonacci_queue::push(ii k) {
    fibonacci_heap::Node *x = fib_heap.push(k);
    lookup[k.second] = x;
  }
 
  
  void fibonacci_queue::pop() {
    if (empty()) return;

    typename fibonacci_heap::Node *x = fib_heap.DeleteMin();
    lookup[x->key.second] = nullptr;
    delete x;
  }

  
  void fibonacci_queue::decrease_key(int v, int k) {

    typename fibonacci_heap::Node *x = lookup[v];

    if (x != nullptr)
      fib_heap.DecreaseKey(lookup[v],ii(k,v));
    else
      push(ii(k,v));
  }

  
  ii fibonacci_queue::top() {
    return fib_heap.FindMin()->key;
  }

  
  bool fibonacci_queue::empty() {
    return size() == 0;
  }

  
  std::size_t fibonacci_queue::size() {
    return fib_heap.size();
  }

  void fibonacci_queue::print() {
    fib_heap.print();
  }
  
}
#endif
