CC=mpic++

CFLAGS=-c -Wall

hierachical_clustering : main.o compute.o
	$(CC) -o hierachical_clustering main.o compute.o

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

compute.o : compute.cpp
	$(CC) $(CFLAGS) compute.cpp

clean: 
	rm -f *.o hierachical_clustering
