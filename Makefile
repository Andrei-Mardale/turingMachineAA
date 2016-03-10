CC = gcc -c
LD = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g

build: main

run: build
	./main
	
main: main.o turing.o
	$(LD) $^ $(CFLAGS) -o $@

main.o: main.c turing.h
	$(CC) $< $(CFLAGS) -o $@

turing.o: turing.c turing.h
	$(CC) $< $(CFLAGS) -o $@

clean:
	rm -f *.o
	rm -f main
