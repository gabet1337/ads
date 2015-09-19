// FibHeap.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "iostream"

using namespace std;

template<class T>
class FibHeap {
public:
	
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

	// To make an empty Fibonacci heap, the Make-Fib-Heap procedure allocates and returns the Fibonacci heap object H, where H.n = 0 and H.min = NIL.
	FibHeap()
		:n(0),
		min(nullptr)
		{
		}

	// The following procedure inserts node x into Fibonacci heap H, aussoming that the node has already been allocated and that x.key has alread been filled in.
	void Insert(Node *x) {
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

	Node* FindMin() {
		return min;
	}

	// First make a root out of each of the minimum node's children and remove the minimum node from the root list.
	// Then CONSOLIDATE the root list by linking roots of equal degree undtil at most one root remains of each degree.
	Node* DeleteMin() {
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
	void Consolidate() {

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

	void Fib_Heap_Link(Node* y, Node* x) {

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


	void DecreaseKey(Node* x, int key) {

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


	void Cut(Node* x, Node* y) {

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

	void CascadingCut(Node* y) {
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

private:

	int n;
	Node *min;

};

int _tmain(int argc, _TCHAR* argv[])
{
	FibHeap<int> fh;

	FibHeap<int>::Node* x1 = new FibHeap<int>::Node(23,nullptr);
	fh.Insert(x1);
	fh.Insert(new FibHeap<int>::Node(54,nullptr));
	fh.Insert(new FibHeap<int>::Node(10,nullptr));
	fh.Insert(new FibHeap<int>::Node(5,nullptr));
	fh.Insert(new FibHeap<int>::Node(9,nullptr));
	FibHeap<int>::Node* x2 = new FibHeap<int>::Node(85,nullptr);
	fh.Insert(x2);
	cout << fh.DeleteMin()->key << endl;
	fh.DecreaseKey(x1, 0);
	fh.Insert(new FibHeap<int>::Node(54,nullptr));
	fh.Insert(new FibHeap<int>::Node(32,nullptr));
	cout << fh.DeleteMin()->key << endl;
	fh.Insert(new FibHeap<int>::Node(4,nullptr));
	fh.Insert(new FibHeap<int>::Node(3,nullptr));
	cout << fh.DeleteMin()->key << endl;
	fh.Insert(new FibHeap<int>::Node(8,nullptr));
	fh.Insert(new FibHeap<int>::Node(8,nullptr));
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

	return 0;
}