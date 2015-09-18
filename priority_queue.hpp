#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

template <class T>
class priority_queue {
public:
  virtual bool empty() = 0;
  virtual std::size_t size() = 0;
  virtual T top() = 0;
  virtual void push(T k) = 0;
  virtual void pop() = 0;
};

#endif
