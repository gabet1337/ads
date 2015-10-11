#ifndef FIBONACCI_QUEUE_HPP
#define FIBONACCI_QUEUE_HPP

#include "h_priority_queue.hpp"
#include "fibonacci_heap.hpp"
#include <unordered_map>
#include <iostream>

namespace pq {
  template<class T>
  class fibonacci_queue : public fibonacci_heap<T>, h_priority_queue<T> {
	
  public:

    std::unordered_multimap<T,typename fibonacci_heap<T>::Node*> fstore;

    fibonacci_queue();
    ~fibonacci_queue();

    void push(T v, int k);
    void pop();
    void decrease_key(T v, int k);
    T top();
    bool empty();

    typename fibonacci_heap<T>::Node* findNode(T vert) {
      typename std::unordered_map<T,typename fibonacci_heap<T>::Node*>::iterator mit = find(vert);
      if (mit == fstore.end()) return nullptr;
      return (*mit).second;
    }

    typename std::unordered_map<T,typename fibonacci_heap<T>::Node*>::iterator find(T k)  {
      typename std::unordered_map<T,typename fibonacci_heap<T>::Node*>::iterator mit = fstore.find(k);
      return mit;
    }

  };

  template <class T>
  fibonacci_queue<T>::fibonacci_queue()
    :fibonacci_heap<T>()
  {}

  template <class T>
  fibonacci_queue<T>::~fibonacci_queue()
  {}

  template <class T>
  void fibonacci_queue<T>::push(T v, int k) {
    typename fibonacci_heap<T>::Node *x = fibonacci_heap<T>::push(k,v);
    fstore.insert(std::pair<T,typename fibonacci_heap<T>::Node*>(v,x));
  }
 
  template <class T>
  void fibonacci_queue<T>::pop() {
    if (fibonacci_heap<T>::empty())
      return;
    typename fibonacci_heap<T>::Node *x = fibonacci_heap<T>::DeleteMin();
    if (!x)
      std::cerr << "Brændvarm Indianer (should never happen)";
    typename std::unordered_map<T,typename fibonacci_heap<T>::Node*>::iterator mit = find(x->payload);
    if (mit != fstore.end())
      fstore.erase(mit);
    else
      std::cerr << "key " << x->key << " cannot be found in hashmap" << std::endl;
    delete x;
  }

  template <class T>
  void fibonacci_queue<T>::decrease_key(T v, int k) {

    typename fibonacci_heap<T>::Node *x = findNode(v);

    if (x != nullptr)
      fibonacci_heap<T>::DecreaseKey(findNode(v),k);
    else
      push(v, k);
  }

  template <class T>
  T fibonacci_queue<T>::top() {
    return fibonacci_heap<T>::min->payload;
  }

  template <class T>
  bool fibonacci_queue<T>::empty() {
    return fibonacci_heap<T>::empty();
  }
}
#endif
