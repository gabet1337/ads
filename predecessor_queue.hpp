#ifndef PREDECESSOR_QUEUE_HPP
#define PREDECESSOR_QUEUE_HPP
#include <cstddef>
#include <utility>
class predecessor_queue {
public:
  predecessor_queue() {};
  virtual ~predecessor_queue() {};
  virtual bool empty() = 0;
  virtual std::size_t size() = 0;
  virtual int find_min() = 0;
  virtual void insert(int k) = 0;
  virtual void delete_min() = 0;
  virtual void delete_key(int k) = 0;
  virtual int predecessor(int k) = 0;
  virtual void print() = 0;
};

#endif
