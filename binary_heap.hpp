#ifndef BINARY_HEAP_HPP
#define BINARY_HEAP_HPP
#include <vector>
namespace pq {
  template <class T>
  class binary_heap {
  public:
    binary_heap();
    ~binary_heap();
    bool empty();
    std::size_t size();
    T top();
    void push(T k);
    void pop();

  private:
    std::size_t _size;
    std::vector<T> container;
    int parent(int i);
    int left(int i);
    int right(int i);
    void max_heapify(int i);
    void heap_increase_key(int i, T key);
  };



  template <class T>
  binary_heap<T>::binary_heap() {
    _size = 0;
    container.push_back(0); // we dont use this element
  }

  template <class T>
  binary_heap<T>::~binary_heap() {
    container.clear();
  }

  template <class T>
  bool binary_heap<T>::empty() {
    return _size == 0;
  }

  template <class T>
  std::size_t binary_heap<T>::size() {
    return _size;
  }

  template <class T>
  T binary_heap<T>::top() {
    if (size() > 0) return container[1];
    else return 0; //throw some error
  }

  template <class T>
  void binary_heap<T>::push(T k) {
    _size++;
    container.push_back(k);
    heap_increase_key(_size, k);
  }

  template <class T>
  void binary_heap<T>::pop() {
    if (size() < 1) return; // throw some error
    container[1] = container[_size];
    _size--;
    container.pop_back();
    max_heapify(1);
  }

  template <class T>
  int binary_heap<T>::parent(int i) {
    return i/2;
  }

  template <class T>
  int binary_heap<T>::left(int i) {
    return 2*i;
  }

  template <class T>
  int binary_heap<T>::right(int i) {
    return 2*i+1;
  }

  template <class T>
  void binary_heap<T>::max_heapify(int i) {
    int l = left(i), r = right(i);
    int largest;
    if (l <= size() && container[l] > container[i]) largest = l;
    else largest = i;
    if (r <= size() && container[r] > container[largest]) largest = r;
    if (largest != i) {
      std::swap(container[i],container[largest]);
      max_heapify(largest);
    }
  }

  template <class T>
  void binary_heap<T>::heap_increase_key(int i, T key) {
    container[i] = key;
    while (i > 1 && container[parent(i)] < container[i]) {
      std::swap(container[i], container[parent(i)]);
      i = parent(i);
    }
  }
}

// void build_max_heap(vector<int> &A) {
//   for (int i = (int)A.size()/2; i >= 1; i--)
//     max_heapify(i);
// }

// int heap_extract_max() {
//   int res = A[1];
//   A[1] = A[heap_size()];
//   heap_size--;
//   max_heapify(1);
//   return res;
// }

// void heap_insert(int key) {
//   heap_size++;
//   A[heap_size()] = -INF;
//   heap_increase_key(heap_size(), key);
// }

#endif
