CC=mpicc
CFLAGS=-c -Wall

all: vectorcompute.o vectorcompute_debug.o vectorcompute_noblock.o

vectorcompute.o : vectorcompute.c
	$(CC) $(CFLAGS) vectorcompute.c

vectorcompute_debug.o: vectorcompute_debug.c
	$(CC) $(CFLAGS) vectorcompute_debug.c

vectorcompute_noblock.o : vectorcompute_noblock.c
	$(CC) $(CFLAGS) vectorcompute_noblock.c

clean: 
	rm -rf *.o
