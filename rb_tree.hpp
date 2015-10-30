#ifndef RB_TREE
#define RB_TREE
#include <cstddef>
#include <iostream>
#include <queue>
#include "predecessor_queue.hpp"
#define RED 1
#define BLACK 0
namespace pq {

  class rb_tree : public predecessor_queue{

    struct node {
      bool color;
      int key;
      node *left, *right, *p;
      node(int _key) : key(_key) {}
    };

  public:
    rb_tree();
    ~rb_tree();
    void insert(int v);
    void erase(int v);
    int minimum();
    int maximum();
    size_t size();
    int successor(int x);
    int predecessor(int x);
    void erase_min();

    // predecessor_queue
    bool empty();
    int find_min();
    void delete_min();
    void delete_key(int k);
    void print();
    
  private:
    size_t _size;
    node *root, *sentinel;
    void rotate_left(node* x);
    void rotate_right(node *x);
    void insert_fixup(node *z);
    void transplant(node *u, node *v);
    void erase_fixup(node *x);
    node* min(node* st);
    node* max(node* st);
    node* find(int v);
    void destruct(node* st);
    void find_suc(int x, node* st, node*& suc);
    void find_pre(int x, node* st, node*& pre);
  };

  rb_tree::rb_tree() {
    sentinel = new node(0);
    sentinel->color = BLACK;
    sentinel->p = sentinel;
    sentinel->right = sentinel;
    sentinel->left = sentinel;
    root = sentinel;
    _size = 0;
  }

  size_t rb_tree::size() {
    return _size;
  }

  void rb_tree::destruct(node *st) {
    if (st->left != sentinel)
      destruct(st->left);
    if (st->right != sentinel)
      destruct(st->right);
    delete st;
  }

  rb_tree::~rb_tree() {
    if (root != sentinel)
      destruct(root);
    delete sentinel;
  }

  void rb_tree::rotate_left(node *x) {
    node *y = x->right;
    x->right = y->left;
    if (y->left != sentinel) y->left->p = x;
    y->p = x->p;
    if (x->p == sentinel) root = y;
    else if (x == x->p->left) x->p->left = y;
    else x->p->right = y;
    y->left = x;
    x->p = y;
  }

  void rb_tree::rotate_right(node *y) {
    node *x = y->left;
    y->left = x->right;
    if (x->right != sentinel) x->right->p = y;
    x->p = y->p;
    if (y->p == sentinel) root = x;
    else if (y == y->p->right) y->p->right = x;
    else y->p->left = x;
    x->right = y;
    y->p = x;
    // node *x = y->left;
    // y->left = x->right;
  }

  void rb_tree::insert(int v) {
    node* z = new node(v);
    node* y = sentinel;
    node* x = root;
    while (x != sentinel) {
      y = x;
      if (z->key < x->key) x = x->left;
      else x = x->right;
    }
    z->p = y;
    if (y == sentinel) root = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;
    z->left = sentinel;
    z->right = sentinel;
    z->color = RED;
    insert_fixup(z);
    _size++;
  }

  void rb_tree::insert_fixup(node* z) {
    while (z->p->color == RED) {
      if (z->p == z->p->p->left) {
        node* y = z->p->p->right;
        if (y->color == RED) {
          z->p->color = BLACK;
          y->color = BLACK;
          z->p->p->color = RED;
          z = z->p->p;
        } else {
          if (z == z->p->right) {
            z = z->p;
            rotate_left(z);
          }
          z->p->color = BLACK;
          z->p->p->color = RED;
          rotate_right(z->p->p);
        }
      } else {
        node* y = z->p->p->left;
        if (y->color == RED) {
          z->p->color = BLACK;
          y->color = BLACK;
          z->p->p->color = RED;
          z = z->p->p;
        } else {
          if (z == z->p->left) {
            z = z->p;
            rotate_right(z);
          }
          z->p->color = BLACK;
          z->p->p->color = RED;
          rotate_left(z->p->p);
        }
      }
    }
    root->color = BLACK;
  }

  int rb_tree::minimum() {
    return min(root)->key;
  }

  int rb_tree::maximum() {
    return max(root)->key;
  }

  rb_tree::node* rb_tree::min(node *st) {
    while (st->left != sentinel) st = st->left;
    return st;
  }

  rb_tree::node* rb_tree::max(node *st) {
    while (st->right != sentinel) st = st->right;
    return st;
  }

  rb_tree::node* rb_tree::find(int v) {
    node *x = root;
    while (x->key != v) {
      if (v < x->key) x = x->left;
      else x = x->right;
    }
    return x;
  }

  void rb_tree::erase(int v) {
    node* z = find(v);
    node* y = z;
    node* x;
    bool y_original_color = y->color;
    if (z->left == sentinel) {
      x = z->right;
      transplant(z,z->right);
    } else if (z->right == sentinel) {
      x = z->left;
      transplant(z,z->left);
    } else {
      y = min(z->right);
      y_original_color = y->color;
      x = y->right;
      if (y->p == z) x->p = y;
      else {
        transplant(y,y->right);
        y->right = z->right;
        y->right->p = y;
      }
      transplant(z,y);
      y->left = z->left;
      y->left->p = y;
      y->color = z->color;
    }
    if (y_original_color == BLACK)
      erase_fixup(x);
    _size--;
  }

  void rb_tree::erase_fixup(node* x) {
    while (x != root && x->color == BLACK) {
      if (x == x->p->left) {
        node* w = x->p->right;
        if (w->color == RED) {
          w->color = BLACK;
          x->p->color = RED;
          rotate_left(x->p);
          w = x->p->right;
        }
        if (w->left->color == BLACK && w->right->color == BLACK) {
          w->color = RED;
          x = x->p;
        } else {
          if (w->right->color == BLACK) {
            w->left->color = BLACK;
            w->color = RED;
            rotate_right(w);
            w = x->p->right;
          }
          w->color = x->p->color;
          x->p->color = BLACK;
          w->right->color = BLACK;
          rotate_left(x->p);
          x = root;
        }
      } else {
        node* w = x->p->left;
        if (w->color == RED) {
          w->color = BLACK;
          x->p->color = RED;
          rotate_right(x->p);
          w = x->p->left;
        }
        if (w->right->color == BLACK && w->left->color == BLACK) {
          w->color = RED;
          x = x->p;
        } else {
          if (w->left->color == BLACK) {
            w->right->color = BLACK;
            w->color = RED;
            rotate_left(w);
            w = x->p->left;
          }
          w->color = x->p->color;
          x->p->color = BLACK;
          w->left->color = BLACK;
          rotate_right(x->p);
          x = root;
        }
      }
    }
    x->color = BLACK;
  }

  void rb_tree::transplant(node* u, node* v) {
    if (u->p == sentinel) root = v;
    else if (u == u->p->left) u->p->left = v;
    else u->p->right = v;
    v->p = u->p;
  }

  int rb_tree::successor(int x) {
    if (size() < 1) return -1;
    if (x >= maximum()) return 2147483647;
    node* suc;
    find_suc(x,root,suc);
    return suc->key;
  }

  int rb_tree::predecessor(int x) {
    if (size() < 1) return -1;
    if (x <= minimum()) return -2147483647;
    node *pre;
    find_pre(x,root,pre);
    return pre->key;
  }

  void rb_tree::find_suc(int x, node* st, node*& suc) {
    if (st == sentinel) return;
    if (st->key == x) {
      if (st->right != sentinel) suc = min(st->right);
      return;
    } else if (x < st->key) {
      suc = st;
      find_suc(x, st->left, suc);
    } else {
      find_suc(x, st->right, suc);
    }
  }

  void rb_tree::find_pre(int x, node* st, node*& pre) {
    if (st == sentinel) return;
    if (st->key == x) {
      if (st->right != sentinel) pre = max(st->left);
      return;
    } else if (x < st->key) {
      find_pre(x, st->left, pre);
    } else {
      pre = st;
      find_pre(x, st->right, pre);
    }
  }


  void rb_tree::erase_min() {
    if (size() > 0) {
      erase(minimum());
    }
  }

  // predecessor_queue
  bool rb_tree::empty() {
    return (_size == 0);
  }
  
  int rb_tree::find_min() {
    return minimum();
  }
  
  void rb_tree::delete_min() {
    erase(minimum());
  }
  
  void rb_tree::delete_key(int k) {
    erase(k);
  }
  
  void rb_tree::print() {
    std::cout << "digraph G {" << std::endl;

    std::queue<rb_tree::node*> q;
    q.push(root);
    while (!q.empty()) {
      node* x = q.front(); q.pop();
      std::cout << x->key << " -> " << x->left->key << std::endl;
      std::cout << x->key << " -> " << x->right->key << std::endl;
      if (x->left != sentinel)
        q.push(x->left);
      if (x->right != sentinel)
        q.push(x->right);
    }

    // int next = (1<<25);
    // std::queue<std::pair<int, rb_tree::node*> > q;
    // q.push(std::make_pair(next++,root));
    // while (!q.empty()) {
    //   std::pair<int, rb_tree::node*> x = q.front(); q.pop();
    //   std::cout << x.first << " [label=\""<<x.second->key<<"\"]" << std::endl;
    //   if (x.second->left != sentinel) {
    //     q.push(std::make_pair(next++, x.second->left));
    //   }
    //   if (x.second->right != sentinel) {
    //     q.push(std::make_pair(next++, x.second->right));
    //   }
    // }
    // next = (1<<25);
    // q.push(std::make_pair(next++,root));
    // while (!q.empty()) {
    //   std::pair<int, rb_tree::node*> x = q.front(); q.pop();
    //   if (x.second->left != sentinel) {
    //     std::cout << x.first << " -> " << next++ << std::endl;
    //     q.push(std::make_pair(next, x.second->left));
    //   }
    //   if (x.second->right != sentinel) {
    //     std::cout << x.first << " -> " << next++ << std::endl;
    //     q.push(std::make_pair(next, x.second->right));
    //   }
    // }

    std::cout << "}" << std::endl;
  }
  

};


#endif
