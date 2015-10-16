#ifndef BINARY_HEAP_HPP
#define BINARY_HEAP_HPP
#define BUBBLE_COUNT
#include <vector>
#include <iostream>
#include "priority_queue.hpp"
typedef std::pair<int,int> ii;
namespace pq {
  
  class binary_heap : public priority_queue {
  public:
    binary_heap(std::size_t max_size);
    ~binary_heap();
    bool empty();
    std::size_t size();
    ii top();
    void push(ii k);
    void pop();
    void decrease_key( int id, int key );
    #ifdef BUBBLE_COUNT
    size_t bcount;
    #endif
    void print();
  private:
    std::size_t _size, max_size;
    std::vector<ii> container;
    std::vector<int> id_to_index;
    int parent(int i);
    int left(int i);
    int right(int i);
    void min_heapify(int i);
    void bubble_up(int i);
  };

  binary_heap::binary_heap(std::size_t max_size) {
    #ifdef BUBBLE_COUNT
    bcount = 0;
    #endif
    this->max_size = max_size;
    id_to_index.resize(max_size);
    _size = 0;
  }

  binary_heap::~binary_heap() {
    id_to_index.clear();
    id_to_index.shrink_to_fit();
    container.clear();
    container.shrink_to_fit();
  }

  bool binary_heap::empty() {
    return _size == 0;
  }

  std::size_t binary_heap::size() {
    return _size;
  }

  ii binary_heap::top() {
    return container[0];
  }

  void binary_heap::push(ii k) {
    //store the index of the node
    int node = k.second;
    id_to_index[node] = _size++;
    //insert and push up!
    container.push_back(k);
    bubble_up(id_to_index[node]);
  }

  void binary_heap::pop() {
    if (size() < 1) { std::cerr << "cannot pop from empty queue" << std::endl; return; } // throw some error
    container[0] = container[_size-1];
    //update the last elements position to 0
    id_to_index[container[0].second] = 0;
    _size--;
    container.pop_back();
    min_heapify(0);
  }

  int binary_heap::parent(int i) {
    return (i-1)>>1;
  }

  int binary_heap::left(int i) {
    return (i<<1)|1;
  }

  int binary_heap::right(int i) {
    return (i+1)<<1;
  }

  void binary_heap::min_heapify(int i) {
    int l = left(i), r = right(i);
    int smallest;
    if (l < (int)size() && container[l] < container[i]) smallest = l;
    else smallest = i;
    if (r < (int)size() && container[r] < container[smallest]) smallest = r;
    if (smallest != i) {
      //swap the indices of nodes at index i and index smallest
      std::swap(id_to_index[container[i].second], id_to_index[container[smallest].second]);
      std::swap(container[i],container[smallest]);
      min_heapify(smallest);
    }
  }

  void binary_heap::bubble_up(int i) {
    while (i > 0 && container[parent(i)] > container[i]) {
      #ifdef BUBBLE_COUNT
      bcount++;
      #endif
      std::swap(id_to_index[container[i].second], id_to_index[container[parent(i)].second]);
      std::swap(container[i], container[parent(i)]);
      i = parent(i);
    }
  }
  
  void binary_heap::decrease_key(int id, int key) {
    container[id_to_index[id]].first = key;
    bubble_up(id_to_index[id]);
  }

  void binary_heap::print() {
    std::cout << "digraph bh {" << std::endl;
    for (size_t i = 0; i < size(); i++) {
      std::cout << i << " [label=\"" << container[i].first << ", " << container[i].second << "\"]" << std::endl;
    }
    for (int i = 0; i < (int)size(); i++) {
      if (left(i) < (int)size())
        std::cout << i << " -> " << left(i) << std::endl;
      if (right(i) < (int)size())
        std::cout << i << " -> " << right(i) << std::endl;
    }
    std::cout << "}" << std::endl;
  }


}

#endif
