#ifndef VAN_EMDE_BOAS
#define VAN_EMDE_BOAS
#include "priority_queue.hpp"
#include "predecessor_queue.hpp"
#include <cmath>
#include <iostream>
//2^31 - 1
#define UNDEF 2147483647

typedef std::pair<int,int> ii;

namespace pq {
  // universe size is 2^24
  class van_emde_boas : public priority_queue, public predecessor_queue {

  public:
    van_emde_boas();
    van_emde_boas(int u);
    ~van_emde_boas();
    int minimum;
    int maximum;
    bool member(int x);
    int predecessor(int x);
    void insert(int x);
    void erase(int x);
    void delete_min();

    // generic_queue
    bool empty();
    std::size_t size();
    void print();
    
    // priority_queue
    void push(ii k);
    void pop();
    void decrease_key(int v, int k);
    ii top();

    // predecessor_queue
    int find_min();
    void delete_key(int k);
    
  private:
    int u;
    size_t n;
    int high(int x);
    int low(int x);
    int index(int x, int y);
    int usr(int u);
    int lsr(int u);
    void empty_insert(int x);

    van_emde_boas *summary;
    van_emde_boas **cluster;
  };
  //16777216
  van_emde_boas::van_emde_boas() : van_emde_boas(16777216) { }

  van_emde_boas::van_emde_boas(int u) {
    this->u = u;
    this->minimum = UNDEF;
    this->maximum = -UNDEF;
    if (u == 2) { summary = 0; cluster = 0; }
    else {
      cluster = new van_emde_boas*[usr(u)];
      for (int i = 0; i < usr(u); i++)
        cluster[i] = new van_emde_boas(lsr(u));
      summary = new van_emde_boas(usr(u));
    }
    n = 0;
  }

  van_emde_boas::~van_emde_boas() {
    if (u > 2) {
      delete summary; summary = 0;
      for (int i = 0; i < usr(u); i++)
        delete cluster[i];
      delete [] cluster;
    }
  }

  bool van_emde_boas::member(int x) {
    if (x == minimum || x == maximum) return true;
    else if (u == 2) return false;
    else return cluster[high(x)]->member(low(x));
  }

  int van_emde_boas::predecessor(int x) {
    if (u == 2) {
      if (x == 1 && minimum == 0) return 0;
      else return -UNDEF;
    } else if (std::abs(maximum) != UNDEF && x > maximum) return maximum;
    else {
      int min_low = cluster[high(x)]->minimum;
      if (std::abs(min_low) != UNDEF && low(x) > min_low) {
        int offset = cluster[high(x)]->predecessor(low(x));
        return index(high(x), offset);
      } else {
        int pred_cluster = summary->predecessor(high(x));
        if (std::abs(pred_cluster) == UNDEF)
          if  (std::abs(minimum) != UNDEF && x > minimum) return minimum;
          else return -UNDEF;
        else {
          int offset = cluster[pred_cluster]->maximum;
          return index(pred_cluster, offset);
        }
      }
    }
  }

  void van_emde_boas::empty_insert(int x) {
    minimum = maximum = x;
  }

  void van_emde_boas::insert(int x) {
    if (std::abs(minimum) == UNDEF) empty_insert(x);
    else {
      if (x < minimum) std::swap(x, minimum);
      if (u > 2) {
        if (std::abs(cluster[high(x)]->minimum) == UNDEF) {
          summary->insert(high(x));
          cluster[high(x)]->empty_insert(low(x));
        } else cluster[high(x)]->insert(low(x));
      }
      if (x > maximum) maximum = x;
    }
    n++;
  }

  void van_emde_boas::delete_min() {
    if (std::abs(minimum == UNDEF)) return;
    erase(minimum);
  }

  void van_emde_boas::erase(int x) {
    if (minimum == maximum) { minimum = UNDEF; maximum = -UNDEF;}
    else if (u == 2) {
      if (x == 0) minimum = 1;
      else minimum = 0;
      maximum = minimum;
    }
    else {
      if (x == minimum) {
        int first_cluster = summary->minimum;
        x = index(first_cluster, cluster[first_cluster]->minimum);
        minimum = x;
      }
      cluster[high(x)]->erase(low(x));
      if (std::abs(cluster[high(x)]->minimum) == UNDEF) {
        summary->erase(high(x));
        if (x == maximum) {
          int summary_max = summary->maximum;
          if (std::abs(summary_max) == UNDEF) maximum = minimum;
          else maximum = index(summary_max, cluster[summary_max]->maximum);
        }
      } else if (x == maximum) maximum = index(high(x), cluster[high(x)]->maximum);
    }
    n--;
  }

  //upper square root
  int van_emde_boas::usr(int u) {
    return pow(2, ceil(log2(u)/2.0));
  }

  //lower square root
  int van_emde_boas::lsr(int u) {
    return pow(2, floor(log2(u)/2.0));
  }

  int van_emde_boas::high(int x) {
    return floor(x / lsr(u));
  }

  int van_emde_boas::low(int x) {
    return x % lsr(u);
  }

  int van_emde_boas::index(int x, int y) {
    return x * lsr(u) + y;
  }

  // queue
  bool van_emde_boas::empty() {
    return size() == 0;
  }

  std::size_t van_emde_boas::size() {
    return n;
  }

  // priority_queue
  ii van_emde_boas::top() {
    return std::make_pair(minimum,-1);
  }

  void van_emde_boas::push(ii k) {
    insert(k.first);
   }

  void van_emde_boas::pop() {
    delete_min();
   }

  void van_emde_boas::print() {
    
  }

  void van_emde_boas::decrease_key(int v, int k) {
    erase(v);
    insert(k);
  }

  // predecessor_queue
  int van_emde_boas::find_min() {
    return minimum;
  }

  void van_emde_boas::delete_key(int k) {
    erase(k);
  }
  
}
#endif
