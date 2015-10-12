#ifndef FIBONACCI_QUEUE_HPP
#define FIBONACCI_QUEUE_HPP

#include "priority_queue.hpp"
#include "fibonacci_heap.hpp"
#include <unordered_map>
#include <iostream>
#include <vector>

namespace pq {
  class fibonacci_queue : public priority_queue {
	
  public:

    fibonacci_queue(std::size_t _max_size);
    ~fibonacci_queue();

    void push(std::pair<int,int> k);
    void pop();
    void decrease_key(int v, int k);
    std::pair<int,int> top();
    bool empty();
    std::size_t size();

  private:
    
    std::vector<fibonacci_heap::Node*> lookup;
    fibonacci_heap fib_heap;

  };

  
  fibonacci_queue::fibonacci_queue(size_t _max_size) {
    lookup.resize(_max_size, nullptr);
  }
  
  
  fibonacci_queue::~fibonacci_queue()
  {}

  
  void fibonacci_queue::push(std::pair<int,int> k) {
    fibonacci_heap::Node *x = fib_heap.push(k);
    lookup[k.second] = x;
  }
 
  
  void fibonacci_queue::pop() {

    if (empty())
      return;

    typename fibonacci_heap::Node *x = fib_heap.DeleteMin();
    lookup[x->key.second] = nullptr;
    delete x;
  }

  
  void fibonacci_queue::decrease_key(int v, int k) {

    typename fibonacci_heap::Node *x = lookup[v];

    if (x != nullptr)
      fib_heap.DecreaseKey(lookup[v],std::make_pair(k,v));
    else
      push(std::make_pair(k,v));
  }

  
  std::pair<int,int> fibonacci_queue::top() {
    return fib_heap.FindMin()->key;
  }

  
  bool fibonacci_queue::empty() {
    return size() == 0;
  }

  
  std::size_t fibonacci_queue::size() {
    return fib_heap.size();
  }
  
}
#endif
