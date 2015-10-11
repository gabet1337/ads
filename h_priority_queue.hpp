template <class T>
class h_priority_queue {
public:
  virtual bool empty() = 0;
  virtual T top() = 0;
  virtual void push(T v, int k) = 0;
  virtual void pop() = 0;
  virtual void decrease_key(T v, int k) = 0;
};

