#ifndef FIBONACCI_HEAP_HPP
#define FIBONACCI_HEAP_HPP
#define GOLDEN_RATIO 1.61803398875

#include <math.h>
#include <cstddef>
#include <utility>
#include <iostream>
#include <string>

namespace pq {

  typedef std::pair<int,int> ii;

  class fibonacci_heap {
  public:

    fibonacci_heap();
    ~fibonacci_heap();
    bool empty();
    std::size_t size();

    class Node {
    public:
      Node(ii _key)
        :key(_key),
         degree(0),
         marked(false),
         parent(nullptr),
         left(nullptr),
         right(nullptr),
         child(nullptr)
      {}

      ~Node() {}

      ii key;
      int degree;
      bool marked;
      Node *parent;
      Node *left;
      Node *right;
      Node *child;
    };

    void DecreaseKey(Node* x, ii key);
    Node* push(ii k);    
    void Insert(Node *x);
    Node* FindMin();
    Node* DeleteMin();
    void pop();
    Node* top();
    void print();
    
  private:

    Node *min;
    std::size_t n = 0;
    void Consolidate();
    void Fib_Heap_Link(Node* y, Node* x);
    void Cut(Node* x, Node* y);
    void CascadingCut(Node* y);
    void DeleteNodes(Node *x);
    void printNode(Node *x);

  };

  void fibonacci_heap::DeleteNodes(Node *x) {

    if (x == nullptr)
      return;

    Node *next = x;

    do {
      Node* cur = next;
      next = next->right;
      if (cur->degree > 0)
	DeleteNodes(cur->child);   
      delete cur;
    } while (next != x);

  }

  void fibonacci_heap::print() {

    std::cout << " digraph fibheap {" << std::endl;

    printNode(min);
    
    std::cout << "}" << std::endl;
    
  }

  void fibonacci_heap::printNode(Node* x) {

    if (x == nullptr)
      return;
    
    Node *next = x;

    do {
      Node* cur = next;
      next = next->right;
      if (cur->degree > 0) {
	std::cout << cur->key.second << " -> " << cur->child->key.second << " [label=\"c\"]" << std::endl;
	std::cout << cur->child->key.second << " -> " << cur->key.second << " [label=\"p\"]" << std::endl;
	printNode(cur->child);
      }
      if (cur != next) {
	std::cout << cur->key.second << " -> " << next->key.second << " [label=\"r\"]" << std::endl;
	std::cout << next->key.second << " -> " << cur->key.second << " [label=\"l\"]" << std::endl;
      }
      
    } while (next != x);
    
  }
  
  // To make an empty Fibonacci heap, the Make-Fib-Heap procedure allocates and returns the Fibonacci heap object H, where H.n = 0 and H.min = NIL.
  fibonacci_heap::fibonacci_heap() {
    n = 0;
    min = nullptr;
  }

  fibonacci_heap::~fibonacci_heap()
  {
    DeleteNodes(min);
  }

  // The following procedure inserts node x into Fibonacci heap H, assuming that the node has already been allocated and that x.key has alread been filled in.
  
  void fibonacci_heap::Insert(Node *x) {
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
    }
    n++;
  }


  
  typename fibonacci_heap::Node* fibonacci_heap::FindMin() {
    return min;
  }

  // First make a root out of each of the minimum node's children and remove the minimum node from the root list.
  // Then CONSOLIDATE the root list by linking roots of equal degree undtil at most one root remains of each degree.
  
  typename fibonacci_heap::Node* fibonacci_heap::DeleteMin() {
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
				
      n--;
    }

    return z;

  }

  // The procedure Consolidate uses an auxiliary array A[0 .. D(H.n)] to keep track of roots according to their degrees.
  // If A[i] = y, then y is currently root with y.degree = i. It is shown in CLRS 19.4 that D(H.n) is upper bounded by log by golden ratio of n (nice!).
  
  void fibonacci_heap::Consolidate() {

    //int golden_ratio = static_cast<int>(floor(log(static_cast<double>(n))/log(static_cast<double>(1 + sqrt(static_cast<double>(5)))/2)));
    int max_degree = (int)floor(log((double)n)/log(GOLDEN_RATIO));

    int d_hn = max_degree+2; //for good measure.

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

        d++;
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
  
  void fibonacci_heap::Fib_Heap_Link(Node* y, Node* x) {

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

  void fibonacci_heap::DecreaseKey(Node* x, ii key) {

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

  void fibonacci_heap::Cut(Node* x, Node* y) {

    // remove x from the child list of y, decrementing y.degree
    if (y->degree == 1) // test if x is only child
      y->child = nullptr;
    else {
      x->left->right = x->right;
      x->right->left = x->left;
      if (y->child == x)
        y->child = x->right;
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

  void fibonacci_heap::CascadingCut(Node* y) {
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

  bool fibonacci_heap::empty() {
    return n == 0;
  }

  std::size_t fibonacci_heap::size() {
    return n;
  }

  typename fibonacci_heap::Node* fibonacci_heap::push(ii k) {
    Node *x = new Node(k);
    Insert(x);
    return x;
  }

  void fibonacci_heap::pop() {
    DeleteMin();
  }

  typename fibonacci_heap::Node* fibonacci_heap::top() {
    return FindMin();
  }

}
#endif
