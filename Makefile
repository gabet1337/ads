HOBJS = heap_test.o
FOBJS = fibo_tester.o
DOBJS = dijkstra.o
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

h_dijkstra : h_dijkstra_tester.cpp
	$(CC) $(LFLAGS) h_dijkstra_tester.cpp -o h_dijkstra

dijkstra.o : priority_queue.hpp binary_heap.hpp dijkstra.cpp
	$(CC) $(CFLAGS) priority_queue.hpp binary_heap.hpp dijkstra.cpp

heap_test.o : priority_queue.hpp binary_heap.hpp heap_test.cpp
	$(CC) $(CFLAGS) heap_test.cpp

fibo_tester.o : priority_queue.hpp fibonacci_heap.hpp fibo_tester.cpp
	$(CC) $(CFLAGS) fibo_tester.cpp

clean :
	\rm *.o *~ binary_heap_test
