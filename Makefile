CC = g++ -O2 -Wno-deprecated

all: hashtagcounter 

hashtagcounter: App.o FibonacciHeap.o main.o
	$(CC) -o hashtagcounter App.o FibonacciHeap.o main.o

main.o: main.cpp
	$(CC) -g -c main.cpp

App.o: App.cpp
	$(CC) -g -c App.cpp

FibonacciHeap.o: FibonacciHeap.cpp
	$(CC) -g -c FibonacciHeap.cpp

clean: 
	rm -f *.o
	rm -f hashtagcounter