HOBJS = heap_test.o
DOBJS = dijkstra.o
CC = c++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

test : $(HOBJS)
	$(CC) $(LFLAGS) $(HOBJS) -o binary_heap_test

dijkstra : $(DOBJS)
	$(CC) $(LFLAGS) $(DOBJS) -o dijkstra

dijkstra.o : priority_queue.hpp binary_heap.hpp dijkstra.cpp
	$(CC) $(CFLAGS) dijkstra.cpp

heap_test.o : priority_queue.hpp binary_heap.hpp heap_test.cpp
	$(CC) $(CFLAGS) heap_test.cpp

clean :
	\rm *.o *~ binary_heap_test
