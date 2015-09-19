#ifndef FIBONACCI_HEAP_HPP
#define FIBONACCI_HEAP_HPP

#include <math.h>
#include <iostream>
#include <cstddef>
#include <limits>
#include "priority_queue.hpp"

using namespace std;

template<class T>
class fibonnaci_heap : public priority_queue<T> {
public:

  fibonnaci_heap();
  ~fibonnaci_heap();
  bool empty();
  std::size_t size();
  T top();
  void push(T k);
  void pop();

private:

  class Node {
  public:
    Node(T _key, void* _payload)
      :key(_key),
       degree(0),
       marked(false),
       parent(nullptr),
       left(nullptr),
       right(nullptr),
       child(nullptr),
       payload(_payload)
    {
    }

    ~Node() {
    }

    T key;
    int degree;
    bool marked;
    Node *parent;
    Node *left;
    Node *right;
    Node *child;
    void* payload;
  };

  int n;
  Node *min;

  void Insert(Node *x);
  Node* FindMin();
  Node* DeleteMin();
  void Consolidate();
  void Fib_Heap_Link(Node* y, Node* x);
  void DecreaseKey(Node* x, int key);
  void Cut(Node* x, Node* y);
  void CascadingCut(Node* y);
};

// To make an empty Fibonacci heap, the Make-Fib-Heap procedure allocates and returns the Fibonacci heap object H, where H.n = 0 and H.min = NIL.
template <class T>	
fibonnaci_heap<T>::fibonnaci_heap()
  :n(0),
   min(nullptr)
{
}

template <class T>	
fibonnaci_heap<T>::~fibonnaci_heap()
{
}

// The following procedure inserts node x into Fibonacci heap H, assuming that the node has already been allocated and that x.key has alread been filled in.
template <class T>
void fibonnaci_heap<T>::Insert(Node *x) {
  //1 X.degree = 0
  x->degree = 0;
		
  //2 x.p = NIL
  x->parent = nullptr;

  //3 x.child = NIL
  x->child = nullptr;

  //4 x.marked = FALSE
  x->marked = false;

  if (min == nullptr) {
    // create a root list for H containing just x
    x->left = x;
    x->right = x;
    min = x;
  } else {
    // insert x into H's root list
    min->left->right = x;
    x->left = min->left;
    min->left = x;
    x->right = min;

    // if x.key < H.min.key
    if (x->key < min->key)
      min = x;
    //H.n = H.n + 1
    n = n+1;
  }
}


template <class T>
typename fibonnaci_heap<T>::Node* fibonnaci_heap<T>::FindMin() {
  return min;
}

// First make a root out of each of the minimum node's children and remove the minimum node from the root list.
// Then CONSOLIDATE the root list by linking roots of equal degree undtil at most one root remains of each degree.
template <class T>
typename fibonnaci_heap<T>::Node* fibonnaci_heap<T>::DeleteMin() {
  // z = H.min
  Node *z = min;
  // If z != NIL
  if (z != nullptr) {
    // for each child x of z
    Node *x = z->child; // First child visited
    if (x != nullptr) {
      do {

	// Remember current childs right neighbor
	Node *next = x->right;

	// add children to root list of H
	min->left->right = x;
	x->left = min->left;
	min->left = x;
	x->right = min;

	// x.p = NIL
	x->parent = nullptr;

	// Look at next child
	x = next;

      } while (x != z->child);

    }
				
    // remove z from the root list of H
    min->left->right = min->right;
    min->right->left = min->left;

    // if z == z.right
    if (z == z->right)
      min = nullptr; // H.min = NIL
    else {
      min = z->right;
      Consolidate();
    }
				
    n = n - 1;
  }

  return z;

}

// The procedure Consolidate uses an auxiliary array A[0 .. D(H.n)] to keep track of roots according to their degrees.
// If A[i] = y, then y is currently root with y.degree = i. It is shown in CLRS 19.4 that D(H.n) is upper bounded by log by golden ratio of n (nice!).
template <class T>
void fibonnaci_heap<T>::Consolidate() {

  int golden_ratio = static_cast<int>(floor(log(static_cast<double>(n))/log(static_cast<double>(1 + sqrt(static_cast<double>(5)))/2)));

  int d_hn = golden_ratio+2;

  // let A[O .. D(H.n)] be a new array
  Node** A = new Node*[d_hn];

  // for i = 0 to D(H.n)
  for (int i = 0; i < d_hn; i++)
    A[i] = nullptr; // A[i] = NIL
	
  // create a root list to iterate
  int rootSize = 0;

  Node* next;
  Node* w = min;
  do {
    next = w->right;
    rootSize++;
    w = next;
  } while (w != min);

  Node** rootList = new Node*[rootSize];
  for (int i=0; i < rootSize; i++) {
    rootList[i] = next;
    next = next->right;
  }

  // for each node w in the root list of H
  //Node* w = min;

  for (int i=0; i < rootSize; i++) {
	
    // x = w
    Node* x = rootList[i];

    // d = x.degree
    int d = x->degree;

    // while A[d] != NIL
    while ( A[d] != nullptr ) {

      Node* y = A[d]; // another node with same degree as x
				
      if ( x->key > y->key ) {
	// exchange x with y
	Node* temp = x;
	x = y;
	y = temp;
      }

      Fib_Heap_Link(y,x);

      A[d] = nullptr;

      d = d + 1;
    }
			
    A[d] = x;

  }

  delete [] rootList;
		
  min = nullptr;
		
  // for i = 0 to D(H.n)
  for (int i=0; i < d_hn; i++) {
    if (A[i] != nullptr) {
      if (min == nullptr) {
	min = A[i]; // create a root list for H containing just A[i]
	min->right = min;
	min->left = min;
      } else {
	// insert A[i] into H's root list
	min->left->right = A[i];
	A[i]->left = min->left;
	min->left = A[i];
	A[i]->right = min;
	// update min element
	if (A[i]->key < min->key)
	  min = A[i];
      }
    }
  }
  delete [] A;
}

template <class T>
void fibonnaci_heap<T>::Fib_Heap_Link(Node* y, Node* x) {

  // remove y from the root list of H
  y->left->right = y->right;
  y->right->left = y->left;

  // make y a child of x, incrementing x.degree
  if (x->child == nullptr) {
    // x has no children, insert y.
    x->child = y;
    y->right = y;
    y->left = y;
    x->degree = 1;
  } else {
    // add y to child list
    x->child->left->right = y;
    y->left = x->child->left;
    x->child->left = y;
    y->right = x->child;
    x->degree++;
  }

  y->parent = x;
  x->marked = false;

}

template <class T>
void fibonnaci_heap<T>::DecreaseKey(Node* x, int key) {

  if (key > x->key)
    return; // error: new key is greater than current key

  x->key = key;
  Node* y = x->parent;

  if (y != nullptr && x->key < y->key) {
    Cut(x,y);
    CascadingCut(y);
  }

  if (x->key < min->key)
    min = x;
}

template <class T>
void fibonnaci_heap<T>::Cut(Node* x, Node* y) {

  // remove x from the child list of y, decrementing y.degree
  if (y->degree == 1) // test if x is only child
    y->child = nullptr;
  else {
    x->left->right = x->right;
    x->right->left = x->left;
  }
  y->degree--;

  // add x to the root list of H
  min->left->right = x;
  x->left = min->left;
  min->left = x;
  x->right = min;

  x->parent = nullptr;
  x->marked = false;

}

template <class T>
void fibonnaci_heap<T>::CascadingCut(Node* y) {
  Node* z = y->parent;
  if (z != nullptr) {
    if (y->marked == false) {
      y->marked = true;
    } else {
      Cut(y,z);
      CascadingCut(z);
    }
  }
}

template <class T>
bool fibonnaci_heap<T>::empty() {
  return n == 0;
}

template <class T>
std::size_t fibonnaci_heap<T>::size() {
  return n;
}

template <class T>
T fibonnaci_heap<T>::top() {
  return min->key;
}

template <class T>
void fibonnaci_heap<T>::push(T k) {
  Insert(new fibonnaci_heap<T>::Node(k,nullptr));
}

template <class T>
void fibonnaci_heap<T>::pop() {
  if (n < 1) return; // throw some error
  DeleteMin();
}

//int main(int argc, char* argv[])
//{

  //fibonnaci_heap<int> fh;
  //fh.push(10);
  //fh.push(20);
  //fh.push(5);
  //fh.push(80);

  //cout << fh.top() << endl;

  //fh.pop();

  //cout << fh.top() << endl;

  //cout << fh.empty() << endl;

  //while (!fh.empty())
  //  fh.pop();

  //cout << fh.empty() << endl;

  //cout << fh.empty();
	
  //fh.push(10);

  /*
    fibonnaci_heap<int>::Node* x1 = new fibonnaci_heap<int>::Node(23,nullptr);
    fh.Insert(x1);
    fh.Insert(new fibonnaci_heap<int>::Node(54,nullptr));
    fh.Insert(new fibonnaci_heap<int>::Node(10,nullptr));
    fh.Insert(new fibonnaci_heap<int>::Node(5,nullptr));
    fh.Insert(new fibonnaci_heap<int>::Node(9,nullptr));
    fibonnaci_heap<int>::Node* x2 = new fibonnaci_heap<int>::Node(85,nullptr);
    fh.Insert(x2);
    cout << fh.DeleteMin()->key << endl;
    fh.DecreaseKey(x1, 0);
    fh.Insert(new fibonnaci_heap<int>::Node(54,nullptr));
    fh.Insert(new fibonnaci_heap<int>::Node(32,nullptr));
    cout << fh.DeleteMin()->key << endl;
    fh.Insert(new fibonnaci_heap<int>::Node(4,nullptr));
    fh.Insert(new fibonnaci_heap<int>::Node(3,nullptr));
    cout << fh.DeleteMin()->key << endl;
    fh.Insert(new fibonnaci_heap<int>::Node(8,nullptr));
    fh.Insert(new fibonnaci_heap<int>::Node(8,nullptr));
    cout << fh.DeleteMin()->key << endl;
    cout << fh.DeleteMin()->key << endl;
    cout << fh.DeleteMin()->key << endl;
    cout << fh.DeleteMin()->key << endl;
    fh.DecreaseKey(x2, 4);
    cout << fh.DeleteMin()->key << endl;
    cout << fh.DeleteMin()->key << endl;
    cout << fh.DeleteMin()->key << endl;
    cout << fh.DeleteMin()->key << endl;

    //cout << fh.FindMin()->key << endl;
    */
 // return 0;
//}

#endif
