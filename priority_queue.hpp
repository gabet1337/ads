#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP
#include <cstddef>
#include <utility>
class priority_queue {
public:
  priority_queue() {};
  virtual ~priority_queue() {};
  virtual bool empty() = 0;
  virtual std::size_t size() = 0;
  virtual std::pair<int,int> top() = 0;
  virtual void push(std::pair<int,int> k) = 0;
  virtual void pop() = 0;
  virtual void decrease_key( int id, int key ) = 0;
  virtual void print() = 0;
};

#endif
