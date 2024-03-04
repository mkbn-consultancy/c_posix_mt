# makefile for thread pool
#

CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread

all: client.o thread_pool.o
	$(CC) $(CFLAGS) -o test client.o thread_pool.o $(PTHREADS)

client.o: client.c
	$(CC) $(CFLAGS) -c client.c $(PTHREADS)

threadpool.o: threadpool.c threadpool.h
	$(CC) $(CFLAGS) -c thread_pool.c $(PTHREADS)

clean:
	rm -rf *.o
	rm -rf test