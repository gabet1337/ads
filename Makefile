OBJS = heap_test.o
CC = c++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

test : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o binary_heap_test

heap_test.o : binary_heap.hpp heap_test.cpp
	$(CC) $(CFLAGS) heap_test.cpp

clean :
	\rm *.o *~ binary_heap_test
