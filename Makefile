HOBJS = heap_test.o
FOBJS = fibo_tester.o
DOBJS = dijkstra.o
PAPI = /usr/lib/x86_64-linux-gnu/libpapi.so
CC = c++
DEBUG = -g
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)

rbveb : priority_queue.hpp predecessor_queue.hpp van_emde_boas.hpp rb_tree.hpp test_lib.hpp rb_veb_experiments.cpp
	$(CC) $(LFLAGS) priority_queue.hpp predecessor_queue.hpp van_emde_boas.hpp rb_tree.hpp test_lib.hpp rb_veb_experiments.cpp $(PAPI) -o rbveb

rb : rb_tree.hpp rb_tree_test.cpp
	$(CC) $(LFLAGS) rb_tree.hpp rb_tree_test.cpp -o rb

veb : van_emde_boas.hpp veb_test.cpp
	$(CC) $(LFLAGS) van_emde_boas.hpp veb_test.cpp -o veb

vebp : van_emde_boas.hpp veb_priority_test.cpp
	$(CC) $(LFLAGS) van_emde_boas.hpp veb_priority_test.cpp -o vebp

test : $(HOBJS)
	$(CC) $(LFLAGS) $(HOBJS) -o binary_heap_test

fib_test : $(FOBJS)
	$(CC) $(LFLAGS) $(FOBJS) -o fib_test

dijkstra : $(DOBJS)
	$(CC) $(LFLAGS) $(DOBJS) -o dijkstra

h_dijkstra : h_dijkstra_tester.cpp priority_queue.hpp fibonacci_heap.hpp fibonacci_queue.hpp
	$(CC) $(LFLAGS) h_dijkstra_tester.cpp priority_queue.hpp fibonacci_heap.hpp fibonacci_queue.hpp -o h_dijkstra

dijkstra.o : priority_queue.hpp binary_heap.hpp dijkstra.cpp fibonacci_heap.hpp fibonacci_queue.hpp dijkstra.hpp
	$(CC) $(CFLAGS) priority_queue.hpp fibonacci_heap.hpp fibonacci_queue.hpp binary_heap.hpp dijkstra.hpp dijkstra.cpp

heap_test.o : priority_queue.hpp binary_heap.hpp heap_test.cpp
	$(CC) $(CFLAGS) heap_test.cpp

fibo_tester.o : priority_queue.hpp fibonacci_heap.hpp fibo_tester.cpp
	$(CC) $(CFLAGS) fibo_tester.cpp

test_lib : test_lib.hpp test_lib_tester.cpp
	$(CC) $(LFLAGS)  test_lib.hpp test_lib_tester.cpp $(PAPI) -o test_lib_tester

bh_print : binary_heap.hpp priority_queue.hpp bh_test.cpp
	$(CC) $(LFLAGS) binary_heap.hpp priority_queue.hpp bh_test.cpp -o bh_print

run_comp : binary_heap.hpp priority_queue.hpp fibonacci_queue.hpp test_lib.hpp test_run_comp_prio.cpp
	$(CC) $(LFLAGS) binary_heap.hpp priority_queue.hpp fibonacci_queue.hpp test_lib.hpp test_run_comp_prio.cpp $(PAPI) -o test_run_comp

run_veb : binary_heap.hpp priority_queue.hpp fibonacci_queue.hpp van_emde_boas.hpp test_lib.hpp test_run_comp_prio_veb.cpp
	$(CC) $(LFLAGS) binary_heap.hpp van_emde_boas.hpp priority_queue.hpp fibonacci_queue.hpp test_lib.hpp test_run_comp_prio_veb.cpp $(PAPI) -o run_comp_veb

dijkstra_tester : binary_heap.hpp priority_queue.hpp fibonacci_queue.hpp dijkstra.hpp dijkstra_tester.cpp
	$(CC) $(LFLAGS) binary_heap.hpp priority_queue.hpp fibonacci_queue.hpp dijkstra.hpp dijkstra_tester.cpp $(PAPI) -o dijkstra_tester

clean :
	\rm *.o *~ binary_heap_test
