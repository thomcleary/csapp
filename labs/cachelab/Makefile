#
# Student makefile for Cache Lab
# Note: requires a 64-bit x86-64 system
#
CC = gcc
CFLAGS = -g -Wall -Werror -std=gnu99 -m64

all: csim test-trans tracegen

csim: csim.c cachelab.c cachelab.h ./src/sim.o ./src/cache.o ./src/traces.o
	$(CC) $(CFLAGS) -o csim csim.c cachelab.c ./src/sim.o ./src/cache.o ./src/traces.o -lm

./src/sim.o: ./src/sim.c ./src/sim.h ./src/cache.h ./src/traces.h
	$(CC) $(CFLAGS) -c ./src/sim.c -o ./src/sim.o

./src/cache.o: ./src/cache.c ./src/cache.h
	$(CC) $(CFLAGS) -c ./src/cache.c -o ./src/cache.o

#
# Clean the src dirctory
#
clean:
	rm -rf *.o
	rm -rf ./src/*.o
	rm -f *.tar
	rm -f csim
	rm -f .csim_results .marker
