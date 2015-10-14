HOBJS = heap_test.o
FOBJS = fibo_tester.o
DOBJS = dijkstra.o
PAPI = /usr/lib/x86_64-linux-gnu/libpapi.so
CC = c++
DEBUG = -g
CFLAGS = -Wall -c -std=c++11 $(DEBUG)
LFLAGS = -Wall -std=c++11 $(DEBUG)

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

dijkstra_tester : binary_heap.hpp priority_queue.hpp fibonacci_queue.hpp dijkstra.hpp dijkstra_tester.cpp
	$(CC) $(LFLAGS) binary_heap.hpp priority_queue.hpp fibonacci_queue.hpp dijkstra.hpp dijkstra_tester.cpp $(PAPI) -o dijkstra_tester

clean :
	\rm *.o *~ binary_heap_test
